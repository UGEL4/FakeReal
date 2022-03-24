#include "AnimAtom.h"

namespace FakeReal
{
	AnimAtom::AnimAtom()
	{
		Identity();
	}

	AnimAtom::~AnimAtom()
	{

	}

	void AnimAtom::GetMatrix(glm::mat4& outMatrix) const
	{
		glm::mat4 rotate = glm::mat4_cast(mRotation);
		glm::mat4 scale(1.0f);
		scale = glm::scale(scale, mScale);
		glm::mat4 trans(1.0f);
		trans = glm::translate(trans, mPosition);
		outMatrix = trans * rotate * scale;
	}

	void AnimAtom::Interpolation(const AnimAtom& atom1, AnimAtom& atom2, float t)
	{
		while (t < 0.0f)
			t += 1.0f;

		while (t > 1.0f)
			t -= 1.0f;
		mScale = atom1.mScale + (atom2.mScale - atom1.mScale) * t;
		mPosition = atom1.mPosition + (atom2.mPosition - atom1.mPosition) * t;
		mRotation = glm::slerp(atom1.mRotation, atom2.mRotation, t);
	}

	void AnimAtom::Identity()
	{
		mScale = { 1.0f, 1.0f, 1.0f };
		mPosition = { 0.0f, 0.0f, 0.0f };
		mRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	void AnimAtom::FromTransform(const Transform& transform)
	{
		mScale = transform.GetScale();
		mPosition = transform.GetPosition();
		mRotation = transform.GetRotationQ();
	}

	void AnimAtom::FromMatrix(const glm::mat4& matrix)
	{

	}

}
