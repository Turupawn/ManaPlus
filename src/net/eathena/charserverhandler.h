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

#ifndef NET_EATHENA_CHARSERVERHANDLER_H
#define NET_EATHENA_CHARSERVERHANDLER_H

#include "net/ea/charserverhandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{

/**
 * Deals with incoming messages from the character server.
 */
class CharServerHandler final : public MessageHandler,
                                public Ea::CharServerHandler
{
    public:
        CharServerHandler();

        A_DELETE_COPY(CharServerHandler)

        void handleMessage(Net::MessageIn &msg) override final;

        void chooseCharacter(Net::Character *const character) override final;

        void newCharacter(const std::string &name,
                          const int slot,
                          const Gender::Type gender,
                          const int hairstyle,
                          const int hairColor,
                          const unsigned char race,
                          const uint16_t look,
                          const std::vector<int> &stats) const override final;

        void renameCharacter(const int id,
                             const std::string &newName) override final;

        void deleteCharacter(Net::Character *const character,
                             const std::string &email) override final;

        void switchCharacter() const override final;

        void connect() override final;

        void processCharLogin(Net::MessageIn &msg);

        void processCharMapInfo(Net::MessageIn &msg);

        void processChangeMapServer(Net::MessageIn &msg);

        bool isNeedCreatePin() const A_WARN_UNUSED
        { return mNeedCreatePin; }

        void setNewPincode(const std::string &pin) override final;

        /**
         * Sets the character create dialog. The handler will clean up this
         * dialog when a new character is succesfully created, and will unlock
         * the dialog when a new character failed to be created.
         */
        void setCharCreateDialog(CharCreateDialog *const window)
                                 override final;

        void changeSlot(const int oldSlot, const int newSlot) override final;

        void readPlayerData(Net::MessageIn &msg,
                            Net::Character *const character,
                            const bool) const override final;

    protected:
        static void processPincodeStatus(Net::MessageIn &msg);

        static void processCharLogin2(Net::MessageIn &msg);

        static void processCharCreate(Net::MessageIn &msg);

        static void processCharCheckRename(Net::MessageIn &msg);

        static void processCharRename(Net::MessageIn &msg);

        static void processCharChangeSlot(Net::MessageIn &msg);

        static void processCharDeleteFailed(Net::MessageIn &msg);

    private:
        static std::string mNewName;
        static uint32_t mPinSeed;
        static uint32_t mPinAccountId;
        static uint32_t mRenameId;
        static bool mNeedCreatePin;
};

}  // namespace EAthena

#endif  // NET_EATHENA_CHARSERVERHANDLER_H
