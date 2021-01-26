#pragma once

#include "ShapeworksUtils.h"
#include "Mesh.h"
#include "Eigen/Core"
#include "Eigen/Dense"
#include <vtkPolyData.h>

namespace shapeworks {

/// Helper functions for meshes 
class MeshUtils
{
public:

  /// Computes a rigid transformation from source to target using vtkIterativeClosestPointTransform
  static const vtkSmartPointer<vtkMatrix4x4> createIcpTransform(const vtkSmartPointer<vtkPolyData> source, const vtkSmartPointer<vtkPolyData> target, const unsigned iterations = 20);

  /// Distils the vertex and face information from VTK poly data to Eigen matrices
  static Eigen::MatrixXd distilVertexInfo(vtkSmartPointer<vtkPolyData> mesh);
  static Eigen::MatrixXi distilFaceInfo(vtkSmartPointer<vtkPolyData> mesh);

  /// Compute the warp matrix using the mesh and reference points
  static Eigen::MatrixXd generateWarpMatrix(Eigen::MatrixXd TV , Eigen::MatrixXi TF, Eigen::MatrixXd Vref);

  /// Compute individual warp
  static Mesh warpMesh(Eigen::MatrixXd movPts, Eigen::MatrixXd W, Eigen::MatrixXi Fref);

  /// Compute transformation from set of points files using template mesh warp&face matrices
  static bool warpMeshes(std::vector< std::string> movingPointpaths, std::vector< std::string> outputMeshPaths, Eigen::MatrixXd W, Eigen::MatrixXi Fref, const int numP);

  static Mesh thread_safe_read_mesh(std::string filename);

};

} // shapeworks
