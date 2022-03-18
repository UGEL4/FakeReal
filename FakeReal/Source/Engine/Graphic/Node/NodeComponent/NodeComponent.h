#pragma once
/************************************************************************/
/* 用于管理SocketNode                                                   */
/************************************************************************/
#include "../Node.h"

namespace FakeReal {
	class FR_ENGINE_API NodeComponent : public Node
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		NodeComponent();
		virtual ~NodeComponent();

		virtual void UpdateNodeAll(float appTime);
		virtual bool IsNeedDraw() const
		{
			return false;
		}
		template<class T>
		static T* CreateComponent();
	};
	FR_TYPE_MARCO(NodeComponent)

	template<class T>
	T* NodeComponent::CreateComponent()
	{
		if (T::ms_Type.IsDerived(NodeComponent::ms_Type))
		{
			T * Component = (T *)Object::GetInstance<T>();
			Component->PostCreate();
			return Component;
		}
		else
		{
			return nullptr;
		}
	}
}