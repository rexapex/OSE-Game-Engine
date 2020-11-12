#include "pch.h"
#include "MeshLoaderASSIMP.h"

#include "OSE-Core/Resources/Mesh/Mesh.h"

using namespace Assimp;

namespace ose::resources
{
	MeshLoaderASSIMP::MeshLoaderASSIMP(const std::string & project_path) : MeshLoader(project_path)
	{
	
	}

	MeshLoaderASSIMP::~MeshLoaderASSIMP()
	{
	
	}

	// Loads the mesh and sets the data in mesh*
	// Path is absolute and is guaranteed to exist
	void MeshLoaderASSIMP::LoadMesh(std::string const & path, Mesh * mesh)
	{
		// Create an ASSIMP importer
		Importer importer;

		// TODO - Get load settings from mesh meta data
		aiScene const * scene = importer.ReadFile(path.c_str(),
			aiProcess_CalcTangentSpace       | 
			aiProcess_Triangulate            |
			aiProcess_JoinIdenticalVertices  |
			aiProcess_SortByPType);

		// Check the import was successful
		if(!scene)
		{
			LOG_ERROR("Failed to load mesh", path, "-", importer.GetErrorString());
			return;
		}

		// Process the scene and write data to the mesh object
		ProcessScene(scene, mesh);
	}

	// Free the resources used by the mesh object
	void MeshLoaderASSIMP::FreeMesh(Mesh * mesh)
	{
		// NOTE - None of the data stored in Mesh is owned by ASSIMP, so don't need to free anything here
	}

	// Process an ASSIMP scene
	void MeshLoaderASSIMP::ProcessScene(aiScene const * scene, Mesh * mesh)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ASSIMP scene contains an array of meshes
		// In OSE terms, an aiScene is equivalent to an OSE Mesh and an aiMesh is equivalent to an OSE MeshSection
		// From here on, comments and OSE variables names will use the OSE terminology
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Calculate the total number of vertices in the mesh
		size_t total_num_vertices { 0 };
		for(unsigned int m = 0; m < scene->mNumMeshes; ++m)
		{
			total_num_vertices += scene->mMeshes[m]->mNumVertices;
		}

		// Reserve space for vertex data s.t. vectors don't have wasted space
		mesh->SetNumVertices(total_num_vertices, VDT_POSITIONS | VDT_TEX_COORDS | VDT_NORMALS | VDT_TANGENTS);

		// Process the mesh sections
		unsigned int mesh_section_offset { 0 };
		for(unsigned int m = 0; m < scene->mNumMeshes; ++m)
		{
			aiMesh const * ai_mesh_section { scene->mMeshes[m] };
			MeshSection & ose_mesh_section { mesh->AddMeshSection() };

			// Process the faces of the mesh section
			for(unsigned int f = 0; f < ai_mesh_section->mNumFaces; ++f)
			{
				aiFace const & face { ai_mesh_section->mFaces[f] };

				if(face.mNumIndices != 3)
				{
					// NOTE - Should never get here since mesh is loaded with aiProcess_Triangulate
					// TODO - Could detect which meshes are already triangulated and remove aiProcess_Triangulate from those
					LOG_ERROR("Failed to process mesh", mesh->GetPath(), "- only supports meshes composed of triangles");
					return;
				}

				// Add the face to the mesh section
				// Add the mesh section offset so the index is shifted to the correct set of vertices
				ose_mesh_section.AddFace(
					face.mIndices[0] + mesh_section_offset,
					face.mIndices[1] + mesh_section_offset,
					face.mIndices[2] + mesh_section_offset);
			}

			// If the mesh section contains positions, copy them into the positions array
			if(ai_mesh_section->HasPositions())
			{
				// TODO - Speed this up with bulk copy
				for(unsigned int v = 0; v < ai_mesh_section->mNumVertices; ++v)
				{
					mesh->AddVertexPosition((mesh_section_offset + v) * 3,
						ai_mesh_section->mVertices[v].x,
						ai_mesh_section->mVertices[v].y,
						ai_mesh_section->mVertices[v].z);
				}
			}

			// If the mesh section contains normals, copy them into the normals array
			if(ai_mesh_section->HasNormals())
			{
				// TODO - Speed this up with bulk copy
				for(unsigned int v = 0; v < ai_mesh_section->mNumVertices; ++v)
				{
					mesh->AddVertexNormal((mesh_section_offset + v) * 3,
						ai_mesh_section->mNormals[v].x,
						ai_mesh_section->mNormals[v].y,
						ai_mesh_section->mNormals[v].z);
				}
			}

			// TODO - Support multiple texture co-ordinates per mesh section
			if(ai_mesh_section->HasTextureCoords(0))
			{
				// TODO - Speed this up with bulk copy
				for(unsigned int v = 0; v < ai_mesh_section->mNumVertices; ++v)
				{
					mesh->AddVertexTexCoord((mesh_section_offset + v) * 2,
						ai_mesh_section->mTextureCoords[0][v].x,
						ai_mesh_section->mTextureCoords[0][v].y);
				}
			}

			// If the mesh section contains tangent vectors, copy them into the tangents array
			if(ai_mesh_section->HasTangentsAndBitangents())
			{
				// TODO - Speed this up with bulk copy
				for(unsigned int v = 0; v < ai_mesh_section->mNumVertices; ++v)
				{
					mesh->AddVertexTangent((mesh_section_offset + v) * 3,
						ai_mesh_section->mTangents[v].x,
						ai_mesh_section->mTangents[v].y,
						ai_mesh_section->mTangents[v].z);
				}
			}

			// Increment the mesh section offset by the no. of vertices in the previously processed mesh section
			mesh_section_offset += ai_mesh_section->mNumVertices;
		}
	}
}
