#include "EngineMain.h"
#include <assert.h>
#include <algorithm>
namespace FakeReal {
	std::vector<EngineMain::Element>* EngineMain::ms_pInitialArray = nullptr;
	std::vector<EngineMain::Element>* EngineMain::ms_pTerminateArray = nullptr;
	std::vector<PropertyFunc>* EngineMain::ms_pInitialPropertyArray = nullptr;
	std::vector<Function>* EngineMain::ms_pTerminalPropertyArray = nullptr;

	bool EngineMain::Initialize()
	{
		//收集属性表
		if (ms_pInitialPropertyArray)
		{
			for (size_t i = 0; i < ms_pInitialPropertyArray->size(); i++)
			{
				if (!(*ms_pInitialPropertyArray)[i](nullptr))
				{
					assert(0);
					return false;
				}
			}
		}

		std::sort(ms_pInitialArray->begin(), ms_pInitialArray->end(), Compaier());
		for (size_t i = 0; i < ms_pInitialArray->size(); i++)
		{
			if (!(*((*ms_pInitialArray)[i].mFunc))())
			{
				return false;
			}
		}
		ms_pInitialArray->clear();
		delete ms_pInitialArray;
		ms_pInitialPropertyArray->clear();
		delete ms_pInitialPropertyArray;
		return true;
	}

	bool EngineMain::Terminate()
	{
		if (ms_pTerminateArray)
		{
			std::sort(ms_pTerminateArray->begin(), ms_pTerminateArray->end(), Compaier());
			for (size_t i = ms_pTerminateArray->size() - 1; i >= 0; --i)
			{
				if (!(*((*ms_pTerminateArray)[i].mFunc))())
				{
					return false;
				}
			}
			ms_pTerminateArray->clear();
			delete ms_pTerminateArray;
		}
		if (ms_pTerminalPropertyArray)
		{
			for (size_t i = 0; i < ms_pTerminalPropertyArray->size(); i++)
			{
				if (!(*((*ms_pTerminalPropertyArray)[i]))())
				{
					assert(0);
					return false;
				}
			}
			ms_pTerminalPropertyArray->clear();
			delete ms_pTerminalPropertyArray;
		}
		return true;
	}

	void EngineMain::AddInitialFunction(Function f)
	{
		if (!f) return;
		if (!ms_pInitialArray)
		{
			ms_pInitialArray = new std::vector<Element>;
		}
		Element e;
		e.mFunc = f;
		ms_pInitialArray->emplace_back(e);
	}

	void EngineMain::AddInitialFunction(Function f, Priority* pPriority)
	{
		if (!f) return;
		if (!ms_pInitialArray)
		{
			ms_pInitialArray = new std::vector<Element>;
		}
		Element e;
		e.mFunc = f;
		e.m_pPriority = pPriority;
		ms_pInitialArray->emplace_back(e);
	}

	void EngineMain::AddTerminateFunction(Function f)
	{
		if (!f)
			return;
		if (!ms_pTerminateArray)
		{
			ms_pTerminateArray = new std::vector<Element>;
		}
		Element e;
		e.mFunc = f;
		ms_pTerminateArray->emplace_back(e);

	}

	void EngineMain::AddTerminateFunction(Function f, Priority* pPriority)
	{
		if (!f)
			return;
		if (!ms_pTerminateArray)
		{
			ms_pTerminateArray = new std::vector<Element>;
		}
		Element e;
		e.mFunc = f;
		e.m_pPriority = pPriority;
		ms_pTerminateArray->emplace_back(e);
	}

	void EngineMain::AddInitialPropertyFunction(PropertyFunc f)
	{
		if (!f)
		{
			return;
		}
		if (!ms_pInitialPropertyArray)
		{
			ms_pInitialPropertyArray = new std::vector<PropertyFunc>;
		}
		ms_pInitialPropertyArray->emplace_back(f);
	}

	void EngineMain::AddTerminalPropertyFunction(Function f)
	{
		if (!f)
		{
			return;
		}
		if (!ms_pTerminalPropertyArray)
		{
			ms_pTerminalPropertyArray = new std::vector<Function>;
		}
		ms_pTerminalPropertyArray->emplace_back(f);
	}

}
