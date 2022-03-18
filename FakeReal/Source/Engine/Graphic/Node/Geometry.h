#pragma once
/************************************************************************/
/* 形状类，根据不同的材质来分类                                         */
/************************************************************************/

#include "../Spatial.h"
#include "Mesh/Mesh.h"
namespace FakeReal {
	class FR_ENGINE_API Geometry : public Spatial
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		Geometry();
		virtual ~Geometry();
	protected:
		virtual void UpdateNodeAll(float appTime) override;

	public:
		void SetMeshData(Mesh* pData) { m_pMeshData = pData; }
		Mesh* GetMeshData() const { return m_pMeshData; }

		static bool InitialDefaultState();
		static bool TerminalDefaultState();

		static Geometry* GetDefaultRenderCube()
		{
			/*if (!ms_pDefaultRenderCube)
				LoadDefault();*/
			return ms_pDefaultRenderCube;
		}

		const VertexArray* GetVertexArray() const { return m_pMeshData->GetVertexArray(); }
		unsigned int GetIndexCount() const { return m_pMeshData->GetIndexCount(); }
	private:
		static void LoadDefault();
	protected:
		Mesh* m_pMeshData;
		static Geometry* ms_pDefaultRenderCube;

	};
	FR_TYPE_MARCO(Geometry)
}