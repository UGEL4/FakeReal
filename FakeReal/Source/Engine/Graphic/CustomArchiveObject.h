#pragma once

#include "Stream/StreamUtil.h"
#include <unordered_map>
#include "../Core/CoreMarco.h"

namespace FakeReal {
	class Stream;
	class Object;
	//��֧��ָ�����л�����֧��postload �� postclone
	//���Ա����� FRType ��֧�ֵ�
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
