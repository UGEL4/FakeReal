#pragma once
#include <iostream>
namespace FakeReal
{
#pragma warning(disable:4251) //ȥ��ģ�嵼������ľ���
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef BUILD_ENGINE_DLL
	#define FR_ENGINE_API __declspec(dllexport)
#else
	#define FR_ENGINE_API __declspec(dllimport)
#endif
}