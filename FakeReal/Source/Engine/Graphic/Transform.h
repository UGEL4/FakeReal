#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../Core/CoreMarco.h"

namespace FakeReal {
	class FR_ENGINE_API Transform
	{
	public:
		Transform() {}
		Transform(const glm::vec3& scale, const glm::vec3& rotation, const glm::vec3& position);
		~Transform() {}

		Transform(const Transform&) = default;
		Transform& operator = (const Transform&) = default;

		Transform(Transform&&) = default;
		Transform& operator = (Transform&&) = default;

	public:
		const glm::vec3& GetScale() const;
		const glm::vec3& GetRotation() const;
		const glm::quat& GetRotationQ() const;
		const glm::vec3& GetPosition() const;
		glm::vec3& GetPosition();
		glm::vec3 GetRightAxis() const;
		glm::vec3 GetUpAxis() const;
		glm::vec3 GetForwardAxis() const;

		glm::mat4 GetLocalToWorldMatrix() const;

		glm::mat4 GetWorldToLocalMatrix() const;

		void SetScale(const glm::vec3& scale);
		void SetScale(float x, float y, float z);

		void SetRotation(const glm::vec3& eulerAnglesInRadian);
		void SetRotation(float  x, float y, float z);
		void SetRotation(const glm::quat& quaternion);
		void SetRotation(float x, float y, float z, float w);

		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z);

		void Rotate(const glm::vec3& eulerAnglesInRadian);
		void RotateAxis(const glm::vec3& axis, float radian);
		void RotateAround(const glm::vec3& point, const glm::vec3& axis, float radian);

		void Translate(const glm::vec3& direction, float val);

		void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3( 0.f, 1.f, 0.f ));
		void LookTo(const glm::vec3& direction, const glm::vec3& up = glm::vec3( 0.f, 1.f, 0.f ));

		void Product(const Transform& t1, const Transform& t2);

		void Inverse(Transform& out) const;

		void SetMatrix(const glm::mat4& matrix);
	private:
		glm::vec3 GetEulerAnglesFromRotationMatrix(const glm::mat4& rotation_mx);

	private:
		glm::vec3 m_Scale = { 1.f, 1.f, 1.f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Position = { 0.f, 0.f, 0.f };
		glm::quat m_RotationQ;
	};
}
