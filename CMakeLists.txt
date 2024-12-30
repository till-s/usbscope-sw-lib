cmake_minimum_required(VERSION 3.5)

set( SOURCES DAC47cx.cpp FWComm.cpp I2CDev.cpp FEC.cpp Max195xxADC.cpp PGAImpl.cpp VersaClk.cpp AcqCtrl.cpp Board.cpp )
project(fwLib LANGUAGES CXX)

include_directories(. $ENV{HOME}/kicad/usbadc/artix7_v1/fw/modules/usbadc-support/sw)

add_library(fwLib ${SOURCES})

