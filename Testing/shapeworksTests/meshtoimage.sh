#! /bin/bash

shapeworks readmesh --name $DATA/femur.ply meshtoimage --sizex 92 --sizey 67 --sizez 131 compareimage --name $DATA/femurImage.nrrd
if [[ $? != 0 ]]; then exit -1; fi
shapeworks readmesh --name $DATA/femur.ply meshtoimage antialias --iterations 50 --maxrmserror 0.0 compareimage --name $DATA/antialias3.nrrd
