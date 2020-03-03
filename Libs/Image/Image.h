#pragma once

#include "Shapeworks.h"
#include "Transform.h"
#include <limits>
#include <itkImage.h>

namespace shapeworks {

class Image
{
public:
  static const unsigned dims = 3;
  using PixelType = float;
  using ImageType = itk::Image<PixelType, dims>;

  Image() {}
  Image(const std::string &inFilename) { read(inFilename); }

  bool read(const std::string &inFilename);
  bool write(const std::string &outFilename, bool useCompression = true);
  bool antialias(unsigned numIterations = 50, float maxRMSErr = 0.01f, unsigned numLayers = dims);
  bool recenter();
  bool isoresample(double isoSpacing = 1.0f, Dims outputSize = Dims());
  bool pad(int padding, PixelType value);
  Point3 centerOfMass() const;
  void applyTransform(const Transform &transform);
  bool extractLabel(PixelType label = 1.0);
  bool closeHoles();
  bool threshold(PixelType min = std::numeric_limits<PixelType>::epsilon(), PixelType max = std::numeric_limits<float>::max());
  bool fastMarch(float isovalue);
  // bool nextfunction(...);

  bool operator==(const Image &other) const;

private:
  ImageType::Pointer image;
};

} // shapeworks
