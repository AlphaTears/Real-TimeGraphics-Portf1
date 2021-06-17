#pragma once

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <CoreStructures/GUPivotCamera.h>
#include <string>

const aiScene *aiImportModel(const std::string& filename, unsigned int flags);
void aiReportScene(const aiScene* scene);
void aiRender(const aiScene* scene, CoreStructures::GUPivotCamera *mainCamera);
