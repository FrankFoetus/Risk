#include "IOManager.h"

#include <fstream>


namespace Bengine {

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
		// create a input filestream
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		// seek to the end of the file
		file.seekg(0, std::ios::end);

		// get file size
		int fileSize = file.tellg();
		// seek back to beginning to read file
		file.seekg(0, std::ios::beg);

		// disregard file header bytes
		fileSize -= file.tellg();

		// make the buffer as big as the file
		buffer.resize(fileSize);
		// file.read expects a char*, so we need to put a pointer 
		// to the address of the first element in the vector
		// need to cast to char because we use unsigned char but file.read expects char*
		file.read((char *)&(buffer[0]), fileSize);
		// optional close call , because filestreams close themselfes
		file.close();

		return true;
	}

}