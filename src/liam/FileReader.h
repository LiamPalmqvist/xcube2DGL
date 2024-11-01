#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>

using namespace std;
class FileReader
{
public:
	FileReader::FileReader();

	// declaration of the file to be parsed
	ifstream file;
	string line;
	string out;

	// returns a pointer to the file's contents
	string FileReader::ReadFile(string& fileName);
	void FileReader::WriteFile();

	// declaration of the basic shader program
	//static const GLchar* textureVertex;
	//static const GLchar* textureFragment;

};

