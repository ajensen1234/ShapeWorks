#pragma once

#include "Shapeworks.h"
#include "ImageUtils.h"

#include <vtkPolyData.h>
#include <vtkPlane.h>
#include <swHausdorffDistancePointSetFilter.h>
#include <string>

namespace shapeworks {

class Mesh
{
public:
  using MeshType = vtkSmartPointer<vtkPolyData>; // TODO: we need to support multiple mesh types, such as vtkPolyData and vtkPLYData; probably use vtk mesh base class (if one exists)

  /// Logical region of a mesh
  struct Region
  {
    Coord min = Coord({ 1000000000, 1000000000, 1000000000 });
    Coord max = Coord({ -1000000000, -1000000000, -1000000000 });
    Region(const Dims &dims) : min({0, 0, 0}) {
      if (0 != (dims[0] + dims[1] + dims[2]))
        max = Coord({static_cast<long>(dims[0])-1,
                     static_cast<long>(dims[1])-1,
                     static_cast<long>(dims[2])-1});
    }
    Region(const Coord &_min, const Coord &_max) : min(_min), max(_max) {}
    Region() = default;
    bool operator==(const Region &other) const { return min == other.min && max == other.max; }

    /// verified min/max do not create an inverted or an empty region
    bool valid() const { return max[0] > min[0] && max[1] > min[1] && max[2] > min[2]; }

    Coord origin() const { return min; }
    Dims size() const {
      return Dims({static_cast<unsigned long>(max[0]-min[0]),
                   static_cast<unsigned long>(max[1]-min[1]),
                   static_cast<unsigned long>(max[2]-min[2])});
    }

    /// grows or shrinks the region by the specified amount
    void pad(int padding) {
      for (auto i=0; i<3; i++) {
        min[i] -= padding;
        max[i] += padding;
      }
    }

    /// shrink this region down to the smallest portions of both
    void shrink(const Region &other) {
      for (auto i=0; i<3; i++) {
        min[i] = std::max(min[i], other.min[i]);
        max[i] = std::min(max[i], other.max[i]);
      }
    }

    /// grow this region up to the largest portions of both
    void grow(const Region &other) {
      for (auto i=0; i<3; i++) {
        min[i] = std::min(min[i], other.min[i]);
        max[i] = std::max(max[i], other.max[i]);
      }
    }
  };

  Mesh(vtkSmartPointer<vtkPolyData>&& rhs) : mesh(std::move(rhs)) {}
  Mesh(const std::string& pathname) : mesh(read(pathname)) {}

  // return the current mesh
  MeshType getMesh() const { return this->mesh; }

  /// writes mesh, format specified by filename extension
  Mesh& write(const std::string &pathname);

  /// creates mesh of coverage between two meshes
  Mesh& coverage(const Mesh& other_mesh);

  /// applies filter to generates isosurface
  Mesh& march(double levelset = 0.0);

  /// applies laplacian smoothing
  Mesh& smooth(int iterations = 0, double relaxation = 0.0);

  /// applies filter to reduce number of triangles in mesh
  Mesh& decimate(double reduction = 0.0, double angle = 0.0, bool preservetopology = false);

  /// reflect meshes with respect to a specified center and specific axis
  Mesh& reflect(const Axis &axis, const Vector3 &origin = makeVector({ 0.0, 0.0, 0.0 }));

  /// applies the given transformation to the mesh
  Mesh& applyTransform(const vtkTransform transform);

  /// finds holes in a mesh and closes them
  Mesh& fillHoles();

  /// samples data values at specified point locations
  Mesh& probeVolume(const Image &img);

  /// clips a mesh using a cutting plane
  Mesh& clip(const vtkSmartPointer<vtkPlane> plane);

  /// helper to translate mesh
  Mesh& translate(const Vector3 &v);

  /// helper to scale mesh
  Mesh& scale(const Vector3 &v);

  /// computes bounding box of current mesh
  Mesh::Region boundingBox(bool center=false) const;

  /// compute surface to surface distance
  vtkSmartPointer<swHausdorffDistancePointSetFilter> distance(const std::unique_ptr<Mesh> &other_mesh, bool target=false);

  /// returns surface to surface distance or hausdorff distance
  Vector hausdorffDistance(const std::unique_ptr<Mesh> &other_mesh, bool target=false);

  /// returns relative distance from mesh A to mesh B
  Vector relativeDistanceAtoB(const std::unique_ptr<Mesh> &other_mesh, bool target=false);

  /// returns relative distance from mesh B to mesh A
  Vector relativeDistanceBtoA(const std::unique_ptr<Mesh> &other_mesh, bool target=false);

  /// compute origin of volume that would contain the rasterization of each mesh
  Point3 rasterizationOrigin(int padding, Vector3 spacing);

  /// compute size of volume that would contain the rasterization of each mesh
  Dims rasterizationSize(int padding, Vector3 spacing);

  // query functions //

  /// center of mesh
  Point3 center() const;

  /// compare if points in two meshes are equal
  bool compare_points_equal(const Mesh& other_mesh);

  /// compare if scalars in two meshes are equal
  bool compare_scalars_equal(const Mesh& other_mesh);

private:
  friend struct SharedCommandData;
  Mesh() : mesh(nullptr) {} // only for use by SharedCommandData since a Mesh should always be valid, never "empty"

  /// reads mesh (used only by constructor)
  static MeshType read(const std::string& pathname);

  MeshType mesh;
};

} // shapeworks
