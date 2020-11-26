#include <iostream>
#include <string>
#include <cstdio>

using std::string;
using std::cout;
using std::endl;

class BMPLoader {
public:
	BMPLoader(const string& bmpPath): mBmpPath(bmpPath) {
		FILE* file = fopen(mBmpPath.c_str(), "rb");
		if (!file) {
			cout << "Fail to load image: " << mBmpPath << endl;
			return;
		}
		if (fread(header, 1, 54, file) != 54) {
			cout << "Not a standard bmp file: " << mBmpPath << endl;
			return;
		}
		if (header[0] != 'B' || header[1] != 'M') {
			cout << "Not a standard bmp file: " << mBmpPath << endl;
			return;
		}
		// Read ints from the byte array
		dataPos = *(int*)&(header[0x0A]);
		imageSize = *(int*)&(header[0x22]);
		width = *(int*)&(header[0x12]);
		height = *(int*)&(header[0x16]);
		// Some BMP files are misformatted, guess missing information
		// 3 : one byte for each Red, Green and Blue component
		if (imageSize == 0)    imageSize = width * height * 3;
		// The BMP header is done that way
		if (dataPos == 0)      dataPos = 54;
		// Create a buffer
		data = new unsigned char[imageSize];
		// Read the actual data from the file into the buffer
		fread(data, 1, imageSize, file);
		//Everything is in memory now, the file can be closed
		fclose(file);
	}
	unsigned int GetWidth() const {
		return width;
	}
	unsigned int GetHeight() const {
		return height;
	}
	unsigned char* GetData() const {
		return data;
	}
private:
	string mBmpPath;
	// Data read from the header of the BMP file
	// Each BMP file begins by a 54-bytes header
	unsigned char header[54];
	// Position in the file where the actual data begins
	unsigned int dataPos;
	unsigned int width, height;
	// imageSize = width*height*3
	unsigned int imageSize;
	// Actual RGB data
	unsigned char* data;
};