#ifndef TEXTURE_H
#define TEXTURE_H

// load a .bmp file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

// load a .dds file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

#endif 
