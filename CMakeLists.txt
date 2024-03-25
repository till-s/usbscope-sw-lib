cmake_minimum_required(VERSION 3.5)

set( SOURCES DAC47cx.cpp FWComm.cpp I2CDev.cpp I2CFEC.cpp LED.cpp Max195xxADC.cpp PGAAD8370.cpp PGALmh6882.cpp TCA6408FEC.cpp VersaClk.cpp AcqCtrl.cpp)
project(fwLib LANGUAGES CXX)

include_directories(. $ENV{HOME}/kicad/usbadc/artix7_v1/fw/modules/usbadc-support/sw)

add_library(fwLib ${SOURCES})

