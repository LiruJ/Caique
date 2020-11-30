#include "FileReaders.h"

// Stream includes.
#include <sstream>

void Utilities::LoadFileIntoString(const std::string& fileName, std::string& output)
{
	// Load the file into a stream.
	std::ifstream fileStream(fileName);

	// Load the file into a string.
	LoadFileIntoString(fileStream, output);
}

void Utilities::LoadFileIntoString(const std::ifstream& fileStream, std::string& output)
{
	// Throw an error if the file does not exist.
	if (!DoesFileExist(fileStream))
		throw std::exception("File does not exist.");

	// Ensure the output string is clear.
	output.clear();

	// Load the file into the output string.
	std::stringstream stringStream;
	stringStream << fileStream.rdbuf();

	// Set the output to the value in the buffer.
	output = stringStream.str();
}

bool Utilities::DoesFileExist(const std::string& fileName)
{
	// Open a stream with the given filename.
	std::ifstream fileStream(fileName);

	// Return if the file exists.
	return DoesFileExist(fileStream);
}

bool Utilities::DoesFileExist(const std::ifstream& fileStream)
{
	// Return if the file exists.
	return fileStream.good();
}
