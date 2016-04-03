#pragma once
#include "SOIL.h"
#include <stdio.h>
#include <vector>
#include "Vertex.h"
#include "Texture.h"
#include <GL/glew.h>
#include "shader.hpp"
#include <sstream>
#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;
using namespace ballah;
using namespace graphics;

class Model
{
public:
	/* Functions */
	Model(GLchar* path) {this->loadModel(path);}
	void Draw(Shader *shader);
private:
	/* Model Data */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;
	
	/* Functions */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

