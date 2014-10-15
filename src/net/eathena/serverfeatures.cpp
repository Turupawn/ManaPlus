/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
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

#include "net/eathena/serverfeatures.h"

#include "debug.h"

extern Net::ServerFeatures *serverFeatures;

namespace EAthena
{

ServerFeatures::ServerFeatures()
{
    serverFeatures = this;
}

bool ServerFeatures::haveServerOnlineList() const
{
    return false;
}

bool ServerFeatures::haveOnlineList() const
{
    return false;
}

bool ServerFeatures::havePartyNickInvite() const
{
    return true;
}

bool ServerFeatures::haveChangePartyLeader() const
{
    return true;
}

bool ServerFeatures::haveServerHp() const
{
    return true;
}

bool ServerFeatures::haveLangTab() const
{
    return false;
}

bool ServerFeatures::havePlayerStatusUpdate() const
{
    return false;
}

bool ServerFeatures::haveBrokenPlayerAttackDistance() const
{
    return false;
}

bool ServerFeatures::haveNativeGuilds() const
{
    return true;
}

bool ServerFeatures::haveIncompleteChatMessages() const
{
    return false;
}

bool ServerFeatures::haveRaceSelection() const
{
    return false;
}

bool ServerFeatures::haveLookSelection() const
{
    return false;
}

bool ServerFeatures::haveSpecialChatChannels() const
{
    return false;
}

bool ServerFeatures::haveChatChannels() const
{
    return true;
}

bool ServerFeatures::haveServerIgnore() const
{
    return true;
}

bool ServerFeatures::haveMove3() const
{
    return false;
}

bool ServerFeatures::haveItemColors() const
{
    return false;
}

bool ServerFeatures::haveOtherGender() const
{
    return false;
}

bool ServerFeatures::haveMonsterAttackRange() const
{
    return false;
}

bool ServerFeatures::haveEmailOnRegister() const
{
    return false;
}

bool ServerFeatures::haveEightDirections() const
{
    return true;
}

bool ServerFeatures::haveCharRename() const
{
    return true;
}

bool ServerFeatures::haveMonsterName() const
{
    return true;
}

bool ServerFeatures::haveBankApi() const
{
    return true;
}

}  // namespace EAthena
