#pragma once
#include <string>
#include <unordered_map>
#include "../Object.h"
namespace FakeReal {
	class FR_ENGINE_API Resource
	{
	public:
		enum	//Resource Type
		{
			RT_TEXTURE,
			RT_SKELETON_MODEL,
			RT_STATIC_MODEL,
			RT_ANIMATION,
			RT_ACTOR,
			RT_MATERIAL,
			/*RT_POSTEFFECT,
			RT_SHADER,
			RT_ANIMTREE,
			RT_MORPHTREE,
			RT_TERRAIN,
			RT_FSM,
			RT_ACTOR,
			RT_MAP,
			RT_FONTTYPE,*/
			RT_MAX
		};
		Resource();
		virtual ~Resource() = 0;

		void SetResourceName(const std::string& name)
		{
			mResourceName = name;
		}
		
		std::string& GetResourceName()
		{
			return mResourceName;
		}

		virtual unsigned int GetResourceType() const = 0;

		static std::string& GetFileSuffix(unsigned int uiIndex);

	protected:
		std::string mResourceName;
		static std::string ms_FileSuffix[];
	};

	class FR_ENGINE_API ResourceProxyBase
	{
	public:
		ResourceProxyBase();
		virtual ~ResourceProxyBase() = 0;

		virtual std::string& GetResourceName() = 0;
		virtual bool IsLoaded() = 0;
		virtual unsigned int GetResourceType() = 0;

		void SetResourceName(const std::string& name)
		{
			mResourceName = name;
		}

		void LoadEvent()
		{
			std::unordered_map<Object*, int>::iterator itr = m_LoadedEventObject.begin();
			for (; itr != m_LoadedEventObject.end(); ++itr)
			{
				itr->first->LoadedEvent(this, itr->second);
			}
			m_LoadedEventObject.clear();
		}
		void AddLoadEventObject(Object * pObject, int Data = 0)
		{
			if (!pObject)
			{
				return;
			}
			if (mIsLoaded == false)
			{
				m_LoadedEventObject.emplace(pObject, Data);
			}
			else
			{
				pObject->LoadedEvent(this);
			}
		}
	protected:
		std::string mResourceName;
		bool mIsLoaded;
		std::unordered_map<Object*, int> m_LoadedEventObject;

		void Loaded()
		{
			mIsLoaded = true;
			LoadEvent();
		}
	};

	template <class T>
	class ResourceProxy : public ResourceProxyBase
	{
	public:
		friend class ResourceManager;
		ResourceProxy();
		virtual ~ResourceProxy();

		//这个函数并不会释放旧的资源，如果已经有旧的资源，需要手动释放旧的资源
		void SetNewResource(T* pResource);
		virtual std::string& GetResourceName() override;
		virtual bool IsLoaded() override;
		virtual unsigned int GetResourceType() override;

		T* GetResource();
		static ResourceProxy<T>* Create(T* pResource);
	private:
		T* m_pResource;
	};

	template <class T>
	ResourceProxy<T>::ResourceProxy()
	{
		m_pResource = nullptr;
	}

	template <class T>
	ResourceProxy<T>::~ResourceProxy()
	{
		if (m_pResource)
			delete m_pResource;
		m_pResource = nullptr;
		std::cout << "析构ResourceProxy:" << this << std::endl;
	}

	template <class T>
	void ResourceProxy<T>::SetNewResource(T* pResource)
	{
		m_pResource = pResource;
		pResource->SetResourceName(mResourceName);
	}

	template <class T>
	std::string& ResourceProxy<T>::GetResourceName()
	{
		return mResourceName;
	}

	template <class T>
	bool ResourceProxy<T>::IsLoaded()
	{
		return mIsLoaded;
	}

	template <class T>
	unsigned int ResourceProxy<T>::GetResourceType()
	{
		return GetResource()->GetResourceType();
	}

	template <class T>
	T* ResourceProxy<T>::GetResource()
	{
		if (mIsLoaded)
			return m_pResource;
		return nullptr;
	}

	template <class T>
	ResourceProxy<T>* ResourceProxy<T>::Create(T* pResource)
	{
		ResourceProxy<T>* pProxy = new ResourceProxy<T>();
		pProxy->SetResourceName(pResource->GetResourceName());
		pProxy->SetNewResource(pResource);
		pProxy->Loaded();
		return pProxy;
	}

#define  DECLARE_Proxy(ClassName)\
	typedef ResourceProxy<ClassName> ##ClassName##R;\
	typedef ResourceProxy<ClassName>* ##ClassName##RPtr;
}
