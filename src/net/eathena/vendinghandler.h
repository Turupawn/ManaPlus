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

#ifndef NET_EATHENA_VENDINGHANDLER_H
#define NET_EATHENA_VENDINGHANDLER_H

#include "net/vendinghandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{
class VendingHandler final : public MessageHandler,
                             public Net::VendingHandler
{
    public:
        VendingHandler();

        A_DELETE_COPY(VendingHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void close() const override final;

        void open(const Being *const being) const override final;

        void buy(const Being *const being,
                 const int index,
                 const int amount) const override final;

        void buy2(const Being *const being,
                  const int vendId,
                  const int index,
                  const int amount) const override final;

        void createShop(const std::string &name,
                        const bool flag,
                        std::vector<ShopItem*> &items) const override final;

    protected:
        static void processOpenReq(Net::MessageIn &msg);

        static void processShowBoard(Net::MessageIn &msg);

        static void processHideBoard(Net::MessageIn &msg);

        static void processItemsList(Net::MessageIn &msg);

        static void processBuyAck(Net::MessageIn &msg);

        static void processOpen(Net::MessageIn &msg);

        static void processReport(Net::MessageIn &msg);
};

}  // namespace EAthena

#endif  // NET_EATHENA_VENDINGHANDLER_H
