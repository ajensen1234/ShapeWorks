from pythonTestSetup import *

setup()

from shapeworks import *

def clipfailTest1():
  img = Image(os.environ["DATA"] + "1x2x2.nrrd")
  img.clip(Point(1, 0.0, 0.0), Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 0.0), 0.0)

  compareImg = Image(os.environ["DATA"] + "clipfail.nrrd")

  return img.compare(compareImg)

val = clipfailTest1()

if val is False:
  sys.exit(1)

def clipfailTest2():
  img = Image(os.environ["DATA"] + "1x2x2.nrrd")
  img.clip(Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 0.0), Point(0.0, 0.0, 0.0), 0.0)

  compareImg = Image(os.environ["DATA"] + "clipfail.nrrd")

  return img.compare(compareImg)

val = clipfailTest2()

if val is False:
  sys.exit(1)