#pragma once
#include <iostream>
#include <fstream>
#include <string>

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
	string FileReader::ReadFile(string &fileName);
	void FileReader::WriteFile();
};

