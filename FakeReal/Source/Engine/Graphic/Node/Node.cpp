#include "Node.h"
#include "../Stream/Property.h"
namespace FakeReal {
	IMPLEMENT_RTTI_NO_CREATE_FUNCTION(Node, Spatial);
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(Node)
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END;
	BEGIN_ADD_PROPERTY(Node, Spatial)
	REGISTER_PROPERTY(m_pChildList, "Children", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	Node::Node()
	{
		m_pChildList.clear();
	}

	Node::~Node()
	{
		for (auto& p : m_pChildList)
		{
			if (p)
				delete p;
		}
		m_pChildList.clear();
		std::cout << "析构Node:" << this << std::endl;
	}

	void Node::UpdateNodeAll(float appTime)
	{
		//更新世界位置
		UpdateTransform(appTime);
		//更新子节点
		for (Spatial* p : m_pChildList)
		{
			if (p)
				p->UpdateNodeAll(appTime);
		}
		mbIsChange = false;
	}

	void Node::AddChild(Spatial* pNode)
	{
		if (!pNode)
		{
			return;
		}
		size_t num = m_pChildList.size();
		for (size_t i = 0; i < num; i++)
		{
			if (pNode == m_pChildList[i])
			{
				return;
			}
		}
		pNode->SetParent(this);
		m_pChildList.emplace_back(pNode);
	}

	void Node::DeleteChild(Spatial* pNode)
	{
		if (!pNode)
		{
			return;
		}
		size_t num = m_pChildList.size();
		for (size_t i = 0; i < num; i++)
		{
			if (pNode == m_pChildList[i])
			{
				pNode->SetParent(nullptr);
				m_pChildList.erase(m_pChildList.begin() + i);
				delete pNode;
				return;
			}
		}
	}

	void Node::DeleteAllChild()
	{
		size_t num = m_pChildList.size();
		for (size_t i = 0; i < num; i++)
		{
			if (m_pChildList[i])
			{
				m_pChildList[i]->SetParent(nullptr);
				delete m_pChildList[i];
			}
		}
		m_pChildList.clear();
	}

	void Node::RemoveChild(Spatial* pNode)
	{
		if (!pNode)
		{
			return;
		}
		size_t num = m_pChildList.size();
		for (size_t i = 0; i < num; i++)
		{
			if (pNode == m_pChildList[i])
			{
				pNode->SetParent(nullptr);
				m_pChildList.erase(m_pChildList.begin() + i);
				return;
			}
		}
	}

	void Node::RemoveAllChild()
	{
		size_t num = m_pChildList.size();
		for (size_t i = 0; i < num; i++)
		{
			m_pChildList[i]->SetParent(nullptr);
		}
		m_pChildList.clear();
	}

	Spatial* Node::GetChild(unsigned int uiIndex) const
	{
		if (uiIndex >= m_pChildList.size())
		{
			return nullptr;
		}
		return m_pChildList[uiIndex];
	}

	std::vector<Spatial*>& Node::GetChildList()
	{
		return m_pChildList;
	}

	unsigned int Node::GetChildNum() const
	{
		return m_pChildList.size();
	}
}
