#pragma once
#include "../../Core/CoreMarco.h"
#include "../../Graphic/Object.h"

namespace FakeReal
{
	class ResourceIdentifier;
	class VertexBuffer;
	class FR_ENGINE_API VertexArray : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		VertexArray();
		virtual ~VertexArray();

		void Bind() const;
		void Unbind() const;
		void SetResourceIdentifier(ResourceIdentifier* pID) { m_pID = pID; }

	private:
		ResourceIdentifier* m_pID;
	};
	FR_TYPE_MARCO(VertexArray)
}