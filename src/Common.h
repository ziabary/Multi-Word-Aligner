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

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
using namespace std;
#include <QString>

extern struct stuConfigs{
    bool OnlineLearn;
    bool WMAVerbose;
    stuConfigs(){
        this->OnlineLearn = false;
        this->WMAVerbose = false;
    }
} WMAConfigs;

#define wmaDebug if(WMAConfigs.WMAVerbose) std::cout
#define wmaPrintable(_str) _str.toUtf8().constData()

static void dummy(){
    Q_UNUSED(dummy)
    Q_UNUSED(WMAConfigs)
}

#define TOKEN_START " #S# "
#define TOKEN_END " #E# "
#define TOKEN_COUNT_PATTERN " #%1# "


#endif // COMMON_H
