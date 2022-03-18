#include "EnumTrans.h"

namespace FakeReal
{

	EnumTrans::EnumTrans()
	{

	}

	EnumTrans::~EnumTrans()
	{
		std::cout << "Îö¹¹EnumTrans:" << this << std::endl;
	}

	EnumTrans& EnumTrans::GetInstance()
	{
		static EnumTrans instance;
		return instance;
	}

	void EnumTrans::Add(const char* EnumName, const char* Value)
	{
		std::unordered_map<std::string, std::vector<std::string>>::iterator itr = mEnumSaver.find(EnumName);
		if (itr == mEnumSaver.end())
		{
			std::vector<std::string> enumVal;
			enumVal.emplace_back(Value);
			mEnumSaver.emplace(EnumName, enumVal);
			return;
		}
		std::vector<std::string>& enumValue = itr->second;
		enumValue.emplace_back(Value);
	}

	void EnumTrans::Get(const char* EnumName, std::vector<std::string>& AS)
	{
		std::unordered_map<std::string, std::vector<std::string>>::iterator itr = mEnumSaver.find(EnumName);
		if (itr == mEnumSaver.end())
		{
			return;
		}
		AS = itr->second;
	}

}