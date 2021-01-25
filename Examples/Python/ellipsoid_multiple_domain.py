# -*- coding: utf-8 -*-
"""
====================================================================
Full Example Pipeline for Statistical Shape Modeling with ShapeWorks
====================================================================

In this example we provide a full pipeline with an example dataset of axis 
aligned ellipsoid images with multiple domains
"""
import os
from GroomUtils import *
from OptimizeUtils import *
from AnalyzeUtils import *
import CommonUtils


def Run_Pipeline(args):
    """
    If ellipsoid_multiple_domain.zip is not there it will be downloaded from the ShapeWorks data portal.
    ellipsoid_multiple_domain.zip will be saved in the /Data folder and the data will be extracted 
    in a newly created directory Output/ellipsoid_multiple_domain.
    """
    print("\nStep 1. Extract Data\n")
    if int(args.interactive) != 0:
        input("Press Enter to continue")

    datasetName = "ellipsoid_md_sps"
    outputDirectory = "Output/ellipsoid_md_sps/"
    if not os.path.exists(outputDirectory):
        os.makedirs(outputDirectory)
    CommonUtils.download_and_unzip_dataset(datasetName, outputDirectory)

    meshFiles = sorted(glob.glob(outputDirectory + datasetName + "/meshes/*.ply"))
    imgFiles = sorted(glob.glob(outputDirectory + datasetName + "/segmentations/*.nrrd"))
    
    
    groomDir = outputDirectory + "groomed/"
    # """Apply centering"""
    centeredFiles = center(groomDir + "centered/segmentations", imgFiles)

    # """ Apply padding"""
    paddedFiles = applyPadding(groomDir + "padded/segmentations",centeredFiles,20)

    #"""Get the distance transforms """
    dtFiles = applyDistanceTransforms(groomDir, paddedFiles)
  
    pointDir = outputDirectory + 'shape_models/'
    if not os.path.exists(pointDir):
        os.makedirs(pointDir)

    parameterDictionary = {
        "number_of_particles" : [32,32],
        "use_normals": [0,0],
        "normal_weight": [1.0,1.0],
        "checkpointing_interval" : 200,
        "keep_checkpoints" : 0,
        "iterations_per_split" : 500,
        "optimization_iterations" : 500,
        "starting_regularization" :1000,
        "ending_regularization" : 0.5,
        "recompute_regularization_interval" : 2,
        "domains_per_shape" : 2,
        "domain_type" : 'imgage',
        "relative_weighting" : 1,
        "initial_relative_weighting" : 0.1,
        "procrustes_interval" : 0,
        "procrustes_scaling" : 0,
        "save_init_splits" : 0,
        "verbosity" : 3

      }

    if args.tiny_test:
        parameterDictionary["number_of_particles"] = 32
        parameterDictionary["optimization_iterations"] = 25

 
    """
    Now we execute a single scale particle optimization function.
    """
    print("CALLING OPTIMIZATION CODE")
    # [localPointFiles, worldPointFiles] = runShapeWorksOptimize(pointDir, meshFiles, parameterDictionary)
    runShapeWorksOptimize(pointDir, dtFiles, parameterDictionary)

    if args.tiny_test:
        print("Done with tiny test")
        exit()

    print("\nStep 5. Analysis - Launch ShapeWorksStudio - sparse correspondence model.\n")
    if args.interactive != 0:
        input("Press Enter to continue")

    # Image files are passed because Studio does not support viewing meshes yet.
    # launchShapeWorksStudio(pointDir, meshFiles, localPointFiles, worldPointFiles)
