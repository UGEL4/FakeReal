#include "Resource.h"

namespace FakeReal {
	std::string Resource::ms_FileSuffix[] = {
		"texture",
		"skeletal_model",
		"static_model",
		"animation",
		"actor",
		"material",
		""
	};

	Resource::Resource()
	{

	}

	Resource::~Resource()
	{
		std::cout << "Îö¹¹Resource:" << this << std::endl;
	}

	std::string& Resource::GetFileSuffix(unsigned int uiIndex)
	{
		if (uiIndex < RT_TEXTURE || uiIndex >= RT_MAX)
		{
			return ms_FileSuffix[RT_MAX];
		}
		return ms_FileSuffix[uiIndex];
	}

	ResourceProxyBase::ResourceProxyBase()
	{

	}

	ResourceProxyBase::~ResourceProxyBase()
	{
		std::cout << "Îö¹¹ResourceProxyBase:" << this << std::endl;
	}

}
