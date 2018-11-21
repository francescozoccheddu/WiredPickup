#pragma once

#include <string>
#include <vector>

namespace Storage
{

	std::vector<char> LoadBinaryFile (const std::string& filename);

	std::string LoadTextFile (const std::string& filename);

}