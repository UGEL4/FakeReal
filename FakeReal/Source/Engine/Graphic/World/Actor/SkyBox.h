#pragma once

#include "../../Object.h"
namespace FakeReal
{
	class MeshNode;
	class FR_ENGINE_API SkyBox : public Object
	{
	public:
		SkyBox();
		~SkyBox();

		void InitRenderResource();

		//void SetWorldTransform(const glm::vec3& pos, const glm::vec3& rotate)
		//void SetView(const glm::mat)
		void BeforeDraw(const class Shader* pShader, const class EditorCameraActor* pCameraActor);

		MeshNode* GetMeshNode() const { return m_pMesh; }
	private:
		MeshNode* m_pMesh;
	};
}
