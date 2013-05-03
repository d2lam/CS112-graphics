//
//  Primitives.cpp
//  PLYLoader
//
//  Created by JIANG, SHAN on 12-10-19.
//  Copyright (c) 2012 JIANG, SHAN. All rights reserved.
//
#include <stdlib.h>

#include "Primitives.h"

Primitives::Primitives(){
    nf = 0;
}
Primitives::~Primitives()
{
    if (indices)
        free(indices);
}
void Primitives::alcMem(unsigned int num)
{
    nf = num;
    indices = (Index3ui *)calloc(nf, sizeof(Index3ui));
}
void Primitives::setIndices(Index3ui *idx){
    for (unsigned int i = 0; i < nf; i ++) {
        for (int j = 0; j < 3; j ++) {
            indices[i][j] = idx[i][j];
        }
    }
}
Index3ui * Primitives::getIndices()
{
    return indices;
    
}
unsigned int Primitives::getNumFac()
{
    return nf;
}