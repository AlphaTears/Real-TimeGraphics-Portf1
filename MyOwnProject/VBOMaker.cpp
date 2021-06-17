#include "VBOMaker.h"




//setting  up the number of meshes and allocating data to each one of them.
VBOMaker::VBOMaker(const aiScene* scene) 
{

	numModels = scene->mNumMeshes;

	modelArray = (meshVBO**)malloc(numModels * sizeof(meshVBO*));

	for (int i = 0; i < numModels; i++) 
	{

		modelArray[i] = new meshVBO(scene, i);


	}

}


//risponsible for the model array within the scene which each model has its own number within the scene. 
void VBOMaker::render() {


	for (int i = 0; i < numModels; i++)
	{

		modelArray[i]->render();

	}


}