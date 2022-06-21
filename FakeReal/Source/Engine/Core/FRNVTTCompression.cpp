#include "FRNVTTCompression.h"

namespace FakeReal
{
	nvtt::Format FRNVTTCompression::ms_FotmatToNVTTFormat[CF_MAX] =
	{
		nvtt::Format_BC1,
		nvtt::Format_BC1a,
		nvtt::Format_BC2,
		nvtt::Format_BC3,
		nvtt::Format_BC3n,
		nvtt::Format_BC4,
		nvtt::Format_BC4S,
		nvtt::Format_BC5,
		nvtt::Format_BC5S,
		nvtt::Format_BC6S,
		nvtt::Format_BC6U,
		nvtt::Format_BC7
	};

	void FRNVTTCompression::Setup(CompressFormat format, bool forceUseCpu)
	{
		if (format < CF_BC1 || format >= CF_MAX)
		{
			std::cout << "Error Compress Format, will use default format : CompressFormat::CF_BC1 !" << std::endl;

			m_CompressOptions.setFormat(ms_FotmatToNVTTFormat[CF_BC1]);
		}
		else
		{
			m_CompressOptions.setFormat(ms_FotmatToNVTTFormat[format]);
		}

		if (format == CF_BC6S || format == CF_BC6U || format == CF_BC7)
		{
			m_OutputOptions.setContainer(nvtt::Container_DDS10);
		}
		else
		{
			m_OutputOptions.setContainer(nvtt::Container_DDS);
		}

		m_Context.enableCudaAcceleration(forceUseCpu);
	}

	void FRNVTTCompression::Compress(const std::string& file)
	{
		m_Image.load(file.c_str());

		m_OutputOptions.setOutputHandler(&m_OutputHandler);
		m_OutputOptions.setErrorHandler(&m_ErrorHandler);


		m_Context.outputHeader(m_Image, 1, m_CompressOptions, m_OutputOptions);
		m_Context.compress(m_Image, 0, 0, m_CompressOptions, m_OutputOptions);
	}

}
