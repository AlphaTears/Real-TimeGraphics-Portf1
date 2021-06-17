#include "meshVBO.h"



meshVBO::meshVBO(const aiScene* scene, int meshIndex) 
{
	//setting up the VAO and binding the principle axes of the object, position, normals and colour/texture
	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);


	//setup VBO for position attributes of our objects
	glGenBuffers(1, &meshPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, meshPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mVertices, GL_STATIC_DRAW);
	

	//setting a pointer in a vertix array for GL to use to get vertixes.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

	//getting the normals of the object using VBO 
	glGenBuffers(1, &meshNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, meshNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mNormals, GL_STATIC_DRAW);

	//attributing a pointer for vertex to use in this case its for colour
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

	//setup vbo for colour attribute
	glGenBuffers(1, &meshTexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, meshTexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mTextureCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (const GLvoid*)0);

	// setting up the VBO for the princible axes of the model also assigning data space to it. Colour/texture, position, normals
	glGenBuffers(1, &MeshFaceVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshFaceVBO);

	unsigned int numBytes = scene->mMeshes[meshIndex]->mNumFaces * 3 * sizeof(unsigned int);

	unsigned int* FaceIndexArray = (unsigned int*)malloc(numBytes);

	for (int f = 0, dstIndex = 0; f < scene->mMeshes[meshIndex]->mNumFaces; f ++)
	{

		unsigned int* I = scene->mMeshes[meshIndex]->mFaces[f].mIndices;

		FaceIndexArray[dstIndex] = I[0];
		dstIndex += 1;
		FaceIndexArray[dstIndex] = I[1];
		dstIndex += 1;
		FaceIndexArray[dstIndex] = I[2];
		dstIndex += 1;



	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, FaceIndexArray, GL_STATIC_DRAW);

	//enabling the vertexes arrays and pointing the to the different arrays we wanna use 

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(6);


	//unbinding the principle axes from the VAO 
	glBindVertexArray(0);

	numfaces = scene->mMeshes[meshIndex]->mNumFaces;


}

//the basic renderer used to render our object after the data has been obtained
void meshVBO::render() 
{

	glBindVertexArray(meshVAO);
	glDrawElements(GL_TRIANGLES, numfaces * 3, GL_UNSIGNED_INT, (void*)0);

}