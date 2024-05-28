#include "gepch.h" 
#include "Mesh.h"

#include <glad/glad.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include "imgui/imgui.h"

#include "GEngine/Renderer/Renderer.h"

#include <filesystem>

namespace GEngine {

#define MESH_DEBUG_LOG 1
#if MESH_DEBUG_LOG
#define GE_MESH_LOG(...) GE_CORE_TRACE(__VA_ARGS__)
#else
#define GE_MESH_LOG(...)
#endif

	glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix)
	{
		glm::mat4 result;
		result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
		result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
		result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
		result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
		return result;
	}

	static const uint32_t s_MeshImportFlags =
		aiProcess_CalcTangentSpace |		//预计算切线与副切线  
		aiProcess_Triangulate |             //多边形分解为三角形
		aiProcess_SortByPType |             //不同图元分开到不同网格
		aiProcess_GenNormals |              //生成法线
		aiProcess_GenUVCoords |             //生成纹理坐标
		aiProcess_OptimizeMeshes |          //网格优化
		aiProcess_ValidateDataStructure;    //数据有效校验

	struct LogStream : public Assimp::LogStream
	{
		static void Initialize()
		{
			if (Assimp::DefaultLogger::isNullLogger())
			{
				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
				Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
			}
		}

		virtual void write(const char* message) override
		{
			GE_CORE_ERROR("Assimp error: {0}", message);
		}
	};

	Mesh::Mesh(const std::string& filename)
		: m_FilePath(filename)
	{
		LogStream::Initialize();

		GE_CORE_INFO("Loading mesh: {0}", filename.c_str());
		
		m_Importer = std::make_unique<Assimp::Importer>();

		const aiScene* scene = m_Importer->ReadFile(filename, s_MeshImportFlags);

		if (!scene || !scene->HasMeshes())
			GE_CORE_ERROR("Failed to load mesh file: {0}", filename);

		m_Scene = scene;

		m_MeshShader = Renderer::GetShaderLibrary()->Get("HazelPBR_Static");
		m_BaseMaterial = CreateRef<Material>(m_MeshShader);

		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		m_Submeshes.reserve(scene->mNumMeshes);
		for (size_t m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[m];

			Submesh& submesh = m_Submeshes.emplace_back();
			submesh.BaseVertex = vertexCount;
			submesh.BaseIndex = indexCount;
			submesh.MaterialIndex = mesh->mMaterialIndex;
			submesh.IndexCount = mesh->mNumFaces * 3;
			submesh.MeshName = mesh->mName.C_Str();

			vertexCount += mesh->mNumVertices;
			indexCount += submesh.IndexCount;
			
			//顶点信息
			auto& BoundingBox = submesh.BoundingBox;
			BoundingBox.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
			BoundingBox.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
			for (size_t i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
				vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
				BoundingBox.Min.x = glm::min(vertex.Position.x, BoundingBox.Min.x);
				BoundingBox.Min.y = glm::min(vertex.Position.y, BoundingBox.Min.y);
				BoundingBox.Min.z = glm::min(vertex.Position.z, BoundingBox.Min.z);
				BoundingBox.Max.x = glm::max(vertex.Position.x, BoundingBox.Max.x);
				BoundingBox.Max.y = glm::max(vertex.Position.y, BoundingBox.Max.y);
				BoundingBox.Max.z = glm::max(vertex.Position.z, BoundingBox.Max.z);

				if (mesh->HasTangentsAndBitangents())
				{
					vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
					vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
				}

				if (mesh->HasTextureCoords(0))
					vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

				m_StaticVertices.push_back(vertex);
			}
			
			//顶点索引
			for (size_t i = 0; i < mesh->mNumFaces; i++)
			{
				GE_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
				Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
				m_Indices.push_back(index);
				m_TriangleCache[m].emplace_back(m_StaticVertices[index.V1 + submesh.BaseVertex], m_StaticVertices[index.V2 + submesh.BaseVertex], m_StaticVertices[index.V3 + submesh.BaseVertex]);
			}			
		}

		TraverseNodes(scene->mRootNode);

		if (scene->HasMaterials())
		{
			m_Materials.resize(scene->mNumMaterials);

			for (uint32_t i = 0; i < scene->mNumMaterials; i++)
			{
				auto aiMaterial = scene->mMaterials[i];
				auto aiMaterialName = aiMaterial->GetName();

				auto mi = CreateRef<MaterialInstance>(m_BaseMaterial);
				m_Materials[i] = mi;

				aiColor3D aiColor;
				float shininess, metalness;

				aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
				aiMaterial->Get(AI_MATKEY_SHININESS, shininess);
				aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness);

				float roughness = 1.0f - glm::sqrt(shininess / 100.0f);
				
				mi->Set("u_AlbedoColor", glm::vec3 { aiColor.r, aiColor.g, aiColor.b });
				mi->Set("u_NormalTexToggle", 0.0f);				
				mi->Set("u_Roughness", roughness);
				mi->Set("u_Metalness", metalness);
				mi->Set("u_MetalnessTexToggle", 0.0f);
				
			}
			GE_MESH_LOG("------------------------");
		}

		m_VertexArray = VertexArray::Create();
		
		auto vb = VertexBuffer::Create(m_StaticVertices.data(), m_StaticVertices.size() * sizeof(Vertex));
		vb->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float3, "a_Tangent" },
			{ ShaderDataType::Float3, "a_Binormal" },
			{ ShaderDataType::Float2, "a_TexCoord" },
		});
		m_VertexArray->AddVertexBuffer(vb);
		

		auto ib = IndexBuffer::Create(m_Indices.data(), m_Indices.size() * sizeof(Index));
		m_VertexArray->SetIndexBuffer(ib);
	}

	Mesh::~Mesh()
	{
	}

	static std::string LevelToSpaces(uint32_t level)
	{
		std::string result = "";
		for (uint32_t i = 0; i < level; i++)
			result += "--";
		return result;
	}

	void Mesh::TraverseNodes(aiNode* node, const glm::mat4& parentTransform, uint32_t level)
	{
		//设置transform
		glm::mat4 transform = parentTransform * Mat4FromAssimpMat4(node->mTransformation);
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			uint32_t mesh = node->mMeshes[i];
			auto& submesh = m_Submeshes[mesh];
			submesh.NodeName = node->mName.C_Str();
			submesh.Transform = transform;
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
			TraverseNodes(node->mChildren[i], transform, level + 1);
	}

}