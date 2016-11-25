#!/bin/bash

FENICS_HOME=/home/fenics
FENICS_SHARED="${FENICS_HOME}/shared"
DEPS="${FENICS_SHARED}/deps"
export PATH=${DEPS}/cmake/bin:${PATH}

# Install gtest
cd googletest/build
sudo cp -r ../googletest/include/gtest /usr/local/include/
sudo cp googlemock/gtest/lib*.a /usr/local/lib

# Install Spacy
cd ${DEPS}
cd Spacy/build
sudo make install
#git clone https://github.com/spacy-dev/Spacy.git
#cd Spacy && mkdir build && cd build && cmake .. -DDolfin=ON && make -j2 && sudo make install

# Run tests
cd ${FENICS_SHARED}
mkdir build && cd build
cmake .. -DEigen=ON -DDolfin=ON
make -j2 && ./FEniCS_vectorCreator

