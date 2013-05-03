//
//  Vertices.cpp
//  PLYLoader
//
//  Created by JIANG, SHAN on 12-10-19.
//  Copyright (c) 2012 JIANG, SHAN. All rights reserved.
//

#include <stdlib.h>

#include "Vertices.h"

Vertices::Vertices() {
    nn = 0;
    hasNor = false;
    hasTex = false;
}

Vertices::~Vertices()
{
    if (vertices)
        free(vertices);
    if (normals)
      free(normals);
    if (colors)
        free(colors);
    if (colors)
        free(colors);
    if (texCoords)
        free(texCoords);
}

void Vertices::alcMem(unsigned int num, bool hasColorMap, bool hasNormals, bool hasTexCoords)
{
    Points::alcMem(num, hasColorMap);

    hasNor = hasNormals;
    hasTex = hasTexCoords;

    normals = (Vec3 *)calloc(nv, sizeof(Vec3));
    if (hasTex)
        texCoords = (Vec2 *)calloc(nv, sizeof(Vec2));
}
void Vertices::setNormals(Vec3 *nor){
    for (unsigned int i = 0; i < nv; i ++) {
        normals[i] = nor[i];
    }
}
void Vertices::setTexCoords(Vec2 *tex) {
    for (unsigned int i = 0; i < nv; i++) {
        texCoords[i] = tex[i];
    }
}

Vec3 * Vertices::getNormals()
 {
 return normals;
 }

Vec2 * Vertices::getTexCoords()
{
    return texCoords;
}

unsigned int * Vertices::getNeighbors()
{
    return neighbors;
}
unsigned int Vertices::getNumNgb()
{
    return nn;
}

bool Vertices::hasTexture()
{
    return hasTex;
}