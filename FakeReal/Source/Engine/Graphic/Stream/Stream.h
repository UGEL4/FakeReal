#pragma once

#include "StreamUtil.h"
#include "../Object.h"
#include "../Resource/Resource.h"
#include "../CustomArchiveObject.h"
#include <assert.h>
#include <vector>
namespace FakeReal {

	// Ĭ��cloneָ������VSType property ���ǻ����´���һ��ʵ��Ȼ���ٿ������ݣ�Ϊ���ò����´��������property��ʱ�����property��Ҫ����VSProperty::F_CLONE��־
//Ȼ����postclone �������Լ����������� ���ֻ��Ĭ�ϵ�ֵ�������Ǵ���ʵ����������F_COPY
	template<typename T>
	void Copy(T & Dest, T & Src, std::unordered_map<Object *, Object*>& CloneMap)
	{
		if (TIsFRPointerType<T>::Value)
		{
			Object* & TempSrc = *(Object**)(void *)&Src;
			Object* & TempDest = *(Object**)(void *)&Dest;
			TempDest = Object::_CloneCreateObject(TempSrc, CloneMap);
		}
		else if (TIsFRType<T>::Value)
		{
			Object* TempSrc = (Object *)&Src;
			Object* TempDest = (Object *)&Dest;
			Object::_CloneObject(TempSrc, TempDest, CloneMap);
		}
		else if (TIsFRSmartPointerType<T>::Value)
		{
			std::shared_ptr<Object> & TempSrc = *(std::shared_ptr<Object>*)(void *)&Src;
			std::shared_ptr<Object> & TempDest = *(std::shared_ptr<Object>*)(void *)&Dest;
			Object* pNewObj = Object::_CloneCreateObject(TempSrc.get(), CloneMap);
			TempDest.reset(pNewObj);
		}
		else if (TIsCustomType<T>::Value)
		{
			CustomArchiveObject * TempSrc = (CustomArchiveObject*)(void *)&Src;
			CustomArchiveObject * TempDest = (CustomArchiveObject*)(void *)&Dest;
			TempDest->CopyFrom(TempSrc, CloneMap);
		}
		else
		{
			Dest = Src;
		}
	}
	template<typename T>
	void Copy(std::vector<T>& Dest, std::vector<T>& Src, std::unordered_map<Object*, Object*>& CloneMap)
	{
		Dest.clear();
		Dest.resize(Src.size());
		for (size_t i = 0; i < Src.size(); i++)
		{
			Copy(Dest[i], Src[i], CloneMap);
		}
	}

	template<class Key, class Value>
	void Copy(std::unordered_map<Key, Value>& Dest, std::unordered_map<Key, Value>& Src, std::unordered_map<Object*, Object*>& CloneMap)
	{
		Dest.clear();
		typename std::unordered_map<Key, Value>::iterator srcIter = Src.begin();
		for (; srcIter != Src.end(); ++srcIter)
		{
			Key srcKey = srcIter->first;
			Value srcValue = srcIter->second;
			Key destKey;
			Value destValue;
			Copy(destKey, srcKey, CloneMap);
			Copy(destValue, srcValue, CloneMap);
			Dest.emplace(destKey, destValue);
		}
	}

	class FR_ENGINE_API Stream
	{
	public:
		enum // Archive Type
		{
			AT_SAVE,//����
			AT_LOAD,//����
			AT_LINK,//����
			AT_REGISTER,//ע��
			AT_SIZE,//���ݴ�С
			AT_POSTLOAD,//����
			AT_OBJECT_COLLECT_GC,
			AT_CLEAR_OBJECT_PROPERTY_GC,
			AT_LOAD_OBJECT_COLLECT_GC,
		};

		Stream();
		~Stream();

	public:
		bool ArchiveAll(Object* pObj);
		bool RegisterObject(Object* pObj);
		bool RegisterPostLoadObject(Object* pObject);

	public:
		struct ObjectPropertyTable
		{
			std::string mName;//���Ե�����
			unsigned int m_uiOffSet;//���Ծ����׵�ַ��ƫ����
			unsigned int m_uiSize;//���ԵĴ�С
			unsigned int m_uiNameID;
		};
		struct ObjectTable
		{
			Object* m_pAddr;//ʹ�ö����ַ��ΪΨһ��ʶ�����б��棬����ʱ���ݱ�ʶ���ָ�ָ��ָ��
			std::string mRttiName;//Rtti�����֣��������ص�ʱ���֪��Ҫ����ʲô����
			unsigned int m_uiObjectPropertySize;//������������ռ�õĿռ��С
			unsigned int m_uiObjectPropertyNum;//�������Ե�����
			unsigned int m_uiOffSet;//ObjectPropertyTable�����׵�ַƫ��
			unsigned int m_uiObjectPropertyTableSize;//ObjectPropertyTableռ�õĿռ��С
			std::vector<ObjectPropertyTable> mObjectPropertyArray;//Object���Ա�����
		};

		virtual bool NewSave(const char* file);
		virtual bool NewLoad(const char* file);
		virtual bool NewLoadFromeBuffer(unsigned char* pBuffer, unsigned int uiBuffSize);

	public:
		int GetStrDistUse(const std::string &Str);
		int GetStrDistUse(const char* pCh);
		int GetResourceDistUse(ResourceProxyBase* pResource);
		bool Write(const void* pvBuffer, unsigned int uiSize);
		bool WriteString(const std::string& str);
		bool Read(void* pvBuffer, unsigned int uiSize);
		bool ReadString(std::string& str);
		const Object* GetMapValue(Object* key) const;
		Object* GetFRTypeMapValue(Object* key) const;
		const Object *GetObjectByRtti(const Rtti &Rtti);
		bool GetObjectArrayByRtti(const Rtti &Rtti, std::vector<Object *>& ObjectArray, bool IsDerivedFrom = false);

		void SetStreamFlag(unsigned int uiStreamFlag)
		{
			m_uiStreamFlag = uiStreamFlag;
		}
		unsigned int GetStreamFlag() const
		{
			return m_uiStreamFlag;
		}
		void AddBufferSize(unsigned int uiSize)
		{
			m_uiArchivePropertySize += uiSize;
		}
	private:
		template<class T>
		bool WriteObjectPtr(T * const& pObject)
		{
			if (!Write(&pObject, sizeof(T*)))
			{
				return false;
			}
			return true;
		}
		template<class T>
		bool WriteObjectPtr(const std::shared_ptr<T>& pointer)
		{
			T* pP = pointer.get();
			if (!Write(&pP, sizeof(T*)))
			{
				return false;
			}
			return true;
		}

		bool WriteResource(ResourceProxyBase*& pResource);
		bool ReadResource(ResourceProxyBase*& pResource);

		template<class T>
		bool ReadObjectPtr(T*& pObj)
		{
			if (!Read(&pObj, sizeof(T*)))
			{
				return false;
			}
			return true;
		}
		template<class T>
		bool ReadObjectPtr(std::shared_ptr<T>& pointer)
		{
			T* pP = nullptr;
			if (!Read(&pP, sizeof(T*)))
			{
				return false;
			}
			pointer.reset(pP);
			return true;
		}

		template<class T>
		bool LinkObjectPtr(T*& pObj)
		{
			pObj = (T*)GetMapValue(pObj);

			if (!pObj)
			{
				return false;
			}
			return true;
		}
		template<class T>
		bool LinkObjectPtr(std::shared_ptr<T>& pointer)
		{
			Object* pP = (Object*)pointer.get();
			T* pObj = (T*)GetMapValue(pP);
			pointer.reset(pObj);
			if (!pointer)
			{
				return false;
			}
			return true;
		}
	private:
		unsigned int m_uiStreamFlag;
		unsigned int m_uiVersion;//�汾��
		unsigned int m_uiCurrVersion;//�汾��
		unsigned int m_uiArchivePropertySize;//�ռ����ԵĴ�С
		unsigned int m_uiBufferSize;
		unsigned char* m_pBuffer;
		unsigned char* m_pCurrBuffer;//�������α�
		std::vector<Object*> mObjectArray;
		std::vector<Object*> mPostLoadObjectArray;
		std::unordered_map<Object*, Object*> mLoadMap;
		std::unordered_map<Object*, Object*> mFRTypeLoadMap;

		/*template Archive*/
	public:
		template<class T>
		void Archive(T& io)
		{
			if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_POSTLOAD)
			{
				if (TIsFRSmartPointerType<T>::Value)
				{
					std::shared_ptr<Object>& temp = *(std::shared_ptr<Object>*)(void*)&io;
					ArchiveAll(temp.get());
				}
				else if (TIsFRPointerType<T>::Value)
				{
					Object*& temp = *(Object**)(void*)&io;
					ArchiveAll(temp);
				}
				else if (TIsFRType<T>::Value)
				{
					Object* temp = (Object*)&io;
					ArchiveAll(temp);
				}
				else if (TIsCustomType<T>::Value)
				{
					CustomArchiveObject * Temp = (CustomArchiveObject*)(void *)&io;
					if (Temp)
					{
						Temp->Archive(*this);
					}
				}
			}
			else if (m_uiStreamFlag == AT_SAVE)
			{
				if (TIsResourceProxyPointerType<T>::Value)
				{
					ResourceProxyBase*& Temp = *(ResourceProxyBase**)(void*)&io;
					WriteResource(Temp);
				}
				else if (TIsFRSmartPointerType<T>::Value)
				{
					std::shared_ptr<Object>& temp = *(std::shared_ptr<Object>*)(void*)&io;
					WriteObjectPtr(temp);
				}
				else if (TIsFRPointerType<T>::Value)
				{
					Object*& temp = *(Object**)(void*)&io;
					WriteObjectPtr(temp);
				}
				else if (TIsFRType<T>::Value)
				{
					Object* temp = (Object*)&io;
					WriteObjectPtr(temp);
				}
				else if (TIsFRStringType<T>::Value)
				{
					std::string& temp = *(std::string*)(void*)&io;
					WriteString(temp);
				}
				else if (TIsCustomType<T>::Value)
				{
					CustomArchiveObject* Temp = (CustomArchiveObject*)(void *)&io;
					if (Temp)
					{
						Temp->Archive(*this);
					}
				}
				else
				{
					Write((void*)&io, sizeof(T));
				}
			}
			else if (m_uiStreamFlag == AT_SIZE)
			{
				if (TIsResourceProxyPointerType<T>::Value)
				{
					ResourceProxyBase*& Temp = *(ResourceProxyBase**)(void*)&io;
					m_uiArchivePropertySize += GetResourceDistUse(Temp) + sizeof(bool);
				}
				else if (TIsFRSmartPointerType<T>::Value || TIsFRPointerType<T>::Value || TIsFRType<T>::Value)
				{
					m_uiArchivePropertySize += 8;
				}
				else if (TIsFRStringType<T>::Value)
				{
					std::string& temp = *(std::string*)(void *)&io;
					m_uiArchivePropertySize += GetStrDistUse(temp);
				}
				else if (TIsCustomType<T>::Value)
				{
					CustomArchiveObject * Temp = (CustomArchiveObject*)(void *)&io;
					if (Temp)
					{
						Temp->Archive(*this);
					}
				}
				else
				{
					m_uiArchivePropertySize += sizeof(T);
				}
			}
			else if (m_uiStreamFlag == AT_LOAD)
			{
				if (TIsResourceProxyPointerType<T>::Value)
				{
					ResourceProxyBase*& Temp = *(ResourceProxyBase**)(void*)&io;
					ReadResource(Temp);
				}
				else if (TIsFRPointerType<T>::Value)
				{
					Object*& temp = *(Object**)(void*)&io;
					ReadObjectPtr(temp);
				}
				else if (TIsFRSmartPointerType<T>::Value)
				{
					std::shared_ptr<Object>& temp = *(std::shared_ptr<Object>*)(void*)&io;
					ReadObjectPtr(temp);
				}
				else if (TIsFRType<T>::Value)
				{
					Object* Key = (Object*)&io;
					Object* Value = nullptr;
					ReadObjectPtr(Value);
					mFRTypeLoadMap.emplace(Key, Value);
				}
				else if (TIsFRStringType<T>::Value)
				{
					std::string& str = *(std::string*)(void*)&io;
					ReadString(str);
				}
				else if (TIsCustomType<T>::Value)
				{
					CustomArchiveObject* Temp = (CustomArchiveObject*)(void *)&io;
					if (Temp)
					{
						Temp->Archive(*this);
					}
				}
				else
				{
					Read((void*)&io, sizeof(T));
				}
			}
			else if (m_uiStreamFlag == AT_LINK)
			{
				if (TIsFRPointerType<T>::Value)
				{
					Object* & Temp = *(Object**)(void *)&io;
					LinkObjectPtr(Temp);
				}
				else if (TIsFRSmartPointerType<T>::Value)
				{
					std::shared_ptr<Object>& temp = *(std::shared_ptr<Object>*)(void*)&io;
					LinkObjectPtr(temp);
				}
				else if (TIsFRType<T>::Value)
				{
					Object*  Key = (Object *)&io;
					Object* Value = NULL;
					Value = GetFRTypeMapValue(Key);
					LinkObjectPtr(Value);
					assert(Value);
					Object::CloneObject(Value, Key);
				}
				else if (TIsCustomType<T>::Value)
				{
					CustomArchiveObject * Temp = (CustomArchiveObject*)(void *)&io;
					if (Temp)
					{
						Temp->Archive(*this);
					}
				}
			}
		}

		template<class T>
		void Archive(std::vector<T>& io)
		{
			if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK || m_uiStreamFlag == AT_POSTLOAD)
			{
				for (size_t i = 0; i < io.size(); i++)
				{
					Archive(io[i]);
				}
			}
			else if (m_uiStreamFlag == AT_SIZE)
			{
				unsigned int num = (unsigned int)io.size();
				Archive(num);
				for (unsigned int i = 0; i < num; i++)
				{
					Archive(io[i]);
				}
			}
			else if (m_uiStreamFlag == AT_SAVE)
			{
				unsigned int num = (unsigned int)io.size();
				Archive(num);
				for (unsigned int i = 0; i < num; i++)
				{
					Archive(io[i]);
				}
			}
			else if (m_uiStreamFlag == AT_LOAD)
			{
				unsigned int uiNum = 0;
				Archive(uiNum);
				io.resize(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					Archive(io[i]);
				}
			}
		}

		template<typename KEY, typename VALUE>
		void Archive(std::unordered_map<KEY, VALUE>& io)
		{
			if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK || m_uiStreamFlag == AT_POSTLOAD)
			{
				typename std::unordered_map<KEY, VALUE>::iterator itr = io.begin();
				for (; itr != io.end(); ++itr)
				{
					Archive(itr->first);
					Archive(itr->second);
				}
			}
			else if (m_uiStreamFlag == AT_SIZE)
			{
				unsigned int num = (unsigned int)io.size();
				Archive(num);
				typename std::unordered_map<KEY, VALUE>::iterator itr = io.begin();
				for (; itr != io.end(); ++itr)
				{
					Archive(itr->first);
					Archive(itr->second);
				}
			}
			else if (m_uiStreamFlag == AT_SAVE)
			{
				unsigned int num = (unsigned int)io.size();
				Archive(num);
				typename std::unordered_map<KEY, VALUE>::iterator itr = io.begin();
				for (; itr != io.end(); ++itr)
				{
					Archive(itr->first);
					Archive(itr->second);
				}
			}
			else if (m_uiStreamFlag == AT_LOAD)
			{
				unsigned int uiNum = 0;
				Archive(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					KEY key;
					VALUE value;
					Archive(key);
					Archive(value);
					io.emplace(key, value);
				}
			}
		}
	};

}
