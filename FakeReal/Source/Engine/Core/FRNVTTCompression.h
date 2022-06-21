#pragma once

#include "CoreMarco.h"
#include <nvtt/nvtt.h>
#include <vector>
#include <iostream>
namespace FakeReal
{
	struct FRNVTTOutputHandler : public nvtt::OutputHandler
	{
		FRNVTTOutputHandler():m_TotalSzie(0){}
		~FRNVTTOutputHandler() {}
		// Indicate the start of a new compressed image that's part of the final texture.
		virtual void beginImage(int size, int width, int height, int depth, int face, int miplevel)
		{

		}

		// Output data. Compressed data is output as soon as it's generated to minimize memory allocations.
		virtual bool writeData(const void * data, int size)
		{
			std::vector<unsigned char> Temp;
			if (m_CompressData.size())
			{
				Temp.resize(m_CompressData.size());
			}
			else
			{
				Temp.resize(size);
			}
			std::copy(m_CompressData.begin(), m_CompressData.end(), Temp.begin());

			m_CompressData.resize(size + m_TotalSzie);
			memcpy_s(&m_CompressData[0], Temp.size(), &Temp[0], Temp.size());
			memcpy_s(&m_CompressData[0] + m_TotalSzie, size, data, size);
			m_TotalSzie += size;

			return true;
		}
;

		// Indicate the end of the compressed image. (New in NVTT 2.1)
		virtual void endImage()
		{

		}

		std::vector<unsigned char> m_CompressData;
		int m_TotalSzie;
	};

	struct FRNVTTErrorHandler : public nvtt::ErrorHandler
	{
		FRNVTTErrorHandler() : bSuccess(true){}
		~FRNVTTErrorHandler() {}
		// Called to signal an error.
		virtual void error(nvtt::Error e)
		{
			std::cout << "NVTTCompress Error: " << nvtt::errorString(e) << std::endl;
			bSuccess = false;
		}

		bool bSuccess;
	};

	class FR_ENGINE_API FRNVTTCompression
	{
	public:
		enum CompressFormat
		{
			CF_BC1,
			CF_BC1A,
			CF_BC2,
			CF_BC3,
			CF_BC3N,
			CF_BC4,
			CF_BC4S,
			CF_BC5,
			CF_BC5S,
			CF_BC6U,
			CF_BC6S,
			CF_BC7,
			CF_MAX
		};

		FRNVTTCompression() {}
		~FRNVTTCompression() {}

		void Setup(CompressFormat format, bool forceUseCpu = false);
		void Compress(const std::string& file);

		bool IsSuccess() const { return m_ErrorHandler.bSuccess; }
		unsigned char* GetCompressData() { return &m_OutputHandler.m_CompressData[0]; }
		size_t GetDataSize() const { return m_OutputHandler.m_CompressData.size(); }

	private:
		static nvtt::Format ms_FotmatToNVTTFormat[CF_MAX];
		nvtt::Context m_Context;
		nvtt::Surface m_Image;
		nvtt::CompressionOptions m_CompressOptions;
		nvtt::OutputOptions m_OutputOptions;
		FRNVTTOutputHandler m_OutputHandler;
		FRNVTTErrorHandler m_ErrorHandler;
	};
}
