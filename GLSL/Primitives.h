//
//  Primitives.h
//  PLYLoader
//
//  Created by JIANG, SHAN on 12-10-19.
//  Copyright (c) 2012年 JIANG, SHAN. All rights reserved.
//

#ifndef PLYLoader_Primitives_h
#define PLYLoader_Primitives_h

typedef int Index3ui[3];

class Primitives {
public:
    
    Primitives();
    //Primitives(unsigned int num);
    ~Primitives();
    
    void alcMem(unsigned int num);
    void setIndices(Index3ui *idx);
    
    Index3ui *getIndices();
    unsigned int getNumFac();
    
private:
    
    Index3ui *indices;		// array of vertex indices
    unsigned int nf;
};

#endif
