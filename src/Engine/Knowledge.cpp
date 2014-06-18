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

#include "Knowledge.h"
#include <QStringList>
#include "Common.h"

Knowledge* Knowledge::Instance = NULL;

static QString Punctuations(".,:;/\[]{}()*%#@`~\"'<>?!^=+-_");

/***************************************************************************/
class clsTokenGenerator : public intfInputIterator
{
public:

    clsTokenGenerator(const QStringList& _tokens): Tokens(_tokens){
        this->Curr = 0;
        this->Max = 0;
    }

    ColID_t next(){
        if (this->Curr == this->Max){
            this->Max++;
            if (Max > this->Tokens.size())
                return NOT_ASSIGNED;
            Curr = 0;
            return 0;
        }
        return Knowledge::instance().getIDByToken(this->Tokens[Curr++]);
    }

private:
    int Curr;
    int Max;
    const QStringList& Tokens;
};

/***************************************************************************/
void Knowledge::init(const QString &_baseDir, const QString _sourceLang, const QString _targetLang)
{
    this->SequenceDic = new clsASM(); //TODO Maybe its good to enable configuration
    this->LM = new clsASM();//TODO Maybe its good to enable configuration

    this->SeqDicFile = "./seq." + _sourceLang + "2" + _targetLang + ".asm";
    this->LMFile = "./lm." + _sourceLang + "2" + _targetLang + ".asm";
    this->WordIDFile = "./wm.txt";

    this->load(_baseDir);

    this->WordDic = CachedDictionary::instance();
    this->WordDic->init(_baseDir, _sourceLang, _targetLang);
}

quint32 Knowledge::getIDByToken(const QString &_token, bool _addIfNotExist)
{
    if (_token.isEmpty())
        return 0;
    quint32 ID = this->Token2ID.value(_token);
     if(!ID && _addIfNotExist){
         this->MaxWordID++;
         this->Token2ID.insert(_token, MaxWordID);
         this->ID2token.insert(MaxWordID, _token);
         return MaxWordID;
     }
     return ID;
}

QString Knowledge::getTokenByID(quint32 _id)
{
    return this->ID2token.value(_id);
}

QStringList Knowledge::predictNextTokenByDic(const QString &_token, bool _learn)
{
    return this->predictNextByASM(this->SequenceDic, _token, _learn);
}

void Knowledge::add2SequenceDic(const QStringList &_flWords,
                      const QStringList &_slWords)
{
    QStringList Tokens;
    Tokens.append(_flWords);
    Tokens.append(QString(TOKEN_COUNT_PATTERN).arg(_flWords.size()));
    Tokens.append(_slWords);
    this->SequenceDic->execute(&clsTokenGenerator(Tokens));
}

QStringList Knowledge::predictNextTokenByLM(const QString &_token, bool _learn)
{
    return this->predictNextByASM(this->LM, _token, _learn);
}

void Knowledge::add2LM(const QString _phrase)
{
    QStringList Tokens = _phrase.split(" ");
    Tokens.removeAll("");
    Tokens.insert(0, TOKEN_START);
    this->LM->execute(&clsTokenGenerator(Tokens));
}

QStringList Knowledge::lookupDic(const QString &_word)
{
    //Digits will be keep as is
    bool Ok;_word.toDouble(&Ok); if (Ok) return QStringList()<<_word;
    // Punctuation marks will be keep as is
    if (Punctuations.contains(_word))return QStringList()<<_word;

    return this->WordDic->lookup(_word);
}

void Knowledge::add2Dic(const QString _word, const QStringList _translations)
{
    this->WordDic->add(_word, _translations);
}

void Knowledge::save(const QString &_baseDir)
{
    QFile File(_baseDir + this->WordIDFile);
    if (File.open(this->LastIDInFile ? QFile::Append : QFile::WriteOnly))
    {
        QTextStream Dictionary(&File);
        Dictionary.setCodec("UTF-8");

        for (int i=this->LastIDInFile; i< this->ID2token.size(); i++)
            Dictionary<<this->getTokenByID(i+1)<<endl;
    }

    this->SequenceDic->save((_baseDir + this->SeqDicFile).toUtf8().constData());
    this->LM->save((_baseDir + this->LMFile).toUtf8().constData());
}

void Knowledge::load(const QString &_baseDir)
{
    this->MaxWordID=0;
    this->LastIDInFile = 0;
    this->ID2token.clear();
    this->Token2ID.clear();

    QFile File(this->WordIDFile);
    if (File.open(QFile::ReadOnly))
    {
        QTextStream Dictionary(&File);
        Dictionary.setCodec("UTF-8");

        while(!Dictionary.atEnd())
            this->LastIDInFile = this->getIDByToken(Dictionary.readLine());
    }
    this->getIDByToken(TOKEN_START);
    this->getIDByToken(TOKEN_END);
    this->SequenceDic->load((_baseDir + this->SeqDicFile).toUtf8().constData());
    this->LM->load((_baseDir + this->LMFile).toUtf8().constData());
}

Knowledge::Knowledge()
{}

QStringList Knowledge::predictNextByASM(clsASM *_asm, const QString _token, bool _learn)
{
    QStringList PredictedTokens;
    const std::unordered_set<ColID_t>& Prediction =
            _asm->executeOnce(this->getIDByToken(_token),false);
    for(ColID_t Predicted : Prediction)
        PredictedTokens.append(this->getTokenByID(Predicted));
    return PredictedTokens;
}

