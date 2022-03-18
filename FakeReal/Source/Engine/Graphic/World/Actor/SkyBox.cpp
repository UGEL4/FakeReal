#include "SkyBox.h"
#include "../../Stream/Property.h"
#include "../../Node/MeshNode.h"
#include "../../Node/Geometry.h"
#include "../../../Render/Renderer.h"
#include "../../../Render/RenderResource/RenderResource.h"
#include "../../../Render/Texture/TextureCubeMap.h"
#include "../../../Render/Shader/Shader.h"
#include "../Actor/EditorCameraActor.h"

namespace FakeReal
{

	SkyBox::SkyBox()
	{
		m_pMesh = new MeshNode;
		Geometry* pGeo = new Geometry;
		m_pMesh->AddChild(pGeo);

		glm::vec3 Vertex[24];
		float w2 = 2 / 2, h2 = 2 / 2, d2 = 2 / 2;
		// 右面(+X面)
		Vertex[0] = { w2, -h2, -d2 };
		Vertex[1] = { w2,  h2, -d2 };
		Vertex[2] = { w2,  h2,  d2 };
		Vertex[3] = { w2, -h2,  d2 };
		// 左面(-X面)
		Vertex[4] = { -w2, -h2,  d2 };
		Vertex[5] = { -w2,  h2,  d2 };
		Vertex[6] = { -w2,  h2, -d2 };
		Vertex[7] = { -w2, -h2, -d2 };
		// 顶面(+Y面)
		Vertex[8] = { -w2, h2, -d2 };
		Vertex[9] = { -w2, h2,  d2 };
		Vertex[10] = { w2, h2,  d2 };
		Vertex[11] = { w2, h2, -d2 };
		// 底面(-Y面)
		Vertex[12] = { w2, -h2, -d2 };
		Vertex[13] = { w2, -h2,  d2 };
		Vertex[14] = { -w2, -h2,  d2 };
		Vertex[15] = { -w2, -h2, -d2 };
		// 背面(-Z面)
		Vertex[16] = { -w2, -h2, -d2 };
		Vertex[17] = { -w2,  h2, -d2 };
		Vertex[18] = { w2,  h2, -d2 };
		Vertex[19] = { w2, -h2, -d2 };
		// 正面(+Z面)
		Vertex[20] = { w2, -h2, d2 };
		Vertex[21] = { w2,  h2, d2 };
		Vertex[22] = { -w2,  h2, d2 };
		Vertex[23] = { -w2, -h2, d2 };

		BufferLayout layout;
		LayoutElement LE;
		LE.m_Type = ShaderDataType::Float3;
		LE.m_uiSize = ShaderDataTypeSize(LE.m_Type);
		LE.m_uiOffset = 0;
		LE.m_Semantics = VertexFormat::VF_POSITION;
		LE.m_SemanticsIndex = 0;
		LE.m_bNormalize = false;
		layout.AddElement(LE);
		DataBuffer* pPosBuffer = new DataBuffer;
		pPosBuffer->SetData(&Vertex[0][0], 24, LE.m_Type);

		unsigned int IndexArray[] = {
			0, 1, 2, 2, 3, 0,		// 右面(+X面)
			4, 5, 6, 6, 7, 4,		// 左面(-X面)
			8, 9, 10, 10, 11, 8,	// 顶面(+Y面)
			12, 13, 14, 14, 15, 12,	// 底面(-Y面)
			16, 17, 18, 18, 19, 16, // 背面(+Z面)
			20, 21, 22, 22, 23, 20	// 正面(-Z面)
		};
		DataBuffer* pIndicesBuffer = new DataBuffer;
		pIndicesBuffer->SetData(IndexArray, 36, ShaderDataType::UInt);

		IndexBuffer* pIb = new IndexBuffer(pIndicesBuffer);

		VertexBuffer* pVb = new VertexBuffer;
		pVb->SetLayout(layout);

		Mesh* pMesh = new Mesh();
		pMesh->SetVertexBuffer(pVb);
		pMesh->SetIndexBuffer(pIb);
		pMesh->SetData(VertexFormat::VF_POSITION, pPosBuffer);

		VertexArray* pVa = new VertexArray;
		pMesh->SetVertexArray(pVa);

		pGeo->SetMeshData(pMesh);

		TextureAllState* pTexState = nullptr;
		std::string files[6] = {
			"Resources/SkyBox/right.jpg",
			"Resources/SkyBox/left.jpg",
			"Resources/SkyBox/top.jpg",
			"Resources/SkyBox/bottom.jpg",
			"Resources/SkyBox/front.jpg",
			"Resources/SkyBox/back.jpg"
		};
		Texture2D* pTextures[6];
		for (int i = 0; i < 6; i++)
		{
			LoadImageData data = Texture2D::LoadImage(files[i], false);
			if (data.data)
			{
				pTextures[i] = new Texture2D(files[i], data.width, data.height, data.format);
				pTextures[i]->SetData(data.data, data.dataSize);
			}
		}
		TextureCubeMap* pCubeMap = new TextureCubeMap(pTextures);
		SamplerDesc desc;
		SamplerState* pSamplerState = new SamplerState;
		pSamplerState->SetSamplerDesc(desc);

		pTexState = new TextureAllState(pCubeMap);
		pTexState->SetSamplerState(pSamplerState);

		TextureAllStateR* pTexStateR = ResourceProxy<TextureAllState>::Create(pTexState);
		pMesh->AddTexture2D(pTexStateR);
	}

	SkyBox::~SkyBox()
	{
		if (m_pMesh)
			delete m_pMesh;
	}

	void SkyBox::InitRenderResource()
	{
		if (m_pMesh)
		{
			if (Renderer::IsRendererInited())
			{
				Geometry* pGeo = (Geometry*)m_pMesh->GetChild(0);
				ResourceIdentifier* pVAID = Renderer::CreateVertexArrayID();
				pGeo->GetMeshData()->GetVertexArray()->SetResourceIdentifier(pVAID);

				ResourceIdentifier* pID = Renderer::CreateVertexBufferID(pGeo->GetMeshData()->GetVertexBuffer(), true);
				pGeo->GetMeshData()->GetVertexBuffer()->SetResourceIdentifier(pID);

				ResourceIdentifier* pIndexID = Renderer::CreateIndexBufferID(pGeo->GetMeshData()->GetIndexBuffer());
				pGeo->GetMeshData()->GetIndexBuffer()->SetResourceIdentifier(pIndexID);

				TextureAllState* pTexState = pGeo->GetMeshData()->GetTextureList()[0]->GetResource();
				TextureID* id = Renderer::CreateTextureID(pTexState->GetTexture(), pTexState->GetSamplerState());
				pTexState->SetTextureID(id);
			}
		}
	}

	void SkyBox::BeforeDraw(const class Shader* pShader, const class EditorCameraActor* pCameraActor)
	{
		glm::mat4 view = pCameraActor->GetViewMatrix();
		pShader->Bind();
		pShader->SetMatrix4("view", &view[0][0]);
		pShader->SetMatrix4("proj", &pCameraActor->GetProjMatrix()[0][0]);

		Geometry* pGeo = (Geometry*)m_pMesh->GetChild(0);
		pGeo->GetMeshData()->GetTextureList()[0]->GetResource()->BindTexture();
	}

}