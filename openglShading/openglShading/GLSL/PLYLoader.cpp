//
//  PLYLoader.cpp
//  PLYLoader
//
//  Created by JIANG, SHAN on 12-10-24.
//  Copyright (c) 2012 JIANG, SHAN. All rights reserved.
//

#include "PLYLoader.h"

using namespace std;

Vec3 *vertices;
Color3u *colors;
Vec3 *normals;
Vec2 *textures;
Index3ui *indices;
Vec3 *fnormals;

float minTmp[3];
float maxTmp[3];

PLYObject::PLYObject(FILE *inFile)
{

    nproperties = 0;
    hasnormal = hascolor = hastexture = false;
    
    // init bounding box
    for (int i = 0; i < 3; i++) {
		minTmp[i] = FLT_MAX;
        maxTmp[i] = -FLT_MAX;
    }
    
    // default order
    for (int i = 0; i < 11; i++)
        order[i] = -1;
    
    if (!checkHeader(inFile)) {
        fprintf(stderr, "Error: could not read PLY file.\n");
        return;
    }
    
    vrt.alcMem(nv, hascolor, hasnormal, hastexture);
    fac.alcMem(nf);
    
    vertices = (Vec3 *)calloc(nv, sizeof(Vec3));
    normals = (Vec3 *)calloc(nv, sizeof(Vec3));
    if (hascolor) {
        colors = (Color3u *)calloc(nv, sizeof(Color3u));
    }
    if (!hasnormal) {
        fnormals = (Vec3 *)calloc(nf, sizeof(Vec3));
    }
    if (hastexture) {
        textures = (Vec2 *)calloc(nv, sizeof(Vec2));
    }
    readVertices(inFile);
    indices = (Index3ui *)calloc(nf, sizeof(Index3ui));
    readFaces(inFile);

    if (!hasnormal)
    {
      for (unsigned int i = 0; i < nv; i++)
        normals[i] = Unit(normals[i]);
      free(fnormals);
      hasnormal = true;
    }

    vrt.setVertices(vertices);
    vrt.setMin(minTmp[0], minTmp[1], minTmp[2]);
    vrt.setMax(maxTmp[0], maxTmp[1], maxTmp[2]);
    vrt.setNormals(normals);
    fac.setIndices(indices);
    free(vertices);
    free(normals);
    if (hascolor) {
        free(colors);
    }
    if (hastexture) {
        free(textures);
    }
    free(indices);
    
}
PLYObject::~PLYObject()
{
    if (order) {
        free(order);
    }
    delete &vrt;
    delete &fac;
}
bool PLYObject::checkHeader(FILE *inFile)
{
    char buf[128], type[128], c[32];
    int i;
    
    // read ply file header
    fscanf(inFile, "%s\n", buf);
    if (strcmp(buf, "ply") != 0) {
        fprintf(stderr, "Error: Input file is not of .ply type.\n");
        return false;
    }
    fgets(buf, 128, inFile);
    if (strncmp(buf, "format ascii", 12) != 0) {
        fprintf(stderr, "Error: Input file is not in ASCII format.\n");
        return false;
    }
    
    fgets(buf, 128, inFile);
    while (strncmp(buf, "comment", 7) == 0)
        fgets(buf, 128, inFile);
    
    // read number of vertices
    if (strncmp(buf, "element vertex", 14) == 0)
        sscanf(buf, "element vertex %d\n", &nv);
    else {
        fprintf(stderr, "Error: number of vertices expected.\n");
        return false;
    }
    
    // read vertex properties order
    i = 0;
    fgets(buf, 128, inFile);
    while (strncmp(buf, "property", 8) == 0) {
        sscanf(buf, "property %s %s\n", type, c);
        if (strncmp(c, "x", 1) == 0)
            order[0] = i;
        else if (strncmp(c, "y", 1) == 0)
            order[1] = i;
        else if (strncmp(c, "z", 1) == 0)
            order[2] = i;
        
        else if (strncmp(c, "nx", 2) == 0)
            order[3] = i;
        else if (strncmp(c, "ny", 2) == 0)
            order[4] = i;
        else if (strncmp(c, "nz", 2) == 0)
            order[5] = i;
        
        else if (strncmp(c, "red", 3) == 0)
            order[6] = i;
        else if (strncmp(c, "green", 5) == 0)
            order[7] = i;
        else if (strncmp(c, "blue", 4) == 0)
            order[8] = i;
        
        else if (strncmp(c, "tu", 2) == 0)
            order[9] = i;
        else if (strncmp(c, "tv", 2) == 0)
            order[10] = i;
        
        i++;
        fgets(buf, 128, inFile);
    }
    nproperties = i;
    
    for (i = 0; i < 3; i++) {
        if (order[i] < 0) {
            fprintf(stderr, "Error: not enough vertex coordinate fields (nx, ny, nz).\n");
            return false;
        }
    }
    hasnormal = true;
    for (i = 3; i < 6; i++)
        if (order[i] < 0)
            hasnormal = false;
    hascolor = true;
    for (i = 6; i < 9; i++)
        if (order[i] < 0)
            hascolor = false;
    hastexture = true;
    for (i = 9; i < 11; i++)
        if (order[i] < 0)
            hastexture = false;
    
    if (!hasnormal)
        fprintf(stderr, "Warning: no normal coordinates used from file.\n");
    if (!hascolor)
        fprintf(stderr, "Warning: no color used from file.\n");
    if (!hastexture)
        fprintf(stderr, "Warning: no texture coordinates used from file.\n");
    
    // number of faces and face properties
    if (strncmp(buf, "element face", 12) == 0)
        sscanf(buf, "element face %d\n", &nf);
    else {
        fprintf(stderr, "Error: number of faces expected.\n");
        return false;
    }
    
    fgets(buf, 128, inFile);
    if (strncmp(buf, "property list", 13) != 0) {
        fprintf(stderr, "Error: property list expected.\n");
        return false;
    }
    
    fgets(buf, 128, inFile);
    while (strncmp(buf, "end_header", 10) != 0)
        fgets(buf, 128, inFile);
    
    return true;
}

void PLYObject::readVertices(FILE *inFile)
{
    char buf[128];
    unsigned int i;
    int j;
    float values[32];
    
    vertices = (Vec3 *)calloc(nv, sizeof(Vec3));
    
    // read in vertex attributes
    for (i = 0; i < nv; i++) {
        fgets(buf, 128, inFile);
        sscanf(buf,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", &values[0], &values[1], &values[2], &values[3],
               &values[4], &values[5], &values[6], &values[7], &values[8], &values[9], &values[10], &values[11],
               &values[12], &values[13], &values[14], &values[15]);
        
        //for (j = 0; j < 3; j++)
        vertices[i].x = values[order[0]];
        vertices[i].y = values[order[1]];
        vertices[i].z = values[order[2]];
        
        if (hasnormal){
            normals = (Vec3 *)calloc(nv, sizeof(Vec3));
            normals[i].x = values[order[3]];
            normals[i].y = values[order[4]];
            normals[i].z = values[order[5]];
        }
        if (hascolor)
        {
            colors = (Color3u *)calloc(nv, sizeof(Color3u));
            for (j = 0; j < 3; j++)
                colors[i][j] = (unsigned char)values[order[6+j]];
        }
        if (hastexture){
            textures = (Vec2 *)calloc(nv, sizeof(Vec2));
            textures[i].x = values[order[9]];
            textures[i].y = values[order[10]];
        }
        
        if (vertices[i].x < minTmp[0]) {
            minTmp[0] = vertices[i].x;
        }
        if (vertices[i].x > maxTmp[0]) {
            maxTmp[0] = vertices[i].x;
        }
        if (vertices[i].y < minTmp[1]) {
            minTmp[1] = vertices[i].y;
        }
        if (vertices[i].y > maxTmp[1]) {
            maxTmp[1] = vertices[i].y;
        }
        if (vertices[i].z < minTmp[2]) {
            minTmp[2] = vertices[i].z;
        }
        if (vertices[i].z > maxTmp[2]) {
            maxTmp[2] = vertices[i].z;
        }
    }
}
 
void PLYObject::readFaces(FILE *inFile)
{
    char buf[128];
    unsigned int i;
    int k;
    
    // read in face connectivity
    for (i = 0; i < nf; i++) {
        fgets(buf, 128, inFile);
        sscanf(buf, "%d %d %d %d", &k, &indices[i][0], &indices[i][1], &indices[i][2]);
        if (k != 3) {
            cout << k << endl;
            fprintf(stderr, "Error: not a triangular face will be dealt later.\n");
            exit(1);
        }
        
        if (!hasnormal)
        {
            // set up face normal
            facNor(fnormals[i], vertices[indices[i][0]], vertices[indices[i][1]], vertices[indices[i][2]]);

            // accumulate normal information of each vertex
            for (int j = 0; j < 3; j++)
                normals[indices[i][j]] += fnormals[i];
        }
    }
}

void PLYObject::facNor(Vec3 &nor, const Vec3 &vrt1, const Vec3 &vrt2, const Vec3 &vrt3)
{
    Vec3 u, v;
    u = vrt2 - vrt1;
    v = vrt3 - vrt1;
    nor = u ^ v;
    nor = Unit(nor);
}

void PLYObject::toString(unsigned int num)
{
    unsigned int numOutput = 0;

    cout << "# of vertices: " << vrt.getNumVrt() << "." << endl;
    cout << "Min: " << vrt.getMin().x << " " << vrt.getMin().y << " " << vrt.getMin().z << endl;
    cout << "Max: " << vrt.getMax().x << " " << vrt.getMax().y << " " << vrt.getMax().z << endl;
    numOutput = num > nv ? nv : num;
    cout << "Vertices: " << endl;
    for (unsigned int i = 0; i < numOutput; i ++) {
        cout << vrt.getVertices()[i].x << " " << vrt.getVertices()[i].y << " " << vrt.getVertices()[i].z << endl;
    }
    cout << "Normals: " << endl;
    for (unsigned int i = 0; i < numOutput; i ++) {
        cout << vrt.getNormals()[i].x << " " << vrt.getNormals()[i].y << " " << vrt.getNormals()[i].z << endl;
    }
    cout << "# of faces: " << fac.getNumFac() << "." << endl;
    numOutput = num > nf ? nf : num;
    for (unsigned int i = 0; i < numOutput; i ++) {
        cout << fac.getIndices()[i][0] << " " << fac.getIndices()[i][1] << " " << fac.getIndices()[i][2] << endl;
    }
    
}