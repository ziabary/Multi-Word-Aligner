#####################################################################
 # MWA : A Multi word aligner
 # Copyright (C) 2012-2014  S.Mohammad M. Ziabary <mehran.m@aut.ac.ir>
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
HEADERS += \
    src/Engine/intfExternalDictionary.hpp \
    src/Engine/intfExternalStemmer.hpp \
    src/Engine/intfBaseExternal.hpp \
    src/Engine/Engine.h \
    src/External/CachedDictionary.h \
    src/External/InteractiveDicAndStemmer.h \
    src/External/NullDicAndStemmer.h \
    src/External/GizaBasedDic.h \
    src/External/GlosbeDic.h \
    src/External/GoogleTranslate.h \
    src/External/WordnetStemmer.h \
    src/Common.h \
    src/exMWABase.hpp \
    src/External/Wordreference.h \
    src/Engine/Knowledge.h \
    src/Engine/ILA.h \
    src/Engine/HLMT.h \
    src/Engine/clsTranslatedPhrase.h
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    src/main.cpp \
    src/Engine/Engine.cpp \
    src/External/CachedDictionary.cpp \
    src/External/NullDicAndStemmer.cpp \
    src/External/InteractiveDicAndStemmer.cpp \
    src/External/GizaBasedDic.cpp \
    src/External/GlosbeDic.cpp \
    src/External/GoogleTranslate.cpp \
    src/External/WordnetStemmer.cpp \
    src/External/Wordreference.cpp \
    src/Engine/Knowledge.cpp \
    src/Engine/ILA.cpp \
    src/Engine/HLMT.cpp \
    src/Engine/clsTranslatedPhrase.cpp
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
INCLUDEPATH += src/

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
ASM_LIBPATH=$$(HOME)/local/lib
ASM_INCLUDE=$$(HOME)/local/include

QJSON_LIBPATH=$$(HOME)/local/lib
QJSON_INCLUDE=$$(HOME)/local/include

DEFINES+=USE_WN
WN_LIBPATH=$$(HOME)/local/lib
WN_INCLUDE=$$(HOME)/local/include

##########################################################
####          Do Not Change Anything Below            ####
##########################################################
TEMPLATE = app
TARGET = $$ProjectName
DESTDIR = out/
OBJECTS_DIR = out/build/obj
MOC_DIR = out/build/moc

QMAKE_CXXFLAGS += -std=c++0x

QT += core
QT -= gui

CONFIG(debug, debug|release): DEFINES += MWA_SHOW_DEBUG=1
DEFINES += PROJ_VERSION=$$VERSION

INCLUDEPATH  +=  $$ASM_INCLUDE $$QJSON_INCLUDE $$WN_INCLUDE
QMAKE_LIBDIR +=  $$ASM_LIBPATH $$QJSON_LIBPATH $$WN_LIBPATH
LIBS += -lASM -lcurl -lQJSON -lWN

!exists($$WN_LIBPATH"/libWN.a") : !exists($$WN_LIBPATH"/libWN.so") {
    message("Wordnet not found disabling it.")
    DEFINES -= USE_WN
    LIBS -= -lWN
}




