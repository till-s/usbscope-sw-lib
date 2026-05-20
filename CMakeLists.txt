cmake_minimum_required(VERSION 3.20)

set(SOURCES FWComm.cpp I2CDev.cpp FEC.cpp Max195xxADC.cpp PGAImpl.cpp VersaClk.cpp AcqCtrl.cpp Board.cpp H5Smpl.cpp Flash.cpp)

project(fwLib LANGUAGES CXX)

include_directories(. ../usbadc-support/sw)

add_library(fwLib ${SOURCES})

