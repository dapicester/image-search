Image Search Project
====================

This is the visual image search project.
It contains a Matlab prototype, a C++ library, a TCP server and a demo application.

IMPORTANT
---------

I haven't worked on this for more than one year, and now it compilation fails
with the latest version of the dependent libraries.

Waiting for updating the core, below is the list of dependencies that are known
successfully compile.

Build Instructions
------------------

### Dependencies:

- CMake 2.8
- OpenCV (2.4.12)
- VLFeat (0.9.17)
- Boost (1.55)
  - filesystem
  - iostreams
  - program_options
  - serialization
  - system
  - thread
  - unit_test_framework
- Armadillo (3.920.2)
- Qt4 (4.8)
- yaml-cpp (0.5.1)

Optional

- Gnuplot
- Gnuplot-iostream

### Build

    $ mkdir build && cd build
    $ cmake ..
    $ make
    $ make test


