#pragma once
#include <string>
namespace FakeReal
{
	//native pointer
	template<typename T> struct TIsNativePointerType { enum { Value = false }; };
	template<typename T> struct TIsNativePointerType<T*> { enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<const T*> { enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<const T* const> { enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<T* volatile> { enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<T* const volatile> { enum { Value = true }; };

	//Object ¿‡¿‡–Õ
	template<typename T> struct TIsFRType { enum { Value = false }; };
	//Object pointer
	template<typename T> struct TIsFRPointerType { enum { Value = false }; };
	template<typename T> struct TIsFRSmartPointerType { enum { Value = false }; };
#define FR_TYPE_MARCO(ClassName) \
	template<> struct TIsFRType<ClassName>										    { enum { Value = true }; }; \
	template<> struct TIsFRPointerType<ClassName*>									{ enum { Value = true }; }; \
	template<> struct TIsFRSmartPointerType<std::shared_ptr<ClassName>>			{ enum { Value = true }; }; \
	template<> struct TIsFRSmartPointerType<const std::shared_ptr<ClassName>>	    { enum { Value = true }; };

	template<typename T> struct TIsFREnumType { enum { Value = false }; };
#define FR_ENUMTYPE_MARCO(ClassName) \
	template<>	struct TIsFREnumType<ClassName> { enum { Value = true }; }; 

	// string
	template<typename T> struct TIsFRStringType { enum { Value = false }; };
	template<>	struct TIsFRStringType<std::string> { enum { Value = true }; };


	//custom type
	template<typename T> struct TIsCustomType { enum { Value = false }; };
	template<typename T> struct TIsCustomPointerType { enum { Value = false }; };
#define CUSTOMTYPE_MARCO(ClassName) \
	template<>	struct TIsCustomType<ClassName>	{ enum { Value = true }; }; \
	template<>	struct TIsCustomPointerType<ClassName *>	{ enum { Value = true }; };

	//Resource
	template<class T>
	struct TIsResourceType { enum { Value = false }; };

	template<class T>
	class ResourceProxy;
	template<class T> struct TIsResourceProxyType { enum { Value = false }; };
	template<class T> struct TIsResourceProxyPointerType { enum { Value = false }; };

	template<class T> struct TIsResourceProxyType<ResourceProxy<T>> { enum { Value = true }; };
	template<class T> struct TIsResourceProxyPointerType<ResourceProxy<T>*> { enum { Value = true }; };

	class Animation;
	//template<> struct TIsResourceType<Animation> { enum { Value = true }; };
	template<> struct TIsResourceType<Animation*> { enum { Value = true }; };
}