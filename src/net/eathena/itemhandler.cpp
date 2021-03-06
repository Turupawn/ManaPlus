/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#include "net/eathena/itemhandler.h"

#include "actormanager.h"

#include "net/eathena/protocol.h"

#include "debug.h"

namespace EAthena
{

ItemHandler::ItemHandler() :
    MessageHandler(),
    Ea::ItemHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_ITEM_VISIBLE,
        SMSG_ITEM_DROPPED,
        SMSG_ITEM_REMOVE,
        SMSG_GRAFFITI_VISIBLE,
        0
    };
    handledMessages = _messages;
}

void ItemHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_ITEM_VISIBLE:
            processItemVisible(msg);
            break;

        case SMSG_ITEM_DROPPED:
            processItemDropped(msg);
            break;

        case SMSG_ITEM_REMOVE:
            processItemRemove(msg);
            break;

        case SMSG_GRAFFITI_VISIBLE:
            processGraffiti(msg);
            break;

        default:
            break;
    }
}

void ItemHandler::processItemDropped(Net::MessageIn &msg)
{
    const int id = msg.readInt32("id");
    const int itemId = msg.readInt16("item id");
    msg.readInt16("type");
    const uint8_t identify = msg.readUInt8("identify");
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    const int subX = static_cast<int>(msg.readInt8("subx"));
    const int subY = static_cast<int>(msg.readInt8("suby"));
    const int amount = msg.readInt16("count");

    if (actorManager)
    {
        actorManager->createItem(id, itemId,
            x, y, amount, identify, subX, subY);
    }
}

void ItemHandler::processGraffiti(Net::MessageIn &msg)
{
    msg.readInt32("graffiti id");
    msg.readInt32("creator id");
    msg.readInt16("x");
    msg.readInt16("y");
    msg.readUInt8("job");
    msg.readUInt8("visible");
    msg.readUInt8("is content");
    msg.readString(80, "text");
}

}  // namespace EAthena
