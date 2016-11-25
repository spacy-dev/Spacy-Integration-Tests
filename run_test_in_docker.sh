#!/bin/bash

FENICS_HOME=/home/fenics
FENICS_SHARED="${FENICS_HOME}/shared"
DEPS="${FENICS_SHARED}/deps"

# Install cmake
cd $DEPS
CMAKE_URL="http://www.cmake.org/files/v3.5/cmake-3.5.2-Linux-x86_64.tar.gz"
mkdir cmake && wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
export PATH=${DEPS}/cmake/bin:${PATH}

# Install gtest
git clone https://github.com/google/googletest.git
cd googletest && mkdir build && cd build && cmake .. && make
sudo cp -r ../googletest/include/gtest /usr/local/include/
sudo cp googlemock/gtest/lib*.a /usr/local/lib

# Install Spacy
cd ${DEPS}
git clone https://github.com/spacy-dev/Spacy.git
cd Spacy && mkdir build && cd build && cmake .. -DDolfin=ON && make && sudo make install

# Run tests
cd ${FENICS_SHARED}
mkdir build && cd build
cmake .. -DEigen=ON -DDolfin=ON
make && ctest

