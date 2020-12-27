#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

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

namespace Content
{
	class ContentManager
	{
	public:
		ContentManager(const std::string& exeFolderPath, const std::string& contentFolderName, std::shared_ptr<Graphics::GraphicsContext> graphicsContext) 
			: rootFolderPath(std::filesystem::path(exeFolderPath).parent_path().append(contentFolderName)), graphicsContext(graphicsContext) {}
		
		const std::shared_ptr<Graphics::GraphicsContext> GetGraphicsContext() { return graphicsContext.lock(); }

		void LoadModel(const std::string& assetPath, std::shared_ptr<Graphics::Model>& model);
		void LoadTexture(const std::string& assetPath, std::shared_ptr<Graphics::Texture2D>& texture);
		void LoadMaterial(const std::string& assetPath, std::shared_ptr<Graphics::Material>& material);
		void LoadShader(const std::string& assetPath, std::shared_ptr<Graphics::ShaderProgram>& shader);
	private:

		std::weak_ptr<Graphics::GraphicsContext> graphicsContext;

		std::filesystem::path rootFolderPath;

		std::map<std::string, std::shared_ptr<Graphics::Model>> loadedModels;
		std::map<std::string, std::shared_ptr<Graphics::Texture2D>> loadedTextures;
		std::map<std::string, std::shared_ptr<Graphics::ShaderProgram>> loadedShaders;
		std::map<std::string, std::shared_ptr<Graphics::Material>> loadedMaterials;
		
		std::filesystem::path createFullPath(const std::string& assetPath) { return std::filesystem::path(rootFolderPath).append(assetPath).replace_extension(Builders::FileExtension); }

		void loadAssetFile(const std::filesystem::path& filePath, nlohmann::json& outputData);
	};
}