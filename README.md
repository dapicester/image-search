Image Search Project
====================

This is the visual image search project.
It contains a Matlab prototype, a C++ library and a demo application.

Build Instructions
------------------

### Dependencies:

- CMake 2.8
- OpenCV
- VLFeat
- Boost
  - filesystem
  - iostreams
  - serialization
  - system
  - unit_test_framework
- Armadillo
- Qt 4.8

### Build

    $ mkdir build && cd build
    $ cmake ..
    $ make
    $ make test


