//
//  Points.h
//  PLYLoader
//
//  Created by JIANG, SHAN on 12-10-17.
//  Copyright (c) 2012 JIANG, SHAN. All rights reserved.
//

#ifndef PLYLoader_Points_h
#define PLYLoader_Points_h

#include "inc\Vec3.h"

//typedef float Vector3f[3];
//typedef float Texture2f[2];
typedef unsigned char Color3u[3];

class Points {
public:
    
    Points();
    //Points(unsigned int num);
    //Points(unsigned int num, bool hasColorMap);
    ~Points();
    
    void alcMem(unsigned int num, bool hasColorMap);
    void setVertices(Vec3 *vrt);
    void setColors(Color3u *col);
    void setMin(float x, float y, float z);
    void setMax(float x, float y, float z);
    void setMin(float val, int dim);
    void setMax(float val, int dim);
    
    Vec3 getMin();
    Vec3 getMax();
    float getMin(int dim);
    float getMax(int dim);
    Vec3 *getVertices();
    //Vector3f *getNormals();
    Color3u *getColors();
    bool hasColor();
    unsigned int getNumVrt();
    
protected:
    
    bool hasCol;
    unsigned int nv;		// number of vertices
    Vec3 min;
    Vec3 max;
    Vec3 *vertices;		// array of point coordinates
    //Vector3f *normals;		// array of point normals
    Color3u *colors;		// array of point colors
    //Texture2f *texcoords;		// array of texture coords
    //unsigned int *neighbors;		// array of face indices
};

#endif
