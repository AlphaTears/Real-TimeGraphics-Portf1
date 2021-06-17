#pragma once
#include <glew/glew.h>
#include <CoreStructures/GUObject.h>
#include <CoreStructures/GUMatrix4.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include "sceneVBOs.h"
#include "src/shader_setup.h"



class meshVBO: public CoreStructures::GUObject
{

private:

	GLuint meshVAO;
	GLuint meshPosBuffer;
	GLuint meshTexCoordBuffer;
	GLuint meshNormalBuffer;
	GLuint MeshFaceVBO;
	



	GLuint numfaces;

public:

	meshVBO(const aiScene* scene, int meshIndex);

	void render();




};

