
//This class was included within the example project and nothing was changed inside of it.
#include "aiWrapper.h"
#include <glew/glew.h>
#include <iostream>

using namespace std;
using namespace CoreStructures;

// Create an instance of the Importer class
static Assimp::Importer				importer;

//loading the model into our scene by reading the file used within the string of it.
const aiScene *aiImportModel(const std::string& filename, unsigned int flags)
{
	const aiScene* scene = importer.ReadFile(filename, flags);

	if (!scene)
		cout << "Sorry, " << filename << " not found!" << endl;
	else
		cout << filename << " loaded okay!" << endl;

	return scene;
}


void aiReportScene(const aiScene* scene) {

	auto numMeshes = scene->mNumMeshes;
	auto M = scene->mMeshes;

	for (unsigned int i = 0; i < numMeshes; ++i) {

		cout << "Mesh " << i << ":" << endl;

		auto mptr = M[i];

		cout << "num vertices = " << M[i]->mNumVertices << endl;
		cout << "num faces = " << M[i]->mNumFaces << endl;
	}
}

//a renderer used for loading a single module into the scene. multiple modules will break the renderer.
void aiRender(const aiScene* scene, GUPivotCamera *mainCamera) {

	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf((const float*)mainCamera->projectionTransform().M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf((const float*)mainCamera->viewTransform().M);

	auto numMeshes = scene->mNumMeshes;
	auto M = scene->mMeshes;

	glColor4f(255, 255, 255,0.5f);

	glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < numMeshes; ++i) {

		aiVector3D *texCoordArray = M[i]->mTextureCoords[0]; // Get texture coord channel 0

															 // Process each face
		for (unsigned int j = 0; j < M[i]->mNumFaces; ++j) {

			// Process face indices
			for (unsigned int k = 0; k < M[i]->mFaces[j].mNumIndices; ++k) {

				auto vi = M[i]->mFaces[j].mIndices[k];
				glTexCoord2f(texCoordArray[vi].x, texCoordArray[vi].y);
				glVertex3f(M[i]->mVertices[vi].x, M[i]->mVertices[vi].y, M[i]->mVertices[vi].z);
			}
		}
	}

	glEnd();
}