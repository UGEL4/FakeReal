#pragma once

/*
	进入main函数之前，这个类会完成收集所有的类静态成员变量，并在引擎初始化时初始化收集到的静态变量。
*/

#include "../Graphic/Priority.h"
#include "CoreMarco.h"
namespace FakeReal {
	class Rtti;
	typedef bool(*Function)();
	typedef bool(*PropertyFunc)(Rtti* pRtti);
	class FR_ENGINE_API EngineMain
	{
	public:
		static bool Initialize();
		static bool Terminate();
		static void AddInitialFunction(Function f);
		static void AddInitialFunction(Function f, Priority* pPriority);
		static void AddTerminateFunction(Function f);
		static void AddTerminateFunction(Function f, Priority* pPriority);
		static void AddInitialPropertyFunction(PropertyFunc f);
		static void AddTerminalPropertyFunction(Function f);

		struct Element
		{
			Element()
			{
				mFunc = nullptr;
				m_pPriority = nullptr;
			}

			bool operator < (const Element& e) const
			{
				static Priority pr1;
				static Priority pr2;

				Priority* p1 = nullptr;
				Priority* p2 = nullptr;

				if (m_pPriority)
					p1 = m_pPriority;
				else
					p1 = &pr1;

				if (e.m_pPriority)
					p2 = e.m_pPriority;
				else
					p2 = &pr2;

				return (*p1) < (*p2);
			}

			bool operator > (const Element& e) const
			{
				static Priority pr1;
				static Priority pr2;

				Priority* p1 = nullptr;
				Priority* p2 = nullptr;

				if (m_pPriority)
					p1 = m_pPriority;
				else
					p1 = &pr1;

				if (e.m_pPriority)
					p2 = e.m_pPriority;
				else
					p2 = &pr2;

				return (*p1) > (*p2);
			}

			bool operator == (const Element& e) const
			{
				static Priority pr1;
				static Priority pr2;

				Priority* p1 = nullptr;
				Priority* p2 = nullptr;

				if (m_pPriority)
					p1 = m_pPriority;
				else
					p1 = &pr1;

				if (e.m_pPriority)
					p2 = e.m_pPriority;
				else
					p2 = &pr2;

				return (*p1) == (*p2);
			}

			bool operator <= (const Element& e) const
			{
				static Priority pr1;
				static Priority pr2;

				Priority* p1 = nullptr;
				Priority* p2 = nullptr;

				if (m_pPriority)
					p1 = m_pPriority;
				else
					p1 = &pr1;

				if (e.m_pPriority)
					p2 = e.m_pPriority;
				else
					p2 = &pr2;

				return (*p1) <= (*p2);
			}

			Function mFunc;
			Priority* m_pPriority;
		};

		class Compaier
		{
		public:
			inline bool operator()(const Element& e1, const Element& e2)
			{
				static Priority pr1;
				static Priority pr2;

				Priority* p1 = nullptr;
				Priority* p2 = nullptr;

				if (e1.m_pPriority)
					p1 = e1.m_pPriority;
				else
					p1 = &pr1;

				if (e2.m_pPriority)
					p2 = e2.m_pPriority;
				else
					p2 = &pr2;

				return (*p1) < (*p2);
			}
		};

	private:
		static std::vector<Element>* ms_pInitialArray;
		static std::vector<Element>* ms_pTerminateArray;
		static std::vector<PropertyFunc>* ms_pInitialPropertyArray;
		static std::vector<Function>* ms_pTerminalPropertyArray;
	};

	//宏
	/****************************************************************************************************/
#define DECLARE_INITIAL\
	public:\
	static bool RegisterMainFactory();\
	static bool InitialClassFactory();\
	static Object* FactoryFunction();\
	static bool ms_bRegisterMainFactory;\
	static bool InitialProperty(Rtti* pRtti);\
	static bool TerminalProperty();

#define IMPLEMENT_INITIAL_BEGIN(classname)\
	static bool gs_bRegisterFactory_##classname = classname::RegisterMainFactory();\
	bool classname::ms_bRegisterMainFactory = false;\
	bool classname::InitialClassFactory()\
	{\
		ms_ClassFactoryMap.emplace(ms_Type.GetName(), FactoryFunction);\
		return true;\
	}\
	Object* classname::FactoryFunction()\
	{\
		return new classname;\
	}\
	bool classname::RegisterMainFactory()\
	{\
		if (!ms_bRegisterMainFactory)\
		{
#define IMPLEMENT_INITIAL_END\
			EngineMain::AddInitialFunction(InitialClassFactory);\
			EngineMain::AddInitialPropertyFunction(InitialProperty);\
			EngineMain::AddTerminalPropertyFunction(TerminalProperty);\
			ms_bRegisterMainFactory = true;\
		}\
		return ms_bRegisterMainFactory;\
	}
	/****************************************************************************************************/
#define DECLARE_INITIAL_NO_CLASS_FACTORY\
	public:\
	static bool RegisterMainFactory();\
	static bool ms_bRegisterMainFactory;\
	static bool InitialProperty(Rtti* pRtti);\
	static bool TerminalProperty();

#define IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(classname)\
	static bool gs_bRegisterFactory_##classname = classname::RegisterMainFactory();\
	bool classname::ms_bRegisterMainFactory = false;\
	bool classname::RegisterMainFactory()\
	{\
		if (!ms_bRegisterMainFactory)\
		{
#define IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END\
			EngineMain::AddInitialPropertyFunction(InitialProperty);\
			EngineMain::AddTerminalPropertyFunction(TerminalProperty);\
			ms_bRegisterMainFactory = true;\
		}\
		return ms_bRegisterMainFactory;\
	}
	/****************************************************************************************************/
#define ADD_INITIAL_FUNCTION(function_name)\
	EngineMain::AddInitialFunction(function_name);

#define ADD_INITIAL_FUNCTION_WITH_PRIORITY(function_name)\
	EngineMain::AddInitialFunction(function_name, &ms_Priority);
	/****************************************************************************************************/
#define ADD_TERMINATE_FUNCTION(function_name)\
	EngineMain::AddTerminateFunction(function_name);

#define ADD_TERMINATE_FUNCTION_WITH_PRIORITY(function_name)\
	EngineMain::AddTerminateFunction(function_name, &ms_Priority);
	
/****************************************************************************************************/
#define DECLARE_INITIAL_ONLY \
public: \
		static bool RegisterMainFactory(); \
public: \
		static bool ms_bRegisterMainFactory;

/****************************************************************************************************/
#define IMPLEMENT_INITIAL_ONLY_BEGIN(classname) \
	static bool gs_bStreamRegistered_##classname= classname::RegisterMainFactory (); \
	bool classname::ms_bRegisterMainFactory = false; \
	bool classname::RegisterMainFactory() \
	{ \
		if (!ms_bRegisterMainFactory) \
		{  

/********************************************************************************/
#define IMPLEMENT_INITIAL_ONLY_END \
			ms_bRegisterMainFactory = true; \
		} \
		return ms_bRegisterMainFactory; \
	}
}
