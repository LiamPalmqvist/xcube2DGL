#include "FileReader.h"

FileReader::FileReader() {
	
}

string FileReader::ReadFile(string& fileName) {
	file = ifstream(fileName);

	if (file.is_open()) {
		while (getline(file, line)) {
			line = line.append("\n");
			cout << line;
			out.append(line);
		}
		file.close();
	}
	else {
		cout << "unable to open file" << endl;
	}
	return out;
}

void FileReader::WriteFile() {
	ofstream myFile;
	myFile.open("Example.txt");
	myFile << "Writing to a file\n";
	myFile.close();
	return;
}