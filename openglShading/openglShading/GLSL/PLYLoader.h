//
//  PLYLoader.h
//  PLYLoader
//
//  Created by JIANG, SHAN on 12-10-24.
//  Copyright (c) 2012年 JIANG, SHAN. All rights reserved.
//

#ifndef __PLYLoader__PLYLoader__
#define __PLYLoader__PLYLoader__

#include <iostream>
#include <stdio.h>

#include "Vertices.h"
#include "Primitives.h"

class PLYObject {
public:
    PLYObject(FILE *inFile);
    ~PLYObject();
    bool checkHeader(FILE *inFile);
    void readVertices(FILE *inFile);
    void readFaces(FILE *inFile);
    void facNor(Vec3 &nor, const Vec3 &vrt1, const Vec3 &vrt2, const Vec3 &vrt3);
    
    void toString(unsigned int num);
    
    Vertices vrt;
    Primitives fac;
    
    int nproperties;		// number of vertex properties
    int order[11];		// order of x,y,z, nx,ny,nz, red,green,blue, tu,tv vertex properties
    bool hasnormal, hascolor, hastexture;
    
    unsigned int nv, nf;		// number of vertices, faces
};

#endif /* defined(__PLYLoader__PLYLoader__) */
