//This class was included within the example project and nothing was changed inside of it.
#include "texture_loader.h"
#include <FreeImagePlus.h>
#include <iostream>


using namespace std;


#pragma region FreeImagePlus texture loader
// This function is used to get our texture file/image which is meant to be a 2d texture.
//the 2 numbers from the image which are width and height get wrapped around our imported model. 
GLuint fiLoadTexture(const char *filename, const TextureProperties& properties) {

	BOOL				fiOkay = FALSE;
	GLuint				newTexture = 0;
	fipImage			I;

	fiOkay = I.load(filename);

	if (!fiOkay) {

		cout << "FreeImagePlus: Cannot open image file.\n";
		return 0;
	}

	if (properties.flipImageY) {

		fiOkay = I.flipVertical();
	}
	
	fiOkay = I.convertTo32Bits();

	if (!fiOkay) {

		cout << "FreeImagePlus: Conversion to 24 bits successful.\n";
		return 0;
	}

	auto w = I.getWidth();
	auto h = I.getHeight();

	BYTE *buffer = I.accessPixels();

	if (!buffer) {

		cout << "FreeImagePlus: Cannot access bitmap data.\n";
		return 0;
	}

	glGenTextures(1, &newTexture);
	glBindTexture(GL_TEXTURE_2D, newTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

	// Setup default texture properties
	if (newTexture) {

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	return newTexture;
}

#pragma endregion

