Image Search
============

This is the visual image search project.
It contains the following modules:
- Matlab prototype
- C++ library
- TCP server

As usage examples, two applications are provided:
- GUI application using the C++ library
- a web application using the TCP server

Build Instructions
------------------

It is possible to use Vagrant to setup and build the project.

Please refer to the Ansible provisioning script for a list of
required dependencies.

Build C++ modules:

    $ mkdir build && cd build
    $ cmake ..
    $ make
    $ make test

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

