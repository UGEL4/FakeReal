#pragma once
#include <vector>
namespace FakeReal {
	template <typename RetValueType, typename ... ParamType>
	class Delegate
	{
	public:
		Delegate()
		{
			m_pObj = nullptr;
			mFunc = nullptr;
		}
		~Delegate()
		{

		}

		typedef RetValueType(*NewFunc)(void*, ParamType ...);

	private:
		static Delegate Create(void* pObj, NewFunc pFunc)
		{
			Delegate Temp;
			Temp.m_pObj = pObj;
			Temp.mFunc = pFunc;
			return Temp;
		}

		template <RetValueType(*FuncName)(ParamType...)>
		static RetValueType CommonFunction(void* pObj, ParamType ... param)
		{
			return (FuncName)(param...);
		}

		template <class ClassName, RetValueType(ClassName::*FuncName)(ParamType...)>
		static RetValueType ClassMethod(void* pObj, ParamType ... param)
		{
			ClassName* ptr = (ClassName*)pObj;
			return (ptr->*FuncName)(param...);
		}

		template <class ClassName, RetValueType(ClassName::*FuncName)(ParamType...) const>
		static RetValueType ConstClassMethod(void* pObj, ParamType ... param)
		{
			ClassName* ptr = (ClassName*)pObj;
			return (ptr->*FuncName)(param...);
		}

	public:
		/*Create delegate width a class member function*/
		template <class ClassName, RetValueType(ClassName::*FuncName)(ParamType...)>
		static Delegate FromMethod(ClassName* pObj)
		{
			return Create(pObj, &ClassMethod<ClassName, FuncName>);
		}

		/*Create delegate width a class const member function*/
		template <class ClassName, RetValueType(ClassName::*FuncName)(ParamType...) const>
		static Delegate FromMethodConst(ClassName* pObj)
		{
			return Create(pObj, &ConstClassMethod<ClassName, FuncName>);
		}

		/*Create delegate width a class static function or a common function*/
		template <RetValueType(*FuncName)(ParamType...)>
		static Delegate FromFunction()
		{
			return Create(nullptr, &CommonFunction<FuncName>);
		}

		RetValueType Execute(ParamType ... param) const
		{
			return (*mFunc)(m_pObj, param...);
		}

		RetValueType operator ()(ParamType ... param) const
		{
			return (*mFunc)(m_pObj, param...);
		}

		bool operator ==(const Delegate& right) const
		{
			return (m_pObj == right.m_pObj && mFunc == right.mFunc);
		}

		operator bool() const
		{
			return mFunc != nullptr;
		}

		bool operator !() const
		{
			return !(operator bool());
		}

	private:
		void* m_pObj;
		NewFunc mFunc;
	};

	template <typename RetValueType, typename ... ParamType>
	class DelegateEvent
	{
	public:
		DelegateEvent()
		{
			mDelegateList.clear();
		}
		~DelegateEvent()
		{
			mDelegateList.clear();
		}

		void operator +=(const Delegate<RetValueType, ParamType...>& handler)
		{
			this->Add(handler);
		}

		void Add(const Delegate<RetValueType, ParamType...>& handler)
		{
			if (!Has(handler))
				mDelegateList.emplace_back(handler);
		}

		void operator -=(const Delegate<RetValueType, ParamType...>& handler)
		{
			this->Remove(handler);
		}

		void Remove(const Delegate<RetValueType, ParamType...>& handler)
		{
			for (size_t i = 0; i < mDelegateList.size(); i++)
			{
				if (handler == mDelegateList[i])
				{
					mDelegateList.erase(mDelegateList.begin() + i);
					return;
				}
			}
		}

		bool Has(const Delegate<RetValueType, ParamType...>& handler)
		{
			for (size_t i = 0; i < mDelegateList.size(); i++)
			{
				if (handler == mDelegateList[i])
					return true;
			}
			return false;
		}

		bool IsVaild() const
		{
			return mDelegateList.size() > 0;
		}

		void Reset()
		{
			mDelegateList.clear();
		}

		void Execute(ParamType ... param) const
		{
			for (auto& delegate : mDelegateList)
			{
				delegate.Execute(param...);
			}
		}

		void operator ()(ParamType ... param) const
		{
			for (auto& delegate : mDelegateList)
			{
				delegate.Execute(param...);
			}
		}
	private:
		std::vector<Delegate<RetValueType, ParamType...>> mDelegateList;
	};
}
