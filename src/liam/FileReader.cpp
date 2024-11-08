#include "FileReader.h"

FileReader::FileReader() {
	// empty constructor
}

void FileReader::ReadFile(string& fileName, string* output) {
    output->clear();
    output->append(textureFragment);
	file = ifstream(fileName);

	if (file.is_open()) {
		while (getline(file, line)) {
			line = line.append("\n");
			//cout << line;
			//out.append(line);
            output->append(line);
		}
		file.close();
	}
	else {
		cout << "unable to open file" << endl;
	}
    //cout << shaderCode;
	//return out;
    return;
}

void FileReader::WriteFile() {
    ofstream myFile;
    myFile.open("Example.txt");
    myFile << "Writing to a file\n";
    myFile.close();
    return;
}