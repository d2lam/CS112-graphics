/*////////////////////////////////////////////////////////////////////////////////////
Read Input Data Files
////////////////////////////////////////////////////////////////////////////////////*/

#include "inc\essentials.h"
#include <fstream>


PLYObject *model;

void FitModel(PLYObject* M){
	Vec3 MinBounds=M->vrt.getMin();
	Vec3 MaxBounds=M->vrt.getMax();
	for(int i=0;i<model->nv;i++){
		model->vrt.getVertices()[i].x=-0.5+(model->vrt.getVertices()[i].x-MinBounds.x)/(MaxBounds.x-MinBounds.x);
		model->vrt.getVertices()[i].y=-0.5+(model->vrt.getVertices()[i].y-MinBounds.y)/(MaxBounds.y-MinBounds.y);
		model->vrt.getVertices()[i].z=-0.5+(model->vrt.getVertices()[i].z-MinBounds.z)/(MaxBounds.z-MinBounds.z);
	}
}

void ReadFile(string s){
	FILE *fp=fopen(s.c_str(),"r");
	if(fp==NULL){
		cout<<"File does not exist.";
		exit(0);
	}
	model=new PLYObject(fp);
	FitModel(model);
	//model->toString(5000);
}