#pragma once
#include <string>
#include <vector>
#include "../../Core/CoreMarco.h"

namespace FakeReal {
	class Object;
	class Property;
	typedef Object* (*CreateObjFunc)();
	class FR_ENGINE_API Rtti
	{
	public:
		friend class Object;
		Rtti(const std::string& name, Rtti* pBase, CreateObjFunc cof);
		bool IsSameType(const Rtti& type) const;
		bool IsDerived(const Rtti& type) const;

		inline const std::string& GetName() const
		{
			return mName;
		}
		inline const Rtti& GetBase() const
		{
			return *m_pBase;
		}

		Property* GetProperty(size_t uiIndex) const;
		Property* GetProperty(std::string& propertyName) const;
		size_t GetPropertyNum() const 
		{ 
			size_t size = mPropertyArray.size();
			return size; 
		}
		bool AddProperty(Property* pProperty);
		bool AddProperty(Rtti& rtti);
		void ClearProperty();
	private:
		std::string mName;
		Rtti* m_pBase;
		CreateObjFunc mCreateFunc;
		std::vector<Property*> mPropertyArray;
	};

#define DECLARE_RTTI\
	public:\
		virtual Rtti& GetType() const\
		{\
			return ms_Type;\
		}\
		static Rtti ms_Type;\
		static Priority ms_Priority;

#define IMPLEMENT_RTTI(classname, baseclassname)\
	Rtti classname::ms_Type(std::string(#classname), &baseclassname::ms_Type, &classname::FactoryFunction);\
	Priority classname::ms_Priority;

#define IMPLEMENT_RTTI_NO_CREATE_FUNCTION(classname, baseclassname)\
	Rtti classname::ms_Type(std::string(#classname), &baseclassname::ms_Type, nullptr);\
	Priority classname::ms_Priority;

#define IMPLEMENT_RTTI_NO_PARENT(classname)\
	Rtti classname::ms_Type(std::string(#classname), nullptr, &classname::FactoryFunction);\
	Priority classname::ms_Priority;

#define IMPLEMENT_RTTI_NO_PARENT_NO_CREATE_FUNCTION(classname)\
	Rtti classname::ms_Type(std::string(#classname), nullptr, nullptr);\
	Priority classname::ms_Priority;
}
