#pragma once
//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

// Type includes.
#include <string>
#include <memory>
#include <map>

// Builder includes.
#include <JsonNames.h>

// Json includes.
#include <nlohmann/json.hpp>

// File includes.
#include <filesystem>

// Forward declarations.
namespace Graphics { class Model; class Texture2D; class Material; class ShaderProgram; class GraphicsContext; }
namespace Lua { class LuaContext; class LuaScript; }

namespace Content
{
	class JsonContentManager
	{
	public:
		JsonContentManager(const std::string& exeFolderPath, const std::string& contentFolderName, std::shared_ptr<Graphics::GraphicsContext> graphicsContext, std::weak_ptr<Lua::LuaContext> luaContext)
			: rootFolderPath(std::filesystem::path(exeFolderPath).parent_path().append(contentFolderName)), graphicsContext(graphicsContext), luaContext(luaContext) {}
		
		const std::shared_ptr<Graphics::GraphicsContext> GetGraphicsContext() { return graphicsContext.lock(); }

		template<typename T>
		std::shared_ptr<T> Load(const std::string& assetPath)
		{
			std::shared_ptr<T> pointer = nullptr;
			load(assetPath, pointer);
			return pointer;
		}
		
	private:

		std::weak_ptr<Graphics::GraphicsContext> graphicsContext;
		std::weak_ptr<Lua::LuaContext> luaContext;

		std::filesystem::path rootFolderPath;

		std::map<std::string, std::shared_ptr<Graphics::Model>> loadedModels;
		std::map<std::string, std::shared_ptr<Graphics::Texture2D>> loadedTextures;
		std::map<std::string, std::shared_ptr<Graphics::ShaderProgram>> loadedShaders;
		std::map<std::string, std::shared_ptr<Graphics::Material>> loadedMaterials;
		std::map<std::string, int> loadedScriptInstances;
		
		void load(const std::string& assetPath, std::shared_ptr<Graphics::Model>& model);
		void load(const std::string& assetPath, std::shared_ptr<Graphics::Texture2D>& texture);
		void load(const std::string& assetPath, std::shared_ptr<Graphics::Material>& material);
		void load(const std::string& assetPath, std::shared_ptr<Graphics::ShaderProgram>& shader);
		void load(const std::string& assetPath, std::shared_ptr<Lua::LuaScript>& luaScript);

		std::filesystem::path createFullPath(const std::string& assetPath) { return std::filesystem::path(rootFolderPath).append(assetPath).replace_extension(Builders::FileExtension); }

		void loadAssetFile(const std::filesystem::path& filePath, nlohmann::json& outputData);
	};
}