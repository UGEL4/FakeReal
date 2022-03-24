#pragma once
#include "../../Core/CoreMarco.h"
#include "../Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace FakeReal
{
	class FR_ENGINE_API AnimAtom
	{
	public:
		AnimAtom();
		~AnimAtom();

		void GetMatrix(glm::mat4& outMatrix) const;
		void Interpolation(const AnimAtom& atom1, AnimAtom& atom2, float t);
		void Identity();
		void FromTransform(const Transform& transform);
		void FromMatrix(const glm::mat4& matrix);

		glm::vec3 mScale;
		glm::vec3 mPosition;
		glm::quat mRotation;
	};
}
