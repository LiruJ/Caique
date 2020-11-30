#pragma once

// Type includes.
#include <string>

// Stream includes.
#include <fstream>

namespace Utilities
{
	void LoadFileIntoString(const std::string& fileName, std::string& output);

	void LoadFileIntoString(const std::ifstream& fileStream, std::string& output);

	bool DoesFileExist(const std::string& fileName);

	bool DoesFileExist(const std::ifstream& fileStream);
}