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

#ifndef NET_TMWA_INVENTORYHANDLER_H
#define NET_TMWA_INVENTORYHANDLER_H

#include "net/ea/inventoryhandler.h"

#include "net/tmwa/messagehandler.h"

namespace TmwAthena
{

class InventoryHandler final : public MessageHandler,
                               public Ea::InventoryHandler
{
    public:
        InventoryHandler();

        A_DELETE_COPY(InventoryHandler)

        ~InventoryHandler();

        void handleMessage(Net::MessageIn &msg) override final;

        void equipItem(const Item *const item) const override final;

        void unequipItem(const Item *const item) const override final;

        void useItem(const Item *const item) const override final;

        void dropItem(const Item *const item,
                      const int amount) const override final;

        void closeStorage(const int type) const override final;

        void moveItem2(const int source,
                       const int slot,
                       const int amount,
                       const int destination) const override final;

        void useCard(const int index) const override final;

        void insertCard(const int cardIndex,
                        const int itemIndex) const override final;

        void favoriteItem(const Item *const item,
                          const bool favorite) const override final;

        void selectEgg(const Item *const item) const override final;

        int convertFromServerSlot(const int serverSlot)
                                  const override final A_WARN_UNUSED;

        int getProjectileSlot() const override final
        { return 10; }

    protected:
        static void processPlayerEquipment(Net::MessageIn &msg);

        static void processPlayerInventoryAdd(Net::MessageIn &msg);

        static void processPlayerInventory(Net::MessageIn &msg);

        static void processPlayerStorage(Net::MessageIn &msg);

        static void processPlayerEquip(Net::MessageIn &msg);

        static void processPlayerUnEquip(Net::MessageIn &msg);

        static void processPlayerStorageEquip(Net::MessageIn &msg);

        static void processPlayerStorageAdd(Net::MessageIn &msg);

        static void processPlayerStorageRemove(Net::MessageIn &msg);
};

}  // namespace TmwAthena

#endif  // NET_TMWA_INVENTORYHANDLER_H
