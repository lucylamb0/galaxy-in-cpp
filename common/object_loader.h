//
// Created by User on 04/08/2022.
//

#ifndef C_VERSION_OBJECT_LOADER_H
#define C_VERSION_OBJECT_LOADER_H


#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <vector>

using namespace std;

class ImportedModel
{
private:
    int _numVertices;     //Total number of all vertex coordinates
    std::vector<glm::vec3> _vertices;    //Number of all vertices, including (x,y,z)
    std::vector<glm::vec2> _texCoords;   //Texture coordinates (u, v)
    std::vector<glm::vec3> _normalVecs;     //normal
public:
    ImportedModel();
    ImportedModel(const char* filePath);
    int getNumVertices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTextureCoords();
    std::vector<glm::vec3> getNormals();
};


class ModelImporter
{
private:
    std::vector<float> _vertVals;
    std::vector<float> _triangleVerts;
    std::vector<float> _textureCoords;
    std::vector<float> _stVals;
    std::vector<float> _normals;
    std::vector<float> _normVals;
public:
    ModelImporter();
    void parseOBJ(const char* filePath);
    int getNumVertices();
    std::vector<float> getVertices();
    std::vector<float> getTextureCoordinates();
    std::vector<float> getNormals();

};



#endif //C_VERSION_OBJECT_LOADER_H
