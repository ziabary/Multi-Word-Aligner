#####################################################################
 # MWA : A Multi word aligner
 # Copyright (C) 2012-2014  S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
 #
 # This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation; either version 2 of the License, or
 # (at your option) any later version.

 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.

 # You should have received a copy of the GNU General Public License along
 # with this program; if not, write to the Free Software Foundation, Inc.,
 # 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#####################################################################
ProjectName = "MWA"
BasePath=..

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
Dependencies += ASM curl
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    ExternUnitTest.h
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    ExternUnitTest.cpp
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
INCLUDEPATH += ../src
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
QMAKE_CXXFLAGS += -std=c++0x
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
ASM_LIBPATH=$$(HOME)/local/lib
ASM_INCLUDE=$$(HOME)/local/include

DEFINES+=USE_WN
WN_LIBPATH=$$(HOME)/local/lib
WN_INCLUDE=$$(HOME)/local/include

##########################################################
####          Do Not Change Anything Below            ####
##########################################################
CONFIG += qtestlib
TEMPLATE = app
TARGET = unitTest
DESTDIR = $$BasePath/build
OBJECTS_DIR = $$BasePath/build/obj
MOC_DIR = $$BasePath/build/moc

QT += core
QT -= gui

INCLUDEPATH  +=  $$ASM_INCLUDE $$WN_INCLUDE
QMAKE_LIBDIR +=  $$ASM_LIBPATH $$WN_LIBPATH
LIBS += -lASM -lcurl -lWN

!exists($$WN_LIBPATH"/libWN.a") : !exists($$WN_LIBPATH"/libWN.so") {
    message("Wordnet not found disabling it.")
    DEFINES -= USE_WN
    LIBS -= -lWN
}
