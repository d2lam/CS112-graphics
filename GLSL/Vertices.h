//
//  Vertices.h
//  PLYLoader
//
//  Created by JIANG, SHAN on 12-10-19.
//  Copyright (c) 2012 JIANG, SHAN. All rights reserved.
//

#ifndef PLYLoader_Vertices_h
#define PLYLoader_Vertices_h

#include "Points.h"
#include "inc\Vec2.h"

class Vertices: public Points{
public:
    Vertices();
    ~Vertices();
    
    void alcMem(unsigned int num, bool hasColorMap, bool hasNormal, bool hasTexCoords);
    void setNormals(Vec3 *nor);
    void setTexCoords(Vec2 *tex);
    Vec3 *getNormals();
    Vec2 *getTexCoords();
    unsigned int *getNeighbors();
    bool hasTexture();
    unsigned int getNumNgb();
    
private:
    
    bool hasNor, hasTex;
    Vec3 *normals;		// array of point normals
    Vec2 *texCoords;		// array of texture coords
    unsigned int *neighbors;
    unsigned int nn;
};

#endif
