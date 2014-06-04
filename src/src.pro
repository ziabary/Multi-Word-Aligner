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
VERSION = 0.1.0

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
Dependencies += ASM curl
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    Engine/intfExternalDictionary.hpp \
    Engine/intfExternalStemmer.hpp \
    Engine/intfBaseExternal.hpp \
    Engine/Engine.h \
    External/CachedDictionary.h \
    External/InteractiveDicAndStemmer.h \
    External/NullDicAndStemmer.h \
    External/JSON/JSONArray.h \
    External/JSON/JSONDocument.h \
    External/JSON/JSONObject.h \
    External/JSON/JSONParseError.h \
    External/JSON/JSONValue.h \
    External/JSON/Private/qjson_p.h \
    External/JSON/Private/qjsonparser_p.h \
    External/JSON/Private/qjsonwriter_p.h \
    External/GizaBasedDic.h \
    External/GlosbeDic.h \
    External/GoogleTranslate.h \
    External/WordnetStemmer.h \
    Common.h \
    exMWABase.hpp \
    External/Wordreference.h
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    main.cpp \
    Engine/Engine.cpp \
    External/CachedDictionary.cpp \
    External/NullDicAndStemmer.cpp \
    External/InteractiveDicAndStemmer.cpp \
    External/JSON/Private/qjson.cpp \
    External/JSON/Private/qjsonarray.cpp \
    External/JSON/Private/qjsondocument.cpp \
    External/JSON/Private/qjsonobject.cpp \
    External/JSON/Private/qjsonparser.cpp \
    External/JSON/Private/qjsonvalue.cpp \
    External/JSON/Private/qjsonwriter.cpp \
    External/GizaBasedDic.cpp \
    External/GlosbeDic.cpp \
    External/GoogleTranslate.cpp \
    External/WordnetStemmer.cpp \
    External/Wordreference.cpp
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
INCLUDEPATH +=

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
TEMPLATE = app
TARGET = $$ProjectName
DESTDIR = $$BasePath/build
OBJECTS_DIR = $$BasePath/build/obj
MOC_DIR = $$BasePath/build/moc

QT += core
QT -= gui

CONFIG(debug, debug|release): DEFINES += MWA_SHOW_DEBUG=1
DEFINES += PROJ_VERSION=$$VERSION

INCLUDEPATH  +=  $$ASM_INCLUDE $$WN_INCLUDE
QMAKE_LIBDIR +=  $$ASM_LIBPATH $$WN_LIBPATH
LIBS += -lASM -lcurl -lWN

!exists($$WN_LIBPATH"/libWN.a") : !exists($$WN_LIBPATH"/libWN.so") {
    message("Wordnet not found disabling it.")
    DEFINES -= USE_WN
    LIBS -= -lWN
}






