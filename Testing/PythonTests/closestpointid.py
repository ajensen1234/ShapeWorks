import os
import sys
from shapeworks import *

def closestpointidTest():
  mesh = Mesh(os.environ["DATA"] + "/ellipsoid_0.ply")
  p = mesh.getPoint(50)
  id = mesh.closestPointId(p)

  return id == 50

utils.test(closestpointidTest)
