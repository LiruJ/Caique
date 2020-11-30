#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

// Graphics includes.
#include "Model.h"
#include "Mesh.h"
#include "Material.h"

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
#include <experimental/filesystem>

// LiruGraphics includes.
#include <GraphicsContext.h>
#include <Texture2D.h>
#include <ShaderProgram.h>

namespace Content
{
	class ContentManager
	{
	public:
		ContentManager(const std::string& exeFolderPath, const std::string& contentFolderName, std::shared_ptr<Graphics::GraphicsContext> graphicsContext) 
			: rootFolderPath(std::experimental::filesystem::path(exeFolderPath).parent_path().append(contentFolderName)), graphicsContext(graphicsContext) {}
		
		void LoadModel(const std::string& assetPath, std::shared_ptr<Graphics::Model>& model);
		void LoadTexture(const std::string& assetPath, std::shared_ptr<Graphics::Texture2D>& texture);
		void LoadMaterial(const std::string& assetPath, std::shared_ptr<Graphics::Material>& material);
		void LoadShader(const std::string& assetPath, std::shared_ptr<Graphics::ShaderProgram>& shader);
	private:

		std::shared_ptr<Graphics::GraphicsContext> graphicsContext;

		std::experimental::filesystem::path rootFolderPath;

		std::map<std::string, std::shared_ptr<Graphics::Model>> loadedModels;
		std::map<std::string, std::shared_ptr<Graphics::Texture2D>> loadedTextures;
		std::map<std::string, std::shared_ptr<Graphics::ShaderProgram>> loadedShaders;
		std::map<std::string, std::shared_ptr<Graphics::Material>> loadedMaterials;
		
		std::experimental::filesystem::path createFullPath(const std::string& assetPath) { return std::experimental::filesystem::path(rootFolderPath).append(assetPath).replace_extension(Builders::FileExtension); }

		void loadAssetFile(const std::experimental::filesystem::path& filePath, nlohmann::json& outputData);
	};
}