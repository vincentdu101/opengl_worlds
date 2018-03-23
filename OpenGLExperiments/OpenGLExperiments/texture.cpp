#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint loadBMP_custom(const char * imagepath) {

	printf("Reading image %s\n", imagepath);

	// data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;

	// actual rgb data
	unsigned char * data;

	// open the file 
	FILE * file = fopen(imagepath, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory? Don't forget to read the FAQ!\n", imagepath);
		getchar();
		return 0;
	}

	// read the header, the 54 first byes

	// if less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		fclose(file);
		return 0;
	}

	// a bmp files always begins with bm
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		fclose(file);
		return 0;
	}

	// make sure this is a 24 bpp file
	if (*(int*) & (header[0x1E]) != 0) { printf("Not a correct BMP file\n"); fclose(file); return 0; }
	if (*(int*) & (header[0x1E]) != 24) { printf("Not a correct BMP file\n"); fclose(file); return 0; }

	// read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// some bmp files are misformatted, guess missing information
	// one byte for each red, green, and blue component
	if (imageSize == 0) imageSize = width * height * 3;
	
	// bmp header is done that way
	if (dataPos == 0) dataPos = 54;

	// create a buffer
	data = new unsigned char[imageSize];

	// read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// everything is in memory now, the file can be closed
	fclose(file);

	// create one opengl texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// bind the newly created texture: all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// give the image to opengl
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// opengl has now copied the data, free our own version
	delete[] data;

	// nice trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	// return the textureID
	return textureID;

}

// Equivalent to "DXT1" in ASCII
#define FOURCC_DXT1 0x31545844

// Equivalent to "DXT3" in ASCII
#define FOURCC_DXT3 0x33545844

// Equivalent to "DXT5" in ASCII
#define FOURCC_DXT5 0x35545844

GLuint loadDDS(const char * imagepath) {
	
	unsigned char header[124];

	FILE *fp;

	// try to open the file
	fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory? Don't forget to read the FAQ! \n", imagepath);
		getchar();
		return;
	}

	// verify the type of file
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	// get the surface desc
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char * buffer;
	unsigned int bufsize;

	// how big is it going to be including all mipmaps?
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;

	switch (fourCC) {
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// create one opengl texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// bind the newly created texture: all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// load the mipmaps
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;
	}

	free(buffer);

	return textureID;
}