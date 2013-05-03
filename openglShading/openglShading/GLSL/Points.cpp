//
//  Points.cpp
//  PLYLoader
//
//  Created by JIANG, SHAN on 12-10-17.
//  Copyright (c) 2012年 JIANG, SHAN. All rights reserved.
//
#include <stdlib.h>

#include "Points.h"

Points::Points()
{
    nv = 0;
    hasCol = false;
}
/*
Points::Points(unsigned int num)
{
    nv = num;
    hasCol = false;
    vertices = (Vec3 *)calloc(nv, sizeof(Vec3));
    //normals = (Vector3f *)calloc(nv, sizeof(Vector3f));
}
Points::Points(unsigned int num, bool hasColorMap)
{
    nv = num;
    hasCol = hasColorMap;
    vertices = (Vec3 *)calloc(nv, sizeof(Vec3));
    //normals = (Vector3f *)calloc(nv, sizeof(Vector3f));
    if (hasCol)
        colors = (Color3u *)calloc(nv, sizeof(Color3u));
}*/
Points::~Points()
{
    if (vertices)
        free(vertices);
    //if (normals)
      //  free(normals);
    if (colors)
        free(colors);
}
void Points::alcMem(unsigned int num, bool hasColorMap)
{
    nv = num;
    hasCol = hasColorMap;
    vertices  = (Vec3 *)calloc(nv, sizeof(Vec3));
    if (hasColorMap) {
        colors = (Color3u *)calloc(nv, sizeof(Color3u));
    }
}
void Points::setVertices(Vec3 *vrt)
{
    for (unsigned int i = 0; i < nv; i ++)
    {
        vertices[i] = vrt[i];
    }
}
void Points::setColors(Color3u *col)
{
    for (unsigned int i = 0; i < nv; i ++)
    {
        for (int j = 0; j < 3; j ++) {
            colors[i][j] = col[i][j];
        }
    }
}
void Points::setMin(float x, float y, float z)
{
    min.x = x;
    min.y = y;
    min.z = z;
}
void Points::setMax(float x, float y, float z)
{
    max.x = x;
    max.y = y;
    max.z = z;
}
void Points::setMin(float val, int dim)
{
    switch (dim) {
        case 0:
            min.x = val;
            break;
        case 1:
            min.y = val;
            break;
        case 2:
            min.z = val;
            break;
            
        default:
            //do nothing
            break;
    }
}
void Points::setMax(float val, int dim)
{
    switch (dim) {
        case 0:
            max.x = val;
            break;
        case 1:
            max.y = val;
            break;
        case 2:
            max.z = val;
            break;
            
        default:
            //do nothing
            break;
    }
}
Vec3 Points::getMin()
{
    return min;
}
Vec3 Points::getMax()
{
    return max;
}
float Points::getMin(int dim)
{
    switch (dim) {
        case 0:
            return min.x;
            break;
        case 1:
            return min.y;
            break;
        case 2:
            return min.z;
            break;
            
        default:
            //do nothing
            return 0.0;
            break;
    }

}
float Points::getMax(int dim)
{
    switch (dim) {
        case 0:
            return max.x;
            break;
        case 1:
            return max.y;
            break;
        case 2:
            return max.z;
            break;
            
        default:
            //do nothing
            return 0.0;
            break;
    }
}
Vec3 * Points::getVertices()
{
    return vertices;
    
}
/*
Vector3f * Points::getNormals()
{
    return normals;
}*/
Color3u * Points::getColors()
{
    return colors;
}
unsigned int Points::getNumVrt()
{
    return nv;
}
bool Points::hasColor()
{
    return hasCol;
}
