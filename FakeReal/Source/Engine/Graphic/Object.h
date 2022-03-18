#pragma once
#include <vector>
#include <unordered_map>
#include "Stream/Rtti.h"
#include "Stream/StreamUtil.h"
#include "Priority.h"
#include "../Core/EngineMain.h"
#include "../Core/CoreMarco.h"

namespace FakeReal {
	class Object;
	class FastObjectManager
	{
	public:
		enum
		{
			MAX_OBJ_FLAG = 100000,
			MAX_OBJ_NUM = MAX_OBJ_FLAG - 1,
		};

		FastObjectManager();
		~FastObjectManager();

		unsigned int AddObject(Object* pObj);
		void DeleteObject(Object* pObj);

	protected:
		Object* mObjectArray[MAX_OBJ_FLAG];
		std::vector<unsigned int> mFreeTable;
	};

	class ResourceProxyBase;
	typedef Object* (*CreateFunction)();
	class FR_ENGINE_API Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		Object();
		Object(const Object& rObj);
		Object& operator = (const Object& rObj);
		virtual ~Object();

	public:
		/*无参构造对象开始*/
		/*下面这些函数构造的对象是默认对象，需要自行初始化对象的信息。*/
		static Object* GetInstance(const std::string& objName);

		//只能用于创建继承于Object的对象
		template<class T>
		static T* GetInstance()
		{
			return (T*)GetInstance(T::ms_Type.GetName());
		}
		/*无参构造对象结束*/

		//template<class T, typename ... Args>
		//static T* CreateObjectWithParam(Args&& ... args)
		//{
		//	T* ptr = new T(eastl::forward<Args>(args) ...);
		//	//GetFastObjectManager().AddObject((Object*)ptr);
		//	return ptr;
		//}

	public:
		static void CloneObject(Object* pObjectSrc, Object* pObjectDest);
		static Object* CloneCreateObject(Object* pObject);
		static Object* _CloneCreateObject(Object * pObject, std::unordered_map<Object *, Object*>& CloneMap);
		static void _CloneObject(Object * pObjectSrc, Object * pObjectDest, std::unordered_map<Object *, Object*>& CloneMap);

		virtual bool BeforeSave(void * pData = nullptr);
		virtual bool PostSave(void * pData = nullptr);
		virtual bool PostLoad(void * pData = nullptr);
		virtual bool PostClone(Object * pObjectSrc);
		virtual void PostCreate();
		virtual void LoadedEvent(ResourceProxyBase* pResourceProxy, int data = 0);

		unsigned int GetID() const { return m_uiID; }

		bool IsSameType(const Object* pObject)const;
		bool IsDerived(const Object* pObject)const;
		bool IsSameType(const Rtti& Type)const;
		bool IsDerived(const Rtti& Type)const;

	public:
		enum //Object Flag
		{
			OF_REACH = 0x01,
			OF_UNREACH = 0x02,
			OF_PendingKill = 0x04,
			OF_GCObject = 0x08,
			OF_RootObject = 0x10,
			OF_MAX
		};
		inline void SetFlag(unsigned int uiFlag)
		{
			m_uiFlag |= uiFlag;
		}

	public:
		friend class FastObjectManager;
		static FastObjectManager& GetFastObjectManager()
		{
			static FastObjectManager ms_FastObjectManager;
			return ms_FastObjectManager;
		}
	protected:
		unsigned int m_uiID;
		unsigned int m_uiFlag;
		static std::unordered_map<std::string, CreateFunction> ms_ClassFactoryMap;

	};
	FR_TYPE_MARCO(Object)

	template<class T>
	T* DynamicCast(Object* pObj)
	{
		return pObj && pObj->IsDerived(T::ms_Type) ? (T*)pObj : nullptr;
	}

	template<class T>
	const T* DynamicCast(const Object * pObj)
	{
		return pObj && pObj->IsDerived(T::ms_Type) ? (const T*)pObj : nullptr;
	}
}

