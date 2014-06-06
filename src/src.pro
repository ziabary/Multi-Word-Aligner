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
BasePath=..

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    Engine/intfExternalDictionary.hpp \
    Engine/intfExternalStemmer.hpp \
    Engine/intfBaseExternal.hpp \
    Engine/Engine.h \
    External/CachedDictionary.h \
    External/InteractiveDicAndStemmer.h \
    External/NullDicAndStemmer.h \
    External/GizaBasedDic.h \
    External/GlosbeDic.h \
    External/GoogleTranslate.h \
    External/WordnetStemmer.h \
    Common.h \
    exMWABase.hpp \
    External/Wordreference.h \
    Engine/Knowledge.h \
    Engine/ILA.h
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    main.cpp \
    Engine/Engine.cpp \
    External/CachedDictionary.cpp \
    External/NullDicAndStemmer.cpp \
    External/InteractiveDicAndStemmer.cpp \
    External/GizaBasedDic.cpp \
    External/GlosbeDic.cpp \
    External/GoogleTranslate.cpp \
    External/WordnetStemmer.cpp \
    External/Wordreference.cpp \
    Engine/Knowledge.cpp \
    Engine/ILA.cpp
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
INCLUDEPATH +=

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
QMAKE_CXXFLAGS += -std=c++0x


##########################################################
####          Do Not Change Anything Below            ####
##########################################################
TEMPLATE = app
TARGET = $$ProjectName
DESTDIR = $$BasePath/build
OBJECTS_DIR = $$BasePath/build/obj
MOC_DIR = $$BasePath/build/moc

DepsFile = $$BasePath/deps.pri
include $$DepsFile





