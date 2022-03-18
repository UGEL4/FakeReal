#include "Geometry.h"
#include "../Stream/Property.h"
#include "../../Render/Renderer.h"
#include "../../Render/RenderResource/RenderResource.h"
#include "../../Render/Texture/FRTexture.h"
#include "../Resource/Resource.h"
namespace FakeReal {
	Geometry* Geometry::ms_pDefaultRenderCube = nullptr;

	IMPLEMENT_RTTI(Geometry, Spatial);
	IMPLEMENT_INITIAL_BEGIN(Geometry)
	ADD_INITIAL_FUNCTION(InitialDefaultState)
	ADD_TERMINATE_FUNCTION(TerminalDefaultState)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(Geometry, Spatial)
	REGISTER_PROPERTY(m_pMeshData, "Mesh", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	Geometry::Geometry()
	{

	}
	Geometry::~Geometry()
	{
		if (m_pMeshData)
			delete m_pMeshData;
		std::cout << "析构Geometry:" << this << std::endl;
	}

	void Geometry::UpdateNodeAll(float appTime)
	{
		UpdateTransform(appTime);
		mbIsChange = false;
	}

	bool Geometry::InitialDefaultState()
	{
		ms_pDefaultRenderCube = new Geometry;
		if (!ms_pDefaultRenderCube)
			return false;

		LoadDefault();
		return true;
	}

	bool Geometry::TerminalDefaultState()
	{
		//delete ms_pDefaultRenderCube;
		//ms_pDefaultRenderCube = nullptr;
		return true;
	}

	void Geometry::LoadDefault()
	{
		//cube
		{
			//ms_pDefaultRenderCube = new Geometry;
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
			Vertex[8]  = { -w2, h2, -d2 };
			Vertex[9]  = { -w2, h2,  d2 };
			Vertex[10] = {  w2, h2,  d2 };
			Vertex[11] = {  w2, h2, -d2 };
			// 底面(-Y面)
			Vertex[12] = {  w2, -h2, -d2 };
			Vertex[13] = {  w2, -h2,  d2 };
			Vertex[14] = { -w2, -h2,  d2 };
			Vertex[15] = { -w2, -h2, -d2 };
			// 背面(-Z面)
			Vertex[16] = { -w2, -h2, -d2 };
			Vertex[17] = { -w2,  h2, -d2 };
			Vertex[18] = {  w2,  h2, -d2 };
			Vertex[19] = {  w2, -h2, -d2 };
			// 正面(+Z面)
			Vertex[20] = { w2, -h2, d2 };
			Vertex[21] = { w2,  h2, d2 };
			Vertex[22] = { -w2,  h2, d2 };
			Vertex[23] = { -w2, -h2, d2 };

			glm::vec3 NormalArray[24];
			for (int i = 0; i < 4; ++i)
			{
				// 右面(+X面)
				NormalArray[i] = { 1.0f, 0.0f, 0.0f };
				//meshData.vertexVec[i].tangent = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
				//meshData.vertexVec[i].color = color;
				// 左面(-X面)
				NormalArray[i + 4] = { -1.0f, 0.0f, 0.0f };
				//meshData.vertexVec[i + 4].tangent = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
				//meshData.vertexVec[i + 4].color = color;
				// 顶面(+Y面)
				NormalArray[i + 8] = { 0.0f, 1.0f, 0.0f };
				//meshData.vertexVec[i + 8].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
				//meshData.vertexVec[i + 8].color = color;
				// 底面(-Y面)
				NormalArray[i + 12] = { 0.0f, -1.0f, 0.0f };
				//meshData.vertexVec[i + 12].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
				//meshData.vertexVec[i + 12].color = color;
				// 背面(-Z面)
				NormalArray[i + 16] = { 0.0f, 0.0f, -1.0f };
				//meshData.vertexVec[i + 20].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
				//meshData.vertexVec[i + 20].color = color;
				// 正面(+Z面)
				NormalArray[i + 20] = { 0.0f, 0.0f, 1.0f };
				//meshData.vertexVec[i + 16].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
				//meshData.vertexVec[i + 16].color = color;
			}

			glm::vec2 TexCoord[24];
			for (int i = 0; i < 6; ++i)
			{
				TexCoord[i * 4] = { 1.0f, 0.0f };
				TexCoord[i * 4 + 1] = { 1.0f, 1.0f };
				TexCoord[i * 4 + 2] = { 0.0f, 1.0f };
				TexCoord[i * 4 + 3] = { 0.0f, 0.0f };
			}

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

			BufferLayout layout;
			LayoutElement LE;
			LE.m_Type			= ShaderDataType::Float3;
			LE.m_uiSize			= ShaderDataTypeSize(LE.m_Type);
			LE.m_uiOffset		= 0;
			LE.m_Semantics		= VertexFormat::VF_POSITION;
			LE.m_SemanticsIndex = 0;
			LE.m_bNormalize		= false;
			layout.AddElement(LE);
			DataBuffer* pPosBuffer = new DataBuffer;
			pPosBuffer->SetData(&Vertex[0][0], 24, LE.m_Type);

			LE.m_Type			= ShaderDataType::Float3;
			LE.m_uiSize			= ShaderDataTypeSize(LE.m_Type);
			LE.m_uiOffset		= ShaderDataTypeSize(ShaderDataType::Float3);
			LE.m_Semantics		= VertexFormat::VF_NORMAL;
			LE.m_SemanticsIndex = 0;
			LE.m_bNormalize		= false;
			layout.AddElement(LE);
			DataBuffer* pNormalBuffer = new DataBuffer;
			pNormalBuffer->SetData(&NormalArray[0][0], 24, LE.m_Type);

			LE.m_Type			= ShaderDataType::Float2;
			LE.m_uiSize			= ShaderDataTypeSize(LE.m_Type);
			LE.m_uiOffset		= ShaderDataTypeSize(ShaderDataType::Float3) + ShaderDataTypeSize(ShaderDataType::Float3);
			LE.m_Semantics		= VertexFormat::VF_TEXCOORD;
			LE.m_SemanticsIndex = 0;
			LE.m_bNormalize		= false;
			layout.AddElement(LE);
			DataBuffer* pTexBuffer = new DataBuffer;
			pTexBuffer->SetData(&TexCoord[0][0], 24, LE.m_Type);

			/*unsigned int memSize = 24 * layout.GetStride();
			unsigned char* pData = new unsigned char[memSize];
			unsigned char* Temp = pData;
			for (int i = 0; i < 24; i++)
			{
				memcpy_s(Temp, ShaderDataTypeSize(ShaderDataType::Float3), &Vertex[i][0], ShaderDataTypeSize(ShaderDataType::Float3));
				Temp += ShaderDataTypeSize(ShaderDataType::Float3);

				memcpy_s(Temp, ShaderDataTypeSize(ShaderDataType::Float3), &NormalArray[i][0], ShaderDataTypeSize(ShaderDataType::Float3));
				Temp += ShaderDataTypeSize(ShaderDataType::Float3);

				memcpy_s(Temp, ShaderDataTypeSize(ShaderDataType::Float2), &TexCoord[i][0], ShaderDataTypeSize(ShaderDataType::Float2));
				Temp += ShaderDataTypeSize(ShaderDataType::Float2);

			}*/

			//VAO
			VertexArray* pVa = new VertexArray;
			if (Renderer::IsRendererInited())
			{
				ResourceIdentifier* pVAID = Renderer::CreateVertexArrayID();
				pVa->SetResourceIdentifier(pVAID);
			}

			//VBO
			VertexBuffer* pVb = new VertexBuffer;
			pVb->SetLayout(layout);

			Mesh* pMesh = new Mesh();
			pMesh->SetVertexArray(pVa);

			pMesh->SetVertexBuffer(pVb);
			pMesh->SetData(VertexFormat::VF_POSITION, pPosBuffer);
			pMesh->SetData(VertexFormat::VF_NORMAL, pNormalBuffer);
			pMesh->SetData(VertexFormat::VF_TEXCOORD, pTexBuffer);
			if (Renderer::IsRendererInited())
			{
				ResourceIdentifier* pID = Renderer::CreateVertexBufferID(pVb, true);
				pVb->SetResourceIdentifier(pID);
			}

			//IBO
			IndexBuffer* pIb = new IndexBuffer(pIndicesBuffer);
			if (Renderer::IsRendererInited())
			{
				ResourceIdentifier* pIndexID = Renderer::CreateIndexBufferID(pIb);
				pIb->SetResourceIdentifier(pIndexID);
			}
			pMesh->SetIndexBuffer(pIb);

			//TEXTURE
			TextureAllState* pTexState = nullptr;
			LoadImageData data = Texture2D::LoadImage("Resources/pic.jpeg", true);
			if (data.data)
			{
				Texture* pTexture = new Texture2D("Resources/pic.jpeg", data.width, data.height, data.format);
				pTexture->SetData(data.data, data.dataSize);
				SamplerDesc desc;
				SamplerState* pSamplerState = new SamplerState;
				pSamplerState->SetSamplerDesc(desc);
				pTexState = new TextureAllState(pTexture);
				pTexState->SetSamplerState(pSamplerState);
				if (Renderer::IsRendererInited())
				{
					TextureID* id = Renderer::CreateTextureID(pTexture, pSamplerState);
					pTexState->SetTextureID(id);
				}

				TextureAllStateR* pTexStateR = ResourceProxy<TextureAllState>::Create(pTexState);
				pMesh->AddTexture2D(pTexStateR);
			}

			ms_pDefaultRenderCube->m_pMeshData = pMesh;

			//delete[] pData;
		}
	}

}