#pragma once

#include "Stream/StreamUtil.h"
#include <unordered_map>
#include "../Core/CoreMarco.h"

namespace FakeReal {
	class Stream;
	class Object;
	//不支持指针序列化，不支持postload 和 postclone
	//类成员如果是 FRType 是支持的
	class FR_ENGINE_API CustomArchiveObject
	{
	public:
		CustomArchiveObject();
		virtual ~CustomArchiveObject();
		virtual void Archive(Stream& Stream) = 0;
		virtual void CopyFrom(CustomArchiveObject* pObj, std::unordered_map<Object*, Object*>& CloneMap) = 0;
	};
	CUSTOMTYPE_MARCO(CustomArchiveObject)
}
