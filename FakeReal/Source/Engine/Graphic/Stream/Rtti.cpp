#include "Rtti.h"
#include "../Object.h"
#include "Property.h"
#include <assert.h>

namespace FakeReal {

	Rtti::Rtti(const std::string& name, Rtti* pBase, CreateObjFunc cof)
		: mName(name), m_pBase(pBase), mCreateFunc(cof)
	{

	}

	bool Rtti::IsSameType(const Rtti& type) const
	{
		return (&type == this);
	}

	bool Rtti::IsDerived(const Rtti& type) const
	{
		const Rtti* pTemp = this;
		while (!pTemp->IsSameType(type))
		{
			if (pTemp->m_pBase)
			{
				pTemp = pTemp->m_pBase;
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	Property* Rtti::GetProperty(size_t uiIndex) const
	{
		if (uiIndex >= mPropertyArray.size())
		{
			return nullptr;
		}
		return mPropertyArray[uiIndex];
	}

	Property* Rtti::GetProperty(std::string& propertyName) const
	{
		for (size_t i = 0; i < mPropertyArray.size(); i++)
		{
			if (mPropertyArray[i]->GetName() == propertyName)
			{
				return mPropertyArray[i];
			}
		}
		return nullptr;
	}

	bool Rtti::AddProperty(Property* pProperty)
	{
		if (pProperty)
		{
			for (size_t i = 0; i < mPropertyArray.size(); i++)
			{
				assert(mPropertyArray[i]->GetName() != pProperty->GetName());
			}
			mPropertyArray.emplace_back(pProperty);
			return true;
		}
		assert(0);
		return false;
	}

	bool Rtti::AddProperty(Rtti& rtti)
	{
		return false;
	}

	void Rtti::ClearProperty()
	{
		mPropertyArray.clear();
	}

}
