#include "Object.h"
#include <assert.h>
#include "Stream/Property.h"
#include "Stream/StreamUtil.h"
#include "Stream/Stream.h"
namespace FakeReal {
	FastObjectManager::FastObjectManager()
	{
		mFreeTable.resize(MAX_OBJ_FLAG);
		for (int i = 0; i < MAX_OBJ_FLAG; i++)
		{
			mFreeTable[i] = i;
			mObjectArray[i] = nullptr;
		}
	}

	FastObjectManager::~FastObjectManager()
	{

	}

	unsigned int FastObjectManager::AddObject(Object* pObj)
	{
		size_t pos = mFreeTable.size() - 1;
		unsigned int ID = mFreeTable[pos];
		mObjectArray[ID] = pObj;
		mFreeTable.erase(mFreeTable.begin() + pos);
		return ID;
	}

	void FastObjectManager::DeleteObject(Object* pObj)
	{
		if (mObjectArray[pObj->m_uiID] != nullptr)
		{
			mFreeTable.emplace_back(pObj->m_uiID);
			mObjectArray[pObj->m_uiID] = nullptr;
			pObj->m_uiID = MAX_OBJ_FLAG;
		}
	}

	/*--------------------------------class Object--------------------------------*/
	std::unordered_map<std::string, CreateFunction> Object::ms_ClassFactoryMap;
	IMPLEMENT_RTTI_NO_PARENT_NO_CREATE_FUNCTION(Object);
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(Object)
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END;
	BEGIN_ADD_ROOT_PROPERTY(Object)
	END_ADD_PROPERTY

	Object::Object()
	{
		m_uiFlag = 0;
		m_uiID = GetFastObjectManager().AddObject(this);
		//std::cout << "构造Object:" << this << std::endl;
	}

	Object::Object(const Object& rObj)
	{
		m_uiFlag = rObj.m_uiFlag;
		m_uiID = GetFastObjectManager().AddObject(this);
		//std::cout << "构造Object:" << this << std::endl;
	}

	Object& Object::operator=(const Object& rObj)
	{
		m_uiFlag = rObj.m_uiFlag;
		return *this;
	}

	Object::~Object()
	{
		GetFastObjectManager().DeleteObject(this);
		//std::cout << "析构Object:" << this << std::endl;
	}

	Object* Object::GetInstance(const std::string& objName)
	{
		std::unordered_map<std::string, CreateFunction>::iterator itr = ms_ClassFactoryMap.find(objName);
		if (itr == ms_ClassFactoryMap.end())
		{
			return nullptr;
		}
		Object* pObj = itr->second();
		//GetFastObjectManager().AddObject(pObj);
		return pObj;
	}

	void Object::CloneObject(Object* pObjectSrc, Object* pObjectDest)
	{
		std::unordered_map<Object*, Object*> CloneMap;
		_CloneObject(pObjectSrc, pObjectDest, CloneMap);
		for (auto& p : CloneMap)
		{
			p.second->PostClone(p.first);
		}
	}

	Object* Object::CloneCreateObject(Object* pObject)
	{
		std::unordered_map<Object*, Object*> CloneMap;
		Object* pNewObj = _CloneCreateObject(pObject, CloneMap);
		for (auto& p : CloneMap)
		{
			p.second->PostClone(p.first);
		}
		return pNewObj;
	}

	Object* Object::_CloneCreateObject(Object * pObject, std::unordered_map<Object*, Object*>& CloneMap)
	{
		Object* pNewObject = NULL;
		if (pObject)
		{
			std::unordered_map<Object*, Object*>::iterator itr = CloneMap.find(pObject);

			if (itr == CloneMap.end())
			{
				Rtti& SrcRtti = pObject->GetType();
				pNewObject = GetInstance(SrcRtti.GetName());
				
				CloneMap.emplace(pObject, pNewObject);

				for (unsigned int i = 0; i < SrcRtti.GetPropertyNum(); i++)
				{
					Property * pProperty = SrcRtti.GetProperty(i);
					if (pProperty->GetFlag() & Property::F_CLONE)
					{
						pProperty->CloneData(pObject, pNewObject, CloneMap);
					}
					else if (pProperty->GetFlag() & Property::F_COPY)
					{
						pProperty->CopyData(pObject, pNewObject);
					}
				}
			}
			else
			{
				pNewObject = itr->second;
			}
		}
		return pNewObject;
	}

	void Object::_CloneObject(Object* pObjectSrc, Object* pObjectDest, std::unordered_map<Object*, Object*>& CloneMap)
	{
		if (!pObjectSrc)
		{
			return;
		}
		std::unordered_map<Object*, Object*>::iterator itr = CloneMap.find(pObjectSrc);

		if (itr == CloneMap.end())
		{
			Rtti& SrcRtti = pObjectSrc->GetType();
			Rtti& DestRtti = pObjectDest->GetType();
			for (unsigned int i = 0; i < SrcRtti.GetPropertyNum(); i++)
			{
				Property* pProperty = SrcRtti.GetProperty(i);
				if (pProperty->GetFlag() & Property::F_CLONE)
				{
					pProperty->CloneData(pObjectSrc, pObjectDest, CloneMap);
				}
				else if (pProperty->GetFlag() & Property::F_COPY)
				{
					pProperty->CopyData(pObjectSrc, pObjectDest);
				}
			}
		}
		else
		{
			assert(0);
		}
	}


	bool Object::BeforeSave(void * pData /*= nullptr*/)
	{
		return true;
	}

	bool Object::PostSave(void * pData /*= nullptr*/)
	{
		return true;
	}

	bool Object::PostLoad(void * pData /*= nullptr*/)
	{
		return true;
	}

	bool Object::PostClone(Object * pObjectSrc)
	{
		return true;
	}

	void Object::PostCreate()
	{

	}

	void Object::LoadedEvent(ResourceProxyBase* pResourceProxy, int data)
	{

	}

	bool Object::IsSameType(const Object* pObject) const
	{
		return pObject && GetType().IsSameType(pObject->GetType());
	}

	bool Object::IsSameType(const Rtti& Type) const
	{
		return GetType().IsSameType(Type);
	}

	bool Object::IsDerived(const Object* pObject) const
	{
		return pObject && GetType().IsDerived(pObject->GetType());
	}

	bool Object::IsDerived(const Rtti& Type) const
	{
		return GetType().IsDerived(Type);
	}

}
