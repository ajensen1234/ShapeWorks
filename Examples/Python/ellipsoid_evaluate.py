import argparse
import os
import sys
import platform
from pathlib import Path
import glob
import shapeworks as sw
import numpy as np


def Run_Pipeline(args):
    ellipsoids_dir = 'Output/ellipsoid'
    shape_models_dir = f'{ellipsoids_dir}/shape_models/128'
    if args.tiny_test:
        shape_models_dir = f'{ellipsoids_dir}/shape_models/32'
    if not os.path.exists(shape_models_dir):
        print(
            f'Ellipsoids output not found in {shape_models_dir}. Please run the ellipsoid use case first.', file=sys.stderr)
        sys.exit(1)

    eval_dir = f'{ellipsoids_dir}/evaluation'
    for subdir in ('compactness', 'generalization', 'specificity'):
        Path(eval_dir).joinpath(Path(subdir)).mkdir(
            parents=True, exist_ok=True)

    # get the list of all the world particles
    particleFilesList = glob.glob(shape_models_dir+"/*world.particles")
    # read all he particles files into a particleSystem object
    particleSystem = sw.ParticleSystem(particleFilesList)

    print('\nCompactness\n'
          '-----------')
    """
    ComputeCompactness takes a particleSystem which has the particle data to calculate PCA explained variance
    and generate a scree.txt showing the explainability of each mode. This is used to computethe compactness 
    of the SSM.
    """

    # directory where the scree values will be saved
    save_dir = eval_dir + '/compactness/'

    # Calculate compactness and saved the values in scree.txt
    sw.ShapeEvaluation.ComputeCompactness(
        particleSystem=particleSystem, nModes=1, saveTo=save_dir+"scree.txt")
    if not args.tiny_test:
        sw.plot.plot_scree(save_dir)

    """
    ########################################################################################################
    """

    print('\nGeneralization\n'
          '--------------')
    """
    ComputeGeneralization takes a particleSystem which has the particle data and computes the generalization
    of the SSM. The reconstructions are saved, and the 0th and 100th percentile are opened in
    ShapeWorksStudio for visualization
    """

    # directory where the reconstructions related to generalization will be saved
    save_dir = eval_dir + '/generalization/'

    # Calculate generalization
    sw.ShapeEvaluation.ComputeGeneralization(
        particleSystem=particleSystem, nModes=1, saveTo=save_dir)
    if not args.tiny_test:
        sw.plot.visualize_reconstruction(save_dir)

    """
    ########################################################################################################
    """

    print('\nSpecificity\n'
          '--------------')

    """
    ComputeSpecificity takes a particleSystem which has the particle data  and computes the specficity
    of the SSM. The sampled reconstructions are saved, and the 0th and 100th percentile are opened in
    ShapeWorksStudio for visualization
    """

    # directory where the reconstructions related to specificity will be saved
    save_dir = eval_dir + '/specificity/'

    # Calculate specificity
    sw.ShapeEvaluation.ComputeSpecificity(
        particleSystem=particleSystem, nModes=1, saveTo=save_dir)
    if not args.tiny_test:
        sw.plot.visualize_reconstruction(save_dir)
