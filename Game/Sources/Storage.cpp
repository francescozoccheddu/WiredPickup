#include <Game/Utils/Storage.hpp>

#include <Game/Utils/Exceptions.hpp>
#include <string>
#include <fstream>
#include <sstream>

std::vector<char> Storage::LoadBinaryFile (const std::string& _filename)
{
	std::ifstream file { _filename, std::ios::binary | std::ios::in };
	GAME_ASSERT_MSG (file.is_open (), "File not opened");
	std::vector<char> vec { std::istreambuf_iterator<char> (file), std::istreambuf_iterator<char> () };
	file.close ();
	return vec;
}

std::string Storage::LoadTextFile (const std::string& _filename)
{
	std::ifstream file { _filename };
	GAME_ASSERT_MSG (file.is_open (), "File not opened");
	std::stringstream buffer;
	buffer << file.rdbuf ();
	return buffer.str ();
}