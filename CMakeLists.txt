cmake_minimum_required(VERSION 3.5)

set( SOURCES DAC47cx.cpp FWComm.cpp I2CDev.cpp I2CFEC.cpp Max195xxADC.cpp AD8370PGA.cpp Lmh6882PGA.cpp TCA6408FEC.cpp VersaClk.cpp AcqCtrl.cpp Board.cpp )
project(fwLib LANGUAGES CXX)

include_directories(. $ENV{HOME}/kicad/usbadc/artix7_v1/fw/modules/usbadc-support/sw)

add_library(fwLib ${SOURCES})

