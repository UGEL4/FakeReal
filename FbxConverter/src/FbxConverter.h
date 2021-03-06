#pragma once
#include <fbxsdk.h>
#include <vector>
#include <unordered_map>
#include <Engine\Graphic\Node\Model\StaticMeshNode.h>
#include <Engine\Graphic\Node\Model\SkeletonMeshNode.h>
#include <Engine\Graphic\Node\Model\Skeleton.h>
#include <Engine\Graphic\Node\GeometryNode.h>
#include <Engine\Graphic\Anim\Animation.h>
#include <glm/glm.hpp>

namespace FakeReal {
	struct FbxTextureData
	{
		std::string m_Name;
		std::string m_Path;
		std::string m_RelativePath;
		std::string m_TypeName = "texture_diffuse";
	};

	struct FbxKeyFrameData
	{
		FbxVector4 tran;
		FbxQuaternion rot;
		FbxVector4 scale;
		FbxAMatrix globalPose;
		double keyTime;
	};

	struct FbxAnimData
	{
		std::unordered_map<std::string, std::vector<FbxKeyFrameData>> data;
	};
	class FbxConverter
	{
	public:
		enum //EXPORT TYPE
		{
			ET_STATIC_MESH,
			ET_SKELECTON_MESH,
			ET_ACTION,
			ET_MAX
		};

		FbxConverter(int argc, char** argv);
		~FbxConverter();

		bool ExportFile();
		void InitFbxObjects();
		bool CreateScene();

	private:
		void DestroySdkObjects();
		bool LoadScene(const std::string& file);

	private:
		void ProcessMesh(FbxNode* pNode);
		void GetMesh(FbxNode* pNode);
		void ProcessSkeleton(FbxNode* pNode, Bone* pParentBone = nullptr);
		void GetOffsetMatrix(FbxSkin* pSkin);
		void LoadTexture(FbxMesh* pMesh, std::vector<FbxTextureData>& outList);
		void FbxMatToGlmMat(glm::mat4& out, const FbxAMatrix& in);
		void BoneSkin(FbxSkin* pSkin, std::vector<std::string>& Bones, std::vector<float>& Weights, int ctrlPointIndex);
		void GetAnim(FbxNode* pNode);

		void WriteMeshToFile();
		void WriteSkeletonMeshToFile();
		void WriteAnimToFile();
		void ClearAllInfo();
		void GetTriangleMaterialIndex(FbxMesh* pMesh, int triangleCount, std::vector<int>& TriangleMaterialIndexList);
		void GetTriangleSmGroupIndex(FbxMesh* pMesh, int triangleCount, std::vector<int>& TriangleSmGroupIndexList);
		void ReadVertex(FbxMesh* pMesh, int ctrlPointIndex, glm::vec3& V);
		void ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, glm::vec3& N);
		void ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int triangleIndex, int triangleVertexIndex, int uvIndex, glm::vec2& UV);
		void CreateStaticMesh(int UVNum, bool hasSkin);

	private:
		FbxManager* m_pManager;
		FbxScene* m_pScene;
		std::string m_File;
		std::string m_OutPut;
		std::string m_DefaultName;
		bool m_LoadSceneResult;
		int m_SkeletonCount;
		unsigned int mExportType;

	private:
		FbxAnimData m_Anim;

		MeshNode* m_pMeshNode;
		GeometryNode* m_pGeoNode;
		Skeleton* m_pSkeleton;
		Animation* m_pAnimation;

		std::vector<glm::vec3> mVertexArray;
		std::vector<glm::vec3> mNormalArray;
		std::vector<glm::vec2> mTexCoordArray[2];//????2??UV
		std::vector<unsigned int> mIndexArray;
		std::vector<unsigned int> mVertexSmGroupArray;
		std::vector<std::string> mMeshBoneNameArray;
		std::vector<glm::ivec4> mBoneIndexArray;//????????????????mMeshBoneNameArray??????
		std::vector<glm::vec4> mBoneWeightArray;
	};
}