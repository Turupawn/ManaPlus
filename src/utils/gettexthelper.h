/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILS_GETTEXTHELPER_H
#define UTILS_GETTEXTHELPER_H

#include "localconsts.h"

class GettextHelper final
{
    public:
        static void initLang();

    private:
#ifdef ENABLE_NLS
        static void bindTextDomain(const char *const path);
#else
        static void bindTextDomain(const char *const path A_UNUSED);
#endif
};

#endif  // UTILS_GETTEXTHELPER_H
