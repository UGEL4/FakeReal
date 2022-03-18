#pragma once

#include "../../Object.h"
#include "../../../Core/CoreMarco.h"

namespace FakeReal {
	class Node;
	class FR_ENGINE_API Scene : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		Scene();
		virtual ~Scene();

		virtual void Update(float appTime);

	public:
		void AddComponent(Node* pComponent);
		void RemoveComponent(Node* pComponent);
		void RemoveAllComponent();

	protected:
		std::vector<Node*> mNodeArray;//保存实体的组件的根节点
	};
	FR_TYPE_MARCO(Scene)
}