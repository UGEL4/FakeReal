#include "Stream.h"
#include <fstream>
#include <xiosbase>
#include <iostream>
#include "Property.h"
#include "../Resource/ResourceManager.h"

namespace FakeReal {

	Stream::Stream()
	{
		m_uiStreamFlag = 0;
		m_uiVersion = 0;
		m_uiArchivePropertySize = 0;
		m_uiBufferSize = 0;
		m_pBuffer = nullptr;
		m_pCurrBuffer = nullptr;
		mObjectArray.clear();
		mPostLoadObjectArray.clear();
		mLoadMap.clear();
		mFRTypeLoadMap.clear();
	}

	Stream::~Stream()
	{
		mObjectArray.clear();
		m_pCurrBuffer = nullptr;
	}

	bool Stream::ArchiveAll(Object* pObj)
	{
		if (!pObj)
		{
			return false;
		}
		if (m_uiStreamFlag == AT_REGISTER)
		{
			if (RegisterObject(pObj))
			{
				Rtti& pRtti = pObj->GetType();
				for (size_t i = 0; i < pRtti.GetPropertyNum(); i++)
				{
					Property* pProperty = pRtti.GetProperty(i);
					if (pProperty->GetFlag() & Property::F_SAVE_LOAD)
					{
						pProperty->Archive(*this, pObj);
					}
				}
			}
		}
		else if (m_uiStreamFlag == AT_POSTLOAD)
		{
			if (RegisterPostLoadObject(pObj))
			{

				Rtti &Rtti = pObj->GetType();
				for (unsigned int j = 0; j < Rtti.GetPropertyNum(); j++)
				{
					Property * pProperty = Rtti.GetProperty(j);
					if (pProperty->GetFlag() & Property::F_SAVE_LOAD)
					{
						pProperty->Archive(*this, pObj);
					}
				}
				pObj->PostLoad();
			}
		}
		return true;
	}

	bool Stream::RegisterObject(Object* pObj)
	{
		if (!pObj)
		{
			return false;
		}
		for (size_t i = 0; i < mObjectArray.size(); i++)
		{
			if (pObj == mObjectArray[i])
			{
				return false;
			}
		}
		mObjectArray.emplace_back(pObj);
		return true;
	}

	bool Stream::RegisterPostLoadObject(Object* pObject)
	{
		for (size_t i = 0; i < mPostLoadObjectArray.size(); i++)
		{
			if (mPostLoadObjectArray[i] == pObject)
			{
				return 0;
			}

		}
		mPostLoadObjectArray.emplace_back(pObject);
		return true;
	}

	bool Stream::NewSave(const char* file)
	{
		unsigned int objNum = static_cast<unsigned int>(mObjectArray.size());
		
		//版本号
		m_uiBufferSize += sizeof(unsigned int);
		//Object数量
		m_uiBufferSize += sizeof(unsigned int);

		//生成ObjectTable列表
		std::vector<ObjectTable> ObjectTableArray;
		ObjectTableArray.resize(objNum);
		for (unsigned int i = 0; i < objNum; i++)
		{
			//标识符
			m_uiBufferSize += sizeof(Object*);
			ObjectTableArray[i].m_pAddr = mObjectArray[i];
			//Rtti
			m_uiBufferSize += GetStrDistUse(mObjectArray[i]->GetType().GetName());
			ObjectTableArray[i].mRttiName = mObjectArray[i]->GetType().GetName();
			//对象所有属性占用的空间大小
			m_uiBufferSize += sizeof(unsigned int);
			//对象属性的数量
			m_uiBufferSize += sizeof(unsigned int);
		}

		//创建ObjectPropertyTable并计算空间大小
		m_uiStreamFlag = AT_SIZE;
		for (unsigned int i = 0; i < objNum; i++)
		{
			//ObjectPropertyTable距离首地址偏移
			ObjectTableArray[i].m_uiOffSet = m_uiBufferSize;

			Rtti& rtti = mObjectArray[i]->GetType();
			ObjectTableArray[i].mObjectPropertyArray.resize(rtti.GetPropertyNum());
			ObjectTableArray[i].m_uiObjectPropertyNum = 0;
			for (unsigned int j = 0; j < rtti.GetPropertyNum(); j++)
			{
				Property* pProperty = rtti.GetProperty(j);
				if (pProperty->GetFlag() & Property::F_SAVE_LOAD)
				{
					//属性名
					m_uiBufferSize += GetStrDistUse(pProperty->GetName());
					ObjectTableArray[i].mObjectPropertyArray[j].mName = pProperty->GetName();
					//属性距离首地址的偏移量
					m_uiBufferSize += sizeof(unsigned int);
					ObjectTableArray[i].m_uiObjectPropertyNum++;
				}
			}
			//ObjectPropertyTable占用的空间大小
			ObjectTableArray[i].m_uiObjectPropertyTableSize = m_uiBufferSize - ObjectTableArray[i].m_uiOffSet;
			for (unsigned int j = 0; j < rtti.GetPropertyNum(); j++)
			{
				Property* pProperty = rtti.GetProperty(j);
				if (pProperty->GetFlag() & Property::F_SAVE_LOAD)
				{
					//属性距离首地址的偏移量
					ObjectTableArray[i].mObjectPropertyArray[j].m_uiOffSet = m_uiBufferSize;
					//计算属性占用数据大小
					m_uiArchivePropertySize = 0;
					pProperty->Archive(*this, mObjectArray[i]);
					ObjectTableArray[i].mObjectPropertyArray[j].m_uiSize = m_uiArchivePropertySize;
					m_uiBufferSize += m_uiArchivePropertySize;
				}
			}
			/*从for(unsigned int i = 0; ...)开始到这里，m_uiBufferSize累加了m_uiObjectPropertyTableSize和m_uiOffSet，
			因此把这两个值减掉就是m_uiObjectPropertySize，即所有属性占用的空间大小
			*/
			ObjectTableArray[i].m_uiObjectPropertySize = m_uiBufferSize - ObjectTableArray[i].m_uiObjectPropertyTableSize - ObjectTableArray[i].m_uiOffSet;
		}

		for (unsigned int i = 0; i < objNum; i++)
		{
			//mObjectArray[i]->BeforeSave();
		}

		//申请存储空间
		if (m_pBuffer) delete[] m_pBuffer;
		m_pBuffer = nullptr;
		m_pBuffer = new unsigned char[m_uiBufferSize];
		if (!m_pBuffer)
		{
			return false;
		}
		m_pCurrBuffer = m_pBuffer;
		
		//版本号
		m_uiVersion = m_uiCurrVersion;
		Write(&m_uiVersion, sizeof(unsigned int));
		//Object数量
		Write(&objNum, sizeof(unsigned int));

		//存储ObjectTable列表
		//先存储标识符
		for (size_t i = 0; i < ObjectTableArray.size(); i++)
		{
			//标识符
			if (!Write(&ObjectTableArray[i].m_pAddr, sizeof(Object*)))
			{
				delete[] m_pBuffer;
				return false;
			}
			//Rtti名字
			if (!WriteString(ObjectTableArray[i].mRttiName))
			{
				delete[] m_pBuffer;
				return false;
			}
			//对象所有属性占用的空间大小
			Write(&ObjectTableArray[i].m_uiObjectPropertySize, sizeof(unsigned int));
			//对象属性的数量
			Write(&ObjectTableArray[i].m_uiObjectPropertyNum, sizeof(unsigned int));
		}

		//存储ObjectTable
		m_uiStreamFlag = AT_SAVE;
		for (size_t i = 0; i < objNum; i++)
		{
			Rtti& rtti = mObjectArray[i]->GetType();
			for (size_t j = 0; j < rtti.GetPropertyNum(); j++)
			{
				Property* pProperty = rtti.GetProperty(j);
				if (pProperty->GetFlag() & Property::F_SAVE_LOAD)
				{
					if (!WriteString(ObjectTableArray[i].mObjectPropertyArray[j].mName))
					{
						delete[] m_pBuffer;
						return false;
					}
					unsigned int val = ObjectTableArray[i].mObjectPropertyArray[j].m_uiOffSet;
					if (!Write(&ObjectTableArray[i].mObjectPropertyArray[j].m_uiOffSet, sizeof(unsigned int)))
					{
						delete[] m_pBuffer;
						return false;
					}
				}
			}
			for (size_t j = 0; j < rtti.GetPropertyNum(); j++)
			{
				Property* pProperty = rtti.GetProperty(j);
				if (pProperty->GetFlag() & Property::F_SAVE_LOAD)
				{
					pProperty->Archive(*this, mObjectArray[i]);
				}
			}
		}

		for (unsigned int i = 0; i < objNum; i++)
		{
			//mObjectArray[i]->PostSave();
		}

		//写入文件
		std::ofstream* pOutFile = new std::ofstream;
		if (!pOutFile)
		{
			delete[] m_pBuffer;
			return false;
		}
		pOutFile->open(file, std::ios::binary);
		if (!pOutFile->is_open())
		{
			delete[] m_pBuffer;
			delete pOutFile;
			return false;
		}
		pOutFile->write((const char*)m_pBuffer, m_uiBufferSize);

		pOutFile->close();
		delete[] m_pBuffer;
		delete pOutFile;

		return true;
	}

	bool Stream::NewLoad(const char* file)
	{
		//读取文件
		std::ifstream* pinFile = new std::ifstream;
		if (!pinFile)
		{
			return false;
		}
		pinFile->open(file, std::ios::binary);
		if (!pinFile->is_open())
		{
			delete pinFile;
			return false;
		}

		unsigned int uiBufferSize = 0;
		pinFile->seekg(0, std::ios_base::end);
		uiBufferSize = pinFile->tellg();
		pinFile->seekg(0, std::ios_base::beg);
		unsigned char* pBuffer = new unsigned char[uiBufferSize];
		if (!pBuffer)
		{
			pinFile->close();
			delete pinFile;
			return false;
		}
		pinFile->read((char*)pBuffer, uiBufferSize);
		if (!NewLoadFromeBuffer(pBuffer, uiBufferSize))
		{
			delete[] pBuffer;
			pinFile->close();
			delete pinFile;
			return false;
		}

		delete[] pBuffer;
		pinFile->close();
		delete pinFile;
		return true;
	}

	bool Stream::NewLoadFromeBuffer(unsigned char* pBuffer, unsigned int uiBuffSize)
	{
		if (!pBuffer || !uiBuffSize)
		{
			return false;
		}

		mObjectArray.clear();
		mLoadMap.clear();
		m_pCurrBuffer = nullptr;
		m_uiBufferSize = uiBuffSize;

		m_pBuffer = pBuffer;
		m_pCurrBuffer = pBuffer;

		//版本号
		Read(&m_uiCurrVersion, sizeof(unsigned int));
		//对象数量
		unsigned int uiObjNum = 0;
		Read(&uiObjNum, sizeof(unsigned int));

		//生成ObjectTable列表
		std::vector<ObjectTable> ObjectTableArray;
		ObjectTableArray.resize(uiObjNum);
		for (unsigned int i = 0; i < uiObjNum; i++)
		{
			//标识符
			Object* pObject = nullptr;
			if (!Read(&ObjectTableArray[i].m_pAddr, sizeof(Object*)))
			{
				return false;
			}
			//Rtti
			if (!ReadString(ObjectTableArray[i].mRttiName))
			{
				return false;
			}
			//对象所有属性占用的空间大小
			if (!Read(&ObjectTableArray[i].m_uiObjectPropertySize, sizeof(unsigned int)))
			{
				return false;
			}
			//对象属性的数量
			if (!Read(&ObjectTableArray[i].m_uiObjectPropertyNum, sizeof(unsigned int)))
			{
				return false;
			}
		}
		//创建空的对象
		for (unsigned int i = 0; i < uiObjNum; i++)
		{
			Object* pObject = nullptr;
			pObject = Object::GetInstance(ObjectTableArray[i].mRttiName);
			if (!pObject)
			{
				continue;
			}
			//创建指针映射表
			mLoadMap.emplace(ObjectTableArray[i].m_pAddr, pObject);
			RegisterObject(pObject);
		}
		//加载属性表
		for (unsigned int i = 0; i < uiObjNum; i++)
		{
			ObjectTableArray[i].mObjectPropertyArray.resize(ObjectTableArray[i].m_uiObjectPropertyNum);
			for (unsigned int j = 0; j < ObjectTableArray[i].m_uiObjectPropertyNum; j++)
			{
				ReadString(ObjectTableArray[i].mObjectPropertyArray[j].mName);
				Read(&(ObjectTableArray[i].mObjectPropertyArray[j].m_uiOffSet), sizeof(unsigned int));
			}
			m_pCurrBuffer += ObjectTableArray[i].m_uiObjectPropertySize;
		}
		//加载属性
		m_uiStreamFlag = AT_LOAD;
		for (size_t i = 0; i < mObjectArray.size(); i++)
		{
			Rtti& rtti = mObjectArray[i]->GetType();
			for (size_t j = 0; j < rtti.GetPropertyNum(); j++)
			{
				Property* pProperty = rtti.GetProperty(j);
				if (pProperty->GetFlag() & Property::F_SAVE_LOAD)
				{
					m_pCurrBuffer = m_pBuffer + ObjectTableArray[i].mObjectPropertyArray[j].m_uiOffSet;
					pProperty->Archive(*this, mObjectArray[i]);
				}
			}
		}
		//链接对象
		m_uiStreamFlag = AT_LINK;
		//必须从后往前便利，因为register的过程是递归的深度注册，所以从后往前link保证子节点先完成，然后父亲节点完成。
		for (int i = mObjectArray.size() - 1; i >= 0; i--)//i--千万不要用unsigned int
		{
			Rtti& rtti = mObjectArray[i]->GetType();
			for (size_t j = 0; j < rtti.GetPropertyNum(); j++)
			{
				Property* pProperty = rtti.GetProperty(j);
				if (pProperty->GetFlag() & Property::F_SAVE_LOAD)
				{
					pProperty->Archive(*this, mObjectArray[i]);
				}
			}
		}

		m_pBuffer = nullptr;
		return true;
	}

	int Stream::GetStrDistUse(const std::string &Str)
	{
		return sizeof(unsigned int) + Str.length() * sizeof(char);
	}

	int Stream::GetStrDistUse(const char* pCh)
	{
		return sizeof(unsigned int) + strlen(pCh) * sizeof(char);
	}

	int Stream::GetResourceDistUse(ResourceProxyBase* pResource)
	{
		if (pResource)
		{
			std::string& name = pResource->GetResourceName();
			return GetStrDistUse(name);
		}
		return 0;
	}

	bool Stream::Write(const void * pvBuffer, unsigned int uiSize)
	{
		if (!pvBuffer || !m_pBuffer)
		{
			return false;
		}

		if (unsigned int(m_pCurrBuffer + uiSize - m_pBuffer) > m_uiBufferSize)
		{
			std::cout << "(m_pCurrBuffer + uiSize - m_pBuffer) > m_uiBufferSize:" << m_pCurrBuffer + uiSize - m_pBuffer << ", " << m_uiBufferSize << std::endl;
			return false;
		}

		//windows
		memcpy_s(m_pCurrBuffer, uiSize, pvBuffer, uiSize);

		m_pCurrBuffer += uiSize;
		return true;
	}

	bool Stream::WriteString(const std::string& str)
	{
		unsigned int uiBufferLen = str.length() * sizeof(char);
		//字符串字节数
		if (!Write(&uiBufferLen, sizeof(unsigned int)))
		{
			return false;
		}
		if (!Write(str.data(), uiBufferLen))
		{
			return false;
		}

		return true;
	}

	bool Stream::Read(void* pvBuffer, unsigned int uiSize)
	{
		if (!pvBuffer || !m_pBuffer)
		{
			return false;
		}

		if (unsigned int(m_pCurrBuffer + uiSize - m_pBuffer) > m_uiBufferSize)
		{
			return false;
		}

		//windows
		memcpy_s(pvBuffer, uiSize, m_pCurrBuffer, uiSize);

		m_pCurrBuffer += uiSize;
		return true;
	}

	bool Stream::ReadString(std::string& str)
	{
		unsigned int uiBufferLen = 0;
		if (!Read(&uiBufferLen, sizeof(unsigned int)))
		{
			return false;
		}
		if (uiBufferLen == 0)
		{
			return 1;
		}
		//字符串字节数
		char* pCh = nullptr;
		pCh = new char[uiBufferLen];
		if (!Read(pCh, uiBufferLen))
		{
			delete[] pCh;
			return false;
		}
		str.assign(pCh, uiBufferLen);
		return true;
	}

	const Object* Stream::GetMapValue(Object* key) const
	{
		std::unordered_map<Object*, Object*>::const_iterator itr = mLoadMap.find(key);
		if (itr != mLoadMap.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	Object* Stream::GetFRTypeMapValue(Object* key) const
	{
		std::unordered_map<Object*, Object*>::const_iterator itr = mFRTypeLoadMap.find(key);
		if (itr != mFRTypeLoadMap.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	const Object* Stream::GetObjectByRtti(const Rtti &Rtti)
	{
		Object* pObject = NULL;
		for (size_t i = 0; i < mObjectArray.size(); i++)
		{
			if ((mObjectArray[i]->GetType()).IsSameType(Rtti))
			{
				pObject = mObjectArray[i];
				break;
			}
		}
		if (pObject == NULL)
		{
			for (size_t i = 0; i < mObjectArray.size(); i++)
			{
				if ((mObjectArray[i]->GetType()).IsDerived(Rtti))
				{

					pObject = mObjectArray[i];
					break;
				}
			}
		}

		if (pObject)
		{
			m_uiStreamFlag = AT_POSTLOAD;
			ArchiveAll(pObject);
		}
		return pObject;
	}

	bool Stream::GetObjectArrayByRtti(const Rtti &Rtti, std::vector<Object *>& ObjectArray, bool IsDerivedFrom /*= false*/)
	{
		ObjectArray.clear();
		for (unsigned int i = 0; i < mObjectArray.size(); i++)
		{
			if ((mObjectArray[i]->GetType()).IsSameType(Rtti) || ((mObjectArray[i]->GetType()).IsDerived(Rtti) && IsDerivedFrom))
			{

				ObjectArray.emplace_back(mObjectArray[i]);
			}
		}
		if (ObjectArray.size() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Stream::WriteResource(ResourceProxyBase*& pResource)
	{
		bool isNone = true;
		if (pResource)
		{
			isNone = false;
		}
		Write(&isNone, sizeof(bool));
		if (pResource)
		{
			std::string& name = pResource->GetResourceName();
			WriteString(name);
		}
		return true;
	}

	bool Stream::ReadResource(ResourceProxyBase*& pResource)
	{
		bool isNone = true;
		Read(&isNone, sizeof(bool));
		if (isNone == false)
		{
			std::string name;
			ReadString(name);
			pResource = ResourceManager::LoadResource(name);
			if (!pResource)
			{
				assert(0);
				return false;
			}
		}
		return true;
	}

}
