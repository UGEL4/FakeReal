#pragma once
#include "ModelMeshNode.h"

namespace FakeReal {
	class FR_ENGINE_API StaticMeshNode : public ModelMeshNode
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		StaticMeshNode();
		~StaticMeshNode();

		virtual unsigned int GetResourceType() const override { return RT_STATIC_MODEL; }
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static const StaticMeshNode* GetDefault()
		{ 
			return m_pDefault; 
		}
	private:
		static StaticMeshNode* m_pDefault;
	};
	FR_TYPE_MARCO(StaticMeshNode)
	DECLARE_Proxy(StaticMeshNode)
}