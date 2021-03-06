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

#include "net/eathena/buysellhandler.h"

#include "notifymanager.h"

#include "being/playerinfo.h"

#include "enums/being/attributes.h"

#include "gui/windows/buydialog.h"

#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include "debug.h"

extern Net::BuySellHandler *buySellHandler;

namespace EAthena
{

BuySellHandler::BuySellHandler() :
    MessageHandler(),
    Ea::BuySellHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_NPC_BUY_SELL_CHOICE,
        SMSG_NPC_BUY,
        SMSG_NPC_SELL,
        SMSG_NPC_BUY_RESPONSE,
        SMSG_NPC_SELL_RESPONSE,
        0
    };
    handledMessages = _messages;
    buySellHandler = this;
    mBuyDialog = nullptr;
}

void BuySellHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_NPC_BUY_SELL_CHOICE:
            processNpcBuySellChoice(msg);
            break;

        case SMSG_NPC_BUY:
            processNpcBuy(msg);
            break;

        case SMSG_NPC_SELL:
            processNpcSell(msg);
            break;

        case SMSG_NPC_BUY_RESPONSE:
            processNpcBuyResponse(msg);
            break;

        case SMSG_NPC_SELL_RESPONSE:
            processNpcSellResponse(msg);
            break;

        default:
            break;
    }
}

void BuySellHandler::processNpcBuy(Net::MessageIn &msg)
{
    msg.readInt16("len");
    const int sz = 11;
    const int n_items = (msg.getLength() - 4) / sz;
    mBuyDialog = new BuyDialog(mNpcId);
    mBuyDialog->setMoney(PlayerInfo::getAttribute(Attributes::MONEY));

    for (int k = 0; k < n_items; k++)
    {
        const int value = msg.readInt32("price");
        msg.readInt32("dc value?");
        const int type = msg.readUInt8("type");
        const int itemId = msg.readInt16("item id");
        const unsigned char color = 1;
        mBuyDialog->addItem(itemId, type, color, 0, value);
    }
    mBuyDialog->sort();
}

void BuySellHandler::processNpcSellResponse(Net::MessageIn &msg)
{
    switch (msg.readUInt8("result"))
    {
        case 0:
            NotifyManager::notify(NotifyTypes::SOLD);
            break;
        case 1:
        default:
            NotifyManager::notify(NotifyTypes::SELL_FAILED);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::SELL_TRADE_FAILED);
            break;
        case 3:
            NotifyManager::notify(NotifyTypes::SELL_UNSELLABLE_FAILED);
            break;
    }
}

}  // namespace EAthena
