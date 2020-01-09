/*
 * The unified shapeworks executable for all operations of the ShapeWorks Framework.
 */

#include <exception>
#include "Executable.h"
#include "Commands.h"

using namespace Shapeworks;

int main(int argc, char *argv[])
{
  Executable shapeworks;

  shapeworks.addCommand(Antialias::getCommand());
  shapeworks.addCommand(Smoothmesh::getCommand()); 
  shapeworks.addCommand(ResampleImage::getCommand());
  shapeworks.addCommand(Binarize::getCommand());
  shapeworks.addCommand(RecenterImage::getCommand());
  //...
  
  try {
    shapeworks.run(argc, argv);
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  return 0;
}
