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
VERSION = 0.1.0

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
Dependencies += ASM
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    src/Engine/intfExternalDictionary.hpp \
    src/Engine/intfExternalStemmer.hpp \
    src/Engine/intfBaseExternal.hpp \
    src/Engine/Engine.h \
    src/External/CachedDictionary.h \
    src/External/InteractiveDicAndStemmer.h \
    src/External/NullDicAndStemmer.h
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    src/main.cpp \
    src/Engine/Engine.cpp \
    src/External/CachedDictionary.cpp \
    src/External/NullDicAndStemmer.cpp \
    src/External/InteractiveDicAndStemmer.cpp
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
INCLUDEPATH += src

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
QMAKE_CXXFLAGS += -std=c++0x

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
ASM_LIBPATH=$$(HOME)/local/lib
ASM_INCLUDE=$$(HOME)/local/include

##########################################################
####          Do Not Change Anything Below            ####
##########################################################
TEMPLATE = app
TARGET = $$ProjectName
DESTDIR = build
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/rcc

QT += core
QT -= gui

CONFIG(debug, debug|release): DEFINES += MWA_SHOW_DEBUG=1
DEFINES += PROJ_VERSION=$$VERSION

INCLUDEPATH  +=  $$ASM_INCLUDE
QMAKE_LIBDIR +=  $$ASM_LIBPATH
LIBS += -lASM







