#pragma once

namespace ose
{
	enum EVertexDataType
	{
		VDT_POSITIONS = 1,
		VDT_TEX_COORDS = 2,
		VDT_NORMALS = 4,
		VDT_TANGENTS = 8,
		VDT_BITANGENTS = 16
	};

	// MeshSection represents parts of a mesh which are disconnected
	// A Mesh is comprised of 1 or more MeshSections
	struct MeshSection
	{
	public:

		// Add a new face
		inline void AddFace(unsigned int x, unsigned int y, unsigned int z)
		{
			face_indices_.push_back(x);
			face_indices_.push_back(y);
			face_indices_.push_back(z);
		}

		// Getters for the mesh section data
		std::vector<unsigned int> const & GetFaceIndices() const { return face_indices_; }

	private:
		// Array of indices to faces
		// A face is composed of 3 consecutive indices starting at an index which is a multiple of 3
		// TODO - Have an array of uints and ushorts and use the ushorts if possible
		std::vector<unsigned int> face_indices_;
	};

	class Mesh
	{
	public:
		Mesh(std::string const & name, std::string const & path) : name_(name), path_(path) {}
		~Mesh() {}

		// Get the path of the mesh file
		std::string const & GetPath() const { return path_; }

		// Reserve space for vertex arrays
		inline void SetNumVertices(size_t n, int types)
		{
			num_vertices_ = n;
			positions_.resize((types & VDT_POSITIONS) == VDT_POSITIONS ? n*3 : 0);
			tex_coords_.resize((types & VDT_TEX_COORDS) == VDT_TEX_COORDS ? n*2 : 0);
			normals_.resize((types & VDT_NORMALS) == VDT_NORMALS ? n*3 : 0);
			tangents_.resize((types & VDT_TANGENTS) == VDT_TANGENTS ? n*3 : 0);
			bitangents_.resize((types & VDT_BITANGENTS) == VDT_BITANGENTS ? n*3 : 0);
		}

		// Add a new mesh section
		// Returns a reference to the new mesh section
		inline MeshSection & AddMeshSection()
		{
			return sections_.emplace_back();
		}

		// Add a new vertex position
		inline void AddVertexPosition(size_t index, float x, float y, float z)
		{
			positions_[index+0] = x;
			positions_[index+1] = y;
			positions_[index+0] = z;
		}

		// Add a new vertex texture co-ordinate
		inline void AddVertexTexCoord(size_t index, float u, float v)
		{
			tex_coords_[index+0] = u;
			tex_coords_[index+1] = v;
		}

		// Add a new vertex normal
		inline void AddVertexNormal(size_t index, float x, float y, float z)
		{
			normals_[index+0] = x;
			normals_[index+1] = y;
			normals_[index+0] = z;
		}

		// Get the total number of vertices in the mesh
		size_t GetNumVertices() const { return num_vertices_; }

		// Getters for the vertex array data
		std::vector<float> const & GetPositionData() const { return positions_; }
		std::vector<float> const & GetTexCoordData() const { return tex_coords_; }
		std::vector<float> const & GetNormalData() const { return normals_; }
		std::vector<float> const & GetTangentData() const { return tangents_; }
		std::vector<float> const & GetBitangentData() const { return bitangents_; }

		// Get the array of mesh sections which make up the mesh
		std::vector<MeshSection> const & GetSections() const { return sections_; }

	private:
		
		// Name & path of the mesh file
		std::string name_;
		std::string path_;

		// Number of vertices in the mesh
		// Sum of no. vertices in each mesh section
		size_t num_vertices_;

		// Vertex data arrays
		std::vector<float> positions_;
		std::vector<float> tex_coords_;
		std::vector<float> normals_;
		std::vector<float> tangents_;
		std::vector<float> bitangents_;

		// Array of mesh sections
		std::vector<MeshSection> sections_;
	};
}
