#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include "../../Core/CoreMarco.h"

namespace FakeReal
{
	class FR_ENGINE_API EnumTrans
	{
	public:
		EnumTrans();
		~EnumTrans();
		static EnumTrans& GetInstance();
		void Add(const char* EnumName, const char* Value);
		void Get(const char* EnumName, std::vector<std::string>& AS);

	private:
		std::unordered_map<std::string, std::vector<std::string>> mEnumSaver;
	};

#define ADD_ENUM(EnumName,Value) \
	EnumTrans::GetInstance().Add(#EnumName,#Value);
#define GET_ENUMARRAY(EnumName,Array) \
	EnumTrans::GetInstance().Get(#EnumName,Array);
}