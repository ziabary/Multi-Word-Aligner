/*************************************************************************
 * MWA - A Multi word alignerc
 * Copyright (C) 2012-2014  S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *************************************************************************/
/**
 @author S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
 */

#include <unistd.h>

#include "Engine/Engine.h"
#include <iostream>
#include <QDir>
#include <QFileInfo>

#define ETS_STR_VALUE(_arg)      #_arg
#define ETS_M2STR(_Macro) ETS_STR_VALUE(_Macro)

using namespace std;

static void
usage(void)
{
    cerr<<"Multi word aligner Ver:"<<ETS_M2STR(PROJ_VERSION)<<" Build: "<<__DATE__<<endl;
    cerr<<"Usage:\nMWA arguments"<<endl;
    cerr<<"\t-s source\n\t\tSource language file"<<endl;
    cerr<<"\t-t target\n\t\tTarget language file"<<endl;
    cerr<<"\t-d dictionary\n\t\tSource to be used as external dictionary. Can be "<<Engine::instance().validDics().join(", ").toUtf8().constData()<<"(optional) "<<endl;
    cerr<<"\t-m stemmer\n\t\tSource to be used as external stemmer. Can be "<<Engine::instance().validDics().join(", ").toUtf8().constData()<<" (optional) "<<endl;
    cerr<<"\t-o output\n\t\tDirectory path used to store outputs"<<endl;
    exit(1);
}

int main(int argc, char* argv[])
{
    char optTag;
    QString SourceLangFile;
    QString TargetLangFile;
    QString OutputDir = "./";
    intfExternalDictionary* ExternalDic = NULL;
    intfExternalStemmer*    ExternalStemmer = NULL;

    while ( (optTag = getopt(argc, argv, "d:m:s:t:o:")) != -1) {
        switch (optTag) {
        default:
            cerr<<"Bad option "<<optTag<<optarg<<endl;
            usage();
            break;
        case 's':
            SourceLangFile = optarg;
            break;
        case 't':
            TargetLangFile = optarg;
            break;
        case 'd':
            ExternalDic = Engine::instance().getDicInstance(optarg);
            if (ExternalDic == NULL){
                cerr<<"Invalid Dic "<<optTag<<optarg<<endl;
                usage();
            }
            break;
        case 'm':
            ExternalStemmer = Engine::instance().getStemmerInstance(optarg);
            if (ExternalStemmer == NULL){
                cerr<<"Invalid Stemmer "<<optTag<<optarg<<endl;
                usage();
            }
        case 'o':
            OutputDir = optarg;
            break;
        }
    }

    QFileInfo FileInfo(OutputDir);

    if(OutputDir.isEmpty() && FileInfo.isDir()){
        if (FileInfo.isWritable()){
            cerr<<"Unable to use <"<<OutputDir.toUtf8().constData()<<"> for writing"<<endl;
            return 1;
        }
    }else if (FileInfo.isFile()){
        cerr<<"Unable to use file <"<<OutputDir.toUtf8().constData()<<"> as directory"<<endl;
        return 1;
    }else if (!QDir().mkpath(OutputDir)){
        cerr<<"Unable to create path <"<<OutputDir.toUtf8().constData()<<">"<<endl;
        return 1;
    }

    if (!Engine::instance().initialize(ExternalDic, ExternalStemmer, OutputDir, SourceLangFile, TargetLangFile)){
        return 1;
    }

    return 0;
}

