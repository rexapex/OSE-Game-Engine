#pragma once

#include "OSE-Core/Types.h"

namespace ose
{
	class TextureLoader;
	class Texture;
	class TextureAtlas;
	struct TextureMetaData;

	class TilemapLoader;
	class Tilemap;

	class MeshLoader;
	class Mesh;

	class Material;

	class ShaderProg;

	class ResourceManager
	{
	public:
		ResourceManager(const std::string & project_path);
		~ResourceManager() noexcept;
		// copying is not allowed
		ResourceManager(ResourceManager &) = delete;
		// moving is allowed
		ResourceManager(ResourceManager &&) noexcept;
		// Assignment is not allowed
		ResourceManager & operator=(ResourceManager &) = delete;
		ResourceManager & operator=(ResourceManager &&) = delete;

		// import a file into the project resources directory
		// sub_dir is a sub directory within the resources directory
		void ImportFile(const std::string & file_path, const std::string & sub_dir = "");

		// imports multiple files into project resources directory
		// sub_dir is a sub directory within the resources directory
		void ImportFiles(const std::vector<std::string> & file_paths, const std::string & sub_dir = "");

		// get the texture from either map
		// given the name of the texture, return the texture object
		Texture const * GetTexture(const std::string name);

		// adds the texture at path to the list of active textures, the texture must be in the project's resources directory
		// path is relative to ProjectPath/Resources
		// if no name is given, the relative path will be used
		// IMPORTANT - can be called from any thread (TODO)
		void AddTexture(const std::string & path, const std::string & name = "");

		// create the GPU memory for an already loaded (added) texture
		// returns an iterator to the next texture in the textures_without_GPU_memory map
		// IMPORANT - can only be called from the thread which contains the render context
		std::map<std::string, uptr<Texture>>::const_iterator CreateTexture(const std::string & tex_name);

		// remove the texture from the textures list and free the texture's resources
		// IMPORANT - can be called from any thread (TODO)
		void RemoveTexture(const std::string & tex_name);

		// free the GPU memory of the texture
		// IMPORANT - can only be called from the thread which contains the render context
		void DestroyTexture(const std::string & tex_name);

		// create all textures which are currently lacking a GPU representation
		// IMPORTANT - can only be called from the thread which contains the render context
		void CreateTextures();

		// loads a meta file for some texture, meta files map properties to values
		void LoadTextureMetaFile(const std::string & abs_path, TextureMetaData & meta_data);

		// Get the tilemap from the resources manager
		// Given the name of the tilemap, return the tilemap object
		Tilemap const * GetTilemap(const std::string & name);

		// Adds the tilemap at path to the list of active tilemaps, the tilemap must be in the project's resources directory
		// Path is relative to ProjectPath/Resources
		// If no name is given, the relative path will be used
		// IMPORTANT - Can be called from any thread (TODO)
		void AddTilemap(const std::string & path, const std::string & name = "");

		// Remove the tilemap from the tilemaps list and free the tilemap's resources
		// IMPORANT - can be called from any thread (TODO)
		void RemoveTilemap(const std::string & name);

		// Get the mesh from the resource manager
		// Given the name of the mesh, return the mesh object
		Mesh const * GetMesh(const std::string & name);

		// Adds the mesh at path to the list of active meshes, the mesh must be in the project's resources directory
		// Path is relative to ProjectPath/Resources
		// If no name is given, the relative path will be used
		// IMPORTANT - Can be called from any thread (TODO)
		void AddMesh(const std::string & path, const std::string & name = "");

		// Remove the mesh from the meshes list and free the mesh's resources
		// IMPORTANT - Can be called from any thread (TODO)
		void RemoveMesh(const std::string & name);

		// Get the material from the resource manager
		// Given the name of the material, return the material object
		Material const * GetMaterial(const std::string & name);

		// Adds the material at path to the list of active materials, the material must be in the project's resources directory
		// Path is relative to ProjectPath/Resources
		// If no name is given, the relative path will be used
		// IMPORTANT - Can be called from any thread (TODO)
		void AddMaterial(const std::string & path, const std::string & name = "");

		// Remove the material from the materials list and free the material's resources
		// IMPORTANT - Can be called from any thread (TODO)
		void RemoveMaterial(const std::string & name);

		// Get the shader program from the resource manager
		// Given the name of the shader program, return the shader program object
		ShaderProg const * GetShaderProg(const std::string & name);

		// Adds the shader program at path to the list of active shader programs, the shader program must be in the project's resources directory
		// Path is relative to ProjectPath/Resources
		// If path starts with OSE then path will be interpreted as the name of a builtin shader graph
		// If no name is given, the relative path will be used
		// IMPORTANT - Can be called from any thread (TODO)
		void AddShaderProg(const std::string & path);

		// Create the GPU memory for an already loaded (added) shader program
		// Returns an iterator to the next shader program in the shader_progs_without_gpu_memory map
		// IMPORANT - can only be called from the thread which contains the render context
		std::map<std::string, uptr<ShaderProg>>::const_iterator CreateShaderProg(const std::string & prog_name);

		// Remove the shader program from the shader programs list and free the shader program's resources
		// IMPORTANT - Can be called from any thread (TODO)
		void RemoveShaderProg(const std::string & name);

		// Free the GPU memory of the shader program
		// IMPORANT - can only be called from the thread which contains the render context
		void DestroyShaderProg(const std::string & prog_name);

		// Create all shader programs which are currently lacking a GPU representation
		// IMPORTANT - can only be called from the thread which contains the render context
		void CreateShaderProgs();

	private:
		// the root path of the currently loaded project
		std::string project_path_;

		// maps texture name to texture object
		// any texture with a representation in GPU memory
		// these textures can be destroyed but not removed
		std::map<std::string, uptr<Texture>> textures_with_Gpu_memory_;

		// maps texture name to texture object
		// any texture with no representation in GPU memory
		// only these textures can be created and removed
		std::map<std::string, uptr<Texture>> textures_without_Gpu_memory_;

		// the TextureLoader object used for loading textures from image files
		uptr<TextureLoader> texture_loader_;

		// Maps tilemap name to tilemap object
		std::map<std::string, uptr<Tilemap>> tilemaps_;

		// The TilemapLoader object used for loading tilemap from files
		uptr<TilemapLoader> tilemap_loader_;

		// Maps mesh name to mesh object
		std::map<std::string, uptr<Mesh>> meshes_;

		// The MeshLoader object used for loading meshes from files
		uptr<MeshLoader> mesh_loader_;

		// Maps material name to material object
		std::map<std::string, uptr<Material>> materials_;

		// Maps shader program name to shader program object
		std::map<std::string, uptr<ShaderProg>> shader_progs_with_gpu_memory_;
		
		// Maps shader program name to shader program object
		std::map<std::string, uptr<ShaderProg>> shader_progs_without_gpu_memory_;

		// Load a property file (similar to an ini file)
		// Returns properties as a map from key to value
		// Used for loading simple resources with no bespoke resource loader
		std::unordered_multimap<std::string, std::string> LoadPropertyFile(const std::string & abs_path);
	};
}
