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

#include <QDir>
#include <QFileInfo>
#include "Common.h"
#include "exMWABase.hpp"
#include "Engine/Engine.h"

#define ETS_STR_VALUE(_arg)      #_arg
#define ETS_M2STR(_Macro) ETS_STR_VALUE(_Macro)

using namespace std;

static void
usage(void)
{
    std::cerr<<"Multi word aligner Ver:"<<ETS_M2STR(PROJ_VERSION)<<" Build: "<<__DATE__<<std::endl;
    std::cerr<<"Usage:\nMWA arguments"<<std::endl;
    std::cerr<<"\t-s source\n\t\tSource language file"<<std::endl;
    std::cerr<<"\t-t target\n\t\tTarget language file"<<std::endl;
    std::cerr<<"\t-d dictionary\n\t\tSource to be used as external dictionary. Can be "<<
          wmaPrintable(Engine::instance().validDics().join(", "))<<"(optional) "<<std::endl;
    std::cerr<<"\t-m stemmer\n\t\tSource to be used as external stemmer. Can be "<<
          wmaPrintable(Engine::instance().validDics().join(", "))<<" (optional) "<<std::endl;
    std::cerr<<"\t-D dictionaryArgs\n\t\tSemicolon Separated parameters passed to selected dictionary (optional)"<<std::endl;
    std::cerr<<"\t-M stemmerArgs\n\t\tSemicolon Separated parameters passed to selected Stemmer (optional)"<<std::endl;
    std::cerr<<"\t-o output\n\t\tDirectory path used to store outputs"<<std::endl;
    std::cerr<<"\t-v \n\t\tbe verbose"<<std::endl;
    exit(1);
}

int main(int argc, char* argv[])
{
    char optTag;
    QString SourceLangFile;
    QString TargetLangFile;
    QString ExternalDicArgs;
    QString ExternalStemmerArgs;
    QString OutputDir = "./";
    intfExternalDictionary* ExternalDic = NULL;
    intfExternalStemmer*    ExternalStemmer = NULL;

    while ( (optTag = getopt(argc, argv, "d:m:s:t:o:D:M:v")) != -1) {
        switch (optTag) {
        default:
            std::cerr<<"Bad option "<<optTag<<optarg<<std::endl;
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
                std::cerr<<"Invalid Dic "<<optTag<<optarg<<std::endl;
                usage();
            }
            break;
        case 'm':
            ExternalStemmer = Engine::instance().getStemmerInstance(optarg);
            if (ExternalStemmer == NULL){
                std::cerr<<"Invalid Stemmer "<<optTag<<optarg<<std::endl;
                usage();
            }
        case 'D':
            ExternalDicArgs = optarg;
            break;
        case 'M':
            ExternalStemmerArgs = optarg;
            break;
        case 'o':
            OutputDir = optarg;
            break;
        case 'v':
            gWMAVerbose = true;
            break;
        }
    }

    QFileInfo FileInfo(OutputDir);

    if(OutputDir.isEmpty() && FileInfo.isDir()){
        if (FileInfo.isWritable()){
            std::cerr<<"Unable to use <"<<wmaPrintable(OutputDir)<<"> for writing"<<std::endl;
            return 1;
        }
    }else if (FileInfo.isFile()){
        std::cerr<<"Unable to use file <"<<wmaPrintable(OutputDir)<<"> as directory"<<std::endl;
        return 1;
    }else if (!QDir().mkpath(OutputDir)){
        std::cerr<<"Unable to create path <"<<wmaPrintable(OutputDir)<<">"<<std::endl;
        return 1;
    }

    try{
        Engine::instance().initialize(ExternalDic,
                                      ExternalStemmer,
                                      OutputDir,
                                      SourceLangFile,
                                      TargetLangFile,
                                      ExternalDicArgs,
                                      ExternalStemmerArgs);
    }catch(exMWABase& e){
        std::cerr<<wmaPrintable(e.what())<<std::endl;
    }

    return 0;
}

