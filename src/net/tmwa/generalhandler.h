/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_TMWA_GENERALHANDLER_H
#define NET_TMWA_GENERALHANDLER_H

#include "net/generalhandler.h"

#include "net/tmwa/messagehandler.h"

namespace TmwAthena
{

class ServerFeatures;

class GeneralHandler final : public MessageHandler,
                             public Net::GeneralHandler
{
    public:
        GeneralHandler();

        A_DELETE_COPY(GeneralHandler)

        ~GeneralHandler();

        void handleMessage(Net::MessageIn &msg) override final;

        void load() override final;

        void reload() override final;

        void unload() override final;

        void flushNetwork() override final;

        void clearHandlers() override final;

        void reloadPartially() const override final;

        void gameStarted() const override final;

        void gameEnded() const override final;

    protected:
        static void processConnectionProblem(Net::MessageIn &msg);

        MessageHandlerPtr mAdminHandler;
        MessageHandlerPtr mBeingHandler;
        MessageHandlerPtr mBuySellHandler;
        MessageHandlerPtr mCharServerHandler;
        MessageHandlerPtr mChatHandler;
        MessageHandlerPtr mGameHandler;
        MessageHandlerPtr mGuildHandler;
        MessageHandlerPtr mInventoryHandler;
        MessageHandlerPtr mItemHandler;
        MessageHandlerPtr mLoginHandler;
        MessageHandlerPtr mNpcHandler;
        MessageHandlerPtr mPartyHandler;
        MessageHandlerPtr mPetHandler;
        MessageHandlerPtr mPlayerHandler;
        MessageHandlerPtr mSkillHandler;
        MessageHandlerPtr mTradeHandler;
        MessageHandlerPtr mQuestHandler;
        ServerFeatures *mServerFeatures;
        MessageHandlerPtr mMailHandler;
        MessageHandlerPtr mAuctionHandler;
        MessageHandlerPtr mCashShopHandler;
        MessageHandlerPtr mFamilyHandler;
        MessageHandlerPtr mBankHandler;
        MessageHandlerPtr mMercenaryHandler;
        MessageHandlerPtr mBuyingStoreHandler;
        MessageHandlerPtr mHomunculusHandler;
        MessageHandlerPtr mFriendsHandler;
        MessageHandlerPtr mElementalHandler;
        MessageHandlerPtr mMarketHandler;
        MessageHandlerPtr mVendingHandler;
        MessageHandlerPtr mSearchStoreHandler;
};

}  // namespace TmwAthena

#endif  // NET_TMWA_GENERALHANDLER_H
