Image Search Project
====================

This is the visual image search project.
It contains a Matlab prototype, a C++ library, a TCP server and a demo application.

Build Instructions
------------------

### Dependencies:

- CMake 2.8
- OpenCV
- VLFeat
- Boost
  - filesystem
  - iostreams
  - program_options
  - serialization
  - system
  - thread
  - unit_test_framework
- Armadillo
- Qt 4.8
- yaml-cpp

Optional

- Gnuplot
- Gnuplot-iostream

### Build

    $ mkdir build && cd build
    $ cmake ..
    $ make
    $ make test


