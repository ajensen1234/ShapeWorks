import os
import sys
import glob
import shapeworks as sw

def boundingboxfailTest1():
  filenames = os.environ["DATA"] + "/empty/"
  region = sw.ImageUtils.boundingBox(glob.glob(filenames + "/*.nrrd"))

  return region.valid()

val = boundingboxfailTest1()

if val is False:
  sys.exit(1)

def boundingboxfailTest2():
  filenames = os.environ["DATA"] + "/single/"
  region = sw.ImageUtils.boundingBox(glob.glob(filenames + "/*.nrrd"))

  return region.valid()

val = boundingboxfailTest2()

if val is False:
  sys.exit(1)