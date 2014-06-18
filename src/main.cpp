/*************************************************************************
 * MWA - A Multi word alignerc
 * Copyright (C) 2012-2014  S.Mohammad M. Ziabary <mehran.m@aut.ac.ir>
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
 @author S.Mohammad M. Ziabary <mehran.m@aut.ac.ir>
 */

#include <unistd.h>

#include <QDir>
#include <QFileInfo>
#include <QTextCodec>
#include "Common.h"
#include "exMWABase.hpp"
#include "Engine/Engine.h"

#define ETS_STR_VALUE(_arg)      #_arg
#define ETS_M2STR(_Macro) ETS_STR_VALUE(_Macro)

#define DEFAULT_SOURCE_LANGUAE "en"
#define DEFAULT_TARGET_LANGUAE "es"

stuConfigs WMAConfigs;

#include <QDebug>
static void
usage(void)
{
    std::cerr<<"Multi word aligner Ver:"<<ETS_M2STR(PROJ_VERSION)<<" Build: "<<__DATE__<<std::endl;
    std::cerr<<"Usage:\nMWA [options] SourcePath TargetPath"<<std::endl;
    std::cerr<<"\t-s sourceLang\n\t\tSource language (ISO639) (Default: "<<DEFAULT_SOURCE_LANGUAE<<")"<<std::endl;
    std::cerr<<"\t-t targetLag\n\t\tTarget language (ISO639) (Default: "<<DEFAULT_TARGET_LANGUAE<<")"<<std::endl;
    std::cerr<<"\t-d dictionary\n\t\tSource to be used as external dictionary. Can be "<<
          wmaPrintable(Engine::instance().validDics().join(", "))<<" (optional) "<<std::endl;
    std::cerr<<"\t-m stemmer\n\t\tSource to be used as external stemmer. Can be "<<
          wmaPrintable(Engine::instance().validStemmers().join(", "))<<" (optional) "<<std::endl;
    std::cerr<<"\t-D dictionaryArgs\n\t\tSemicolon Separated parameters passed to selected dictionary (optional)"<<std::endl;
    std::cerr<<"\t-M stemmerArgs\n\t\tSemicolon Separated parameters passed to selected Stemmer (optional)"<<std::endl;
    std::cerr<<"\t-o output\n\t\tDirectory path used to store outputs"<<std::endl;
    std::cerr<<"\t-v \n\t\tbe verbose"<<std::endl;
    exit(1);
}

int main(int argc, char* argv[])
{
    char optTag;
    QString SourceLanguge = DEFAULT_SOURCE_LANGUAE;
    QString TargetLanguage = DEFAULT_TARGET_LANGUAE;
    QString ExternalDicArgs;
    QString ExternalStemmerArgs;
    QString OutputDir = "./";
    intfExternalDictionary* ExternalDic = NULL;
    intfExternalStemmer*    ExternalStemmer = NULL;

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF8"));

    while ( (optTag = getopt(argc, argv, "d:m:s:t:o:D:M:vh")) != -1) {
        switch (optTag) {
        default:
            std::cerr<<"Bad option "<<optTag<<optarg<<std::endl;
            usage();
            break;
        case 's':
            SourceLanguge = optarg;
            break;
        case 't':
            TargetLanguage = optarg;
            break;
        case 'd':
            ExternalDic = Engine::instance().getDicInstance(optarg);
            if (ExternalDic == NULL){
                std::cerr<<"Invalid Dic: "<<optarg<<std::endl;
                usage();
            }
            break;
        case 'm':
            ExternalStemmer = Engine::instance().getStemmerInstance(optarg);
            if (ExternalStemmer == NULL){
                std::cerr<<"Invalid Stemmer: "<<optarg<<std::endl;
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
            WMAConfigs.WMAVerbose = true;
            wmaDebug<<"Switched to verbose"<<endl;
            break;
        case 'h':
            usage();
            break;
        }
    }

    if (optind + 2 > argc){
        std::cerr<<"No file specified as iput"<<std::endl;
        usage();
    }

    const char* SourceLangFile = argv[optind];
    const char* TargetLangFile = argv[optind + 1];

    QFileInfo FileInfo(OutputDir);

    if(OutputDir.isEmpty()){
        std::cerr<<"Unable to use <> for writing"<<std::endl;
        return 1;
/*    }else if (FileInfo.isDir() && FileInfo.permissions()){
        std::cerr<<"Unable to use <"<<wmaPrintable(OutputDir)<<"> for writing"<<std::endl;
        return 1;*/
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
                                      SourceLanguge,
                                      TargetLanguage,
                                      ExternalDicArgs,
                                      ExternalStemmerArgs);
        Engine::instance().process(SourceLangFile, TargetLangFile);
    }catch(exMWABase& e){
        std::cerr<<e.what().toStdString().c_str()<<std::endl;
    }
    catch(...){
        std::cerr<<"Fatal Exception"<<std::endl;
    }

    return 0;
}

