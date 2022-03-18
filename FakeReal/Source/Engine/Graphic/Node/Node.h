#pragma once

#include "../Spatial.h"
#include "../Transform.h"
namespace FakeReal {
	class FR_ENGINE_API Node : public Spatial
	{
		DECLARE_RTTI
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		Node();
		virtual ~Node() = 0;

		virtual void UpdateNodeAll(float appTime);
		virtual void AddChild(Spatial* pNode);
		virtual void DeleteChild(Spatial* pNode);
		virtual void DeleteAllChild();
		virtual void RemoveChild(Spatial* pNode);
		virtual void RemoveAllChild();
		Spatial* GetChild(unsigned int uiIndex) const;
		std::vector<Spatial*>& GetChildList();
		unsigned int GetChildNum() const;

	protected:
		std::vector<Spatial*> m_pChildList;
	};
	FR_TYPE_MARCO(Node)
}
