#include "ContentManager.h"

// Builder includes.
#include <JsonEnums.h>

// Type includes.
#include <vector>

// Stream includes.
#include <iostream>
#include <fstream>

// File includes.
#include <filesystem>

// LiruGraphics includes.
#include <VertexArrayObject.h>
#include <VertexBufferObject.h>

// glm includes.
#include <glm/vec4.hpp>

void Content::ContentManager::LoadModel(const std::string& assetPath, std::shared_ptr<Graphics::Model>& model)
{
	// If the model has already been loaded, use that instead then return.
	std::map<std::string, std::shared_ptr<Graphics::Model>>::iterator foundItem = loadedModels.find(assetPath);
	if (foundItem != loadedModels.end())
	{
		model = (*foundItem).second;
		return;
	}

	// Create the full file path from the given asset path.
	std::experimental::filesystem::path fullFilePath = createFullPath(assetPath);

	// Load the model file.
	nlohmann::json modelData;
	loadAssetFile(fullFilePath, modelData);

	// Get the meshes from the model data.
	nlohmann::json& meshesData = modelData[Builders::MeshDataName];

	// Create a vector to hold the meshes.
	std::vector<Graphics::Mesh> meshes;

	// Go over each mesh and load it.
	for (int meshIndex = 0; meshIndex < meshesData.size(); meshIndex++)
	{
		// Create the vertex array object.
		std::shared_ptr<Graphics::VertexArrayObject> vertexArrayObject = std::make_shared<Graphics::VertexArrayObject>();

		// Create new vertex buffer objects.
		std::shared_ptr<Graphics::VertexBufferObject> positions = std::make_shared<Graphics::VertexBufferObject>(3);
		std::shared_ptr<Graphics::VertexBufferObject> uvs = std::make_shared<Graphics::VertexBufferObject>(2);

		// Get the positions data and add it to the vertex array object.
		nlohmann::json& positionData = meshesData[meshIndex][Builders::PositionsDataName];
		std::vector<float> rawPositionData = positionData[Builders::DataDataName];
		positions->Add(rawPositionData);
		vertexArrayObject->AddVertexBufferObject(positionData[Builders::MeshAttributeDataName], positionData[Builders::AttributeIndexName], positions);

		// If UV data exists, get the UV data and add it to the vertex array object.
		if (meshesData[meshIndex].contains(Builders::UVsDataName))
		{
			nlohmann::json& uvsData = meshesData[meshIndex][Builders::UVsDataName];
			std::vector<float> rawUVData = uvsData[Builders::DataDataName];
			uvs->Add(rawUVData);
			vertexArrayObject->AddVertexBufferObject(uvsData[Builders::MeshAttributeDataName], uvsData[Builders::AttributeIndexName], uvs);
		}

		// Get the indices and add them to a vector.
		std::vector<unsigned int> indices = meshesData[meshIndex][Builders::IndicesDataName];

		// Load the material as an asset.
		std::shared_ptr<Graphics::Material> material;
		LoadMaterial(meshesData[meshIndex][Builders::MeshMaterialDataName], material);

		// Push the mesh into the vector of meshes used for the model.
		meshes.push_back(Graphics::Mesh(graphicsContext, indices, vertexArrayObject, material));
	}

	// Set the model.
	model = std::make_shared<Graphics::Model>(meshes);

	// Add the model to the map of loaded models.
	loadedModels.emplace(assetPath, model);
}

void Content::ContentManager::LoadTexture(const std::string& assetPath, std::shared_ptr<Graphics::Texture2D>& texture)
{
	// If the texture has already been loaded, use that instead then return.
	std::map<std::string, std::shared_ptr<Graphics::Texture2D>>::iterator foundItem = loadedTextures.find(assetPath);
	if (foundItem != loadedTextures.end())
	{
		texture = (*foundItem).second;
		return;
	}

	// Create the full file path from the given asset path.
	std::experimental::filesystem::path fullFilePath = createFullPath(assetPath);

	// Load the texture file.
	nlohmann::json textureData;
	loadAssetFile(fullFilePath, textureData);

	// Load the image data.
	Builders::FilterMode filterMode = textureData[Builders::FilterDataName];
	int width = textureData[Builders::WidthDataName];
	int height = textureData[Builders::HeightDataName];
	std::vector<unsigned char> pixelData = textureData[Builders::DataDataName];

	// Load the texture2D.
	std::shared_ptr<Graphics::Texture2D> textureShared = std::make_shared<Graphics::Texture2D>(filterMode == Builders::FilterMode::Closest, width, height, &pixelData.at(0));

	// Add the texture to the loaded textures.
	loadedTextures.emplace(assetPath, textureShared);
	texture = textureShared;
	
}

void Content::ContentManager::LoadMaterial(const std::string& assetPath, std::shared_ptr<Graphics::Material>& material)
{
	// If the material has already been loaded, use that instead then return.
	std::map<std::string, std::shared_ptr<Graphics::Material>>::iterator foundItem = loadedMaterials.find(assetPath);
	if (foundItem != loadedMaterials.end())
	{
		material = (*foundItem).second;
		return;
	}

	// Create the full file path from the given asset path.
	std::experimental::filesystem::path fullFilePath = createFullPath(assetPath);

	// Load the material file.
	nlohmann::json materialData;
	loadAssetFile(fullFilePath, materialData);

	// Load the shader as an asset.
	std::shared_ptr<Graphics::ShaderProgram> shader;
	LoadShader(materialData[Builders::MaterialShaderName], shader);

	// Load the textures.
	std::map<char, std::weak_ptr<Graphics::Texture2D>> textures;
	for (char textureIndex = 0; textureIndex < materialData[Builders::MaterialTexturesName].size(); textureIndex++)
	{
		// Yeah I know this line looks weird, but essentially it's telling the json parser to read as an int, then turns that int into a char.
		char index = (char)(int)materialData[Builders::MaterialTexturesName][textureIndex][0];

		// Load the texture as an asset.
		std::shared_ptr<Graphics::Texture2D> textureShared;
		LoadTexture(materialData[Builders::MaterialTexturesName][textureIndex][1], textureShared);
		std::weak_ptr<Graphics::Texture2D> texture = textureShared;

		// Add to the map.
		textures.emplace(index, texture);
	}

	// Load the colours.
	std::map<char, glm::vec4> colours;
	for (char colourIndex = 0; colourIndex < materialData[Builders::MaterialColoursName].size(); colourIndex++)
	{
		char index = (char)(int)materialData[Builders::MaterialColoursName][colourIndex][0];

		// Load the colour.
		glm::vec4 colour(0, 0, 0, 0);
		colour.r = materialData[Builders::MaterialColoursName][colourIndex][1][0];
		colour.g = materialData[Builders::MaterialColoursName][colourIndex][1][1];
		colour.b = materialData[Builders::MaterialColoursName][colourIndex][1][2];
		colour.a = materialData[Builders::MaterialColoursName][colourIndex][1][3];

		// Add to the map.
		colours.emplace(index, colour);
	}

	// Set the material.
	material = std::make_shared<Graphics::Material>(shader, textures, colours);

	// Add the material to the map of loaded materials.
	loadedMaterials.emplace(assetPath, material);
}

void Content::ContentManager::LoadShader(const std::string& assetPath, std::shared_ptr<Graphics::ShaderProgram>& shader)
{
	// If the shader has already been loaded, use that instead then return.
	std::map<std::string, std::shared_ptr<Graphics::ShaderProgram>>::iterator foundItem = loadedShaders.find(assetPath);
	if (foundItem != loadedShaders.end())
	{
		shader = (*foundItem).second;
		return;
	}

	// Create the full file path from the given asset path.
	std::experimental::filesystem::path fullFilePath = createFullPath(assetPath);

	// Load the shader file.
	nlohmann::json shaderData;
	loadAssetFile(fullFilePath, shaderData);

	// Load each piece of data into a string.
	std::string vertexSource = shaderData[Builders::VertexSourceName];
	std::string fragmentSource = shaderData[Builders::FragmentSourceName];
	std::string texturesName = (shaderData.contains(Builders::TexturesDataName) && !shaderData[Builders::TexturesDataName].is_null()) ? shaderData[Builders::TexturesDataName] : "";
	std::string coloursName = (shaderData.contains(Builders::ColoursDataName) && !shaderData[Builders::ColoursDataName].is_null()) ? shaderData[Builders::ColoursDataName] : "";
	std::string modelMatrixName = shaderData[Builders::ModelMatrixName];
	std::string projectionMatrixName = shaderData[Builders::ProjectionMatrixName];
	std::string viewMatrixName = shaderData[Builders::ViewMatrixName];
	
	// Set the shader.
	shader = std::make_shared<Graphics::ShaderProgram>(vertexSource, fragmentSource, texturesName, coloursName, modelMatrixName, projectionMatrixName, viewMatrixName);
	
	// Link the shader.
	shader->Link();

	// Add the shader to the map of loaded shaders.
	loadedShaders.emplace(assetPath, shader);
}

void Content::ContentManager::loadAssetFile(const std::experimental::filesystem::path& filePath, nlohmann::json& outputData)
{
	// Load the file.
	std::ifstream fileStream(filePath);
	try { fileStream >> outputData; }
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "Ensure file is using UTF-8 encoding, else strange errors will occur." << std::endl;
	}
}
