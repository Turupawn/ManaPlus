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

#include "net/eathena/loginhandler.h"

#include "client.h"
#include "logger.h"

#include "gui/windows/logindialog.h"

#include "net/serverfeatures.h"

#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/protocol.h"

#include "utils/gettext.h"

#include "debug.h"

extern Net::LoginHandler *loginHandler;

namespace EAthena
{

extern ServerInfo charServer;

LoginHandler::LoginHandler() :
    MessageHandler(),
    Ea::LoginHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_LOGIN_DATA,
        SMSG_LOGIN_ERROR,
        SMSG_LOGIN_ERROR2,
        SMSG_SERVER_VERSION_RESPONSE,
        SMSG_UPDATE_HOST,
        0
    };
    handledMessages = _messages;
    loginHandler = this;
}

LoginHandler::~LoginHandler()
{
}

void LoginHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_LOGIN_DATA:
            processLoginData(msg);
            break;

        case SMSG_LOGIN_ERROR:
            processLoginError(msg);
            break;

        case SMSG_LOGIN_ERROR2:
            processLoginError2(msg);
            break;

        case SMSG_SERVER_VERSION_RESPONSE:
            processServerVersion(msg);
            break;

        case SMSG_UPDATE_HOST:
            processUpdateHost(msg);
            break;

        default:
            break;
    }
}

void LoginHandler::connect()
{
    if (!mNetwork)
        return;

    mNetwork->connect(mServer);
    if (serverFeatures->haveServerVersion())
    {
        sendVersion();
    }
    else
    {
        if (client->getState() != STATE_LOGIN)
            client->setState(STATE_LOGIN);
    }
}

bool LoginHandler::isConnected() const
{
    if (!mNetwork)
        return false;

    return mNetwork->isConnected();
}

void LoginHandler::disconnect()
{
    if (mNetwork && mNetwork->getServer() == mServer)
        mNetwork->disconnect();
}

void LoginHandler::changePassword(const std::string &restrict username
                                  A_UNUSED,
                                  const std::string &restrict
                                  oldPassword A_UNUSED,
                                  const std::string &restrict
                                  newPassword A_UNUSED) const
{
}

void LoginHandler::sendLoginRegister(const std::string &restrict username,
                                     const std::string &restrict password,
                                     const std::string &restrict email) const
{
    if (email.empty())
    {
        createOutPacket(CMSG_LOGIN_REGISTER);
        outMsg.writeInt32(20, "client version");
        outMsg.writeString(username, 24, "login");
        outMsg.writeStringNoLog(password, 24, "password");
        outMsg.writeInt8(0x03, "client type");
    }
    else
    {
        createOutPacket(CMSG_LOGIN_REGISTER2);
        outMsg.writeString(username, 24, "login");
        outMsg.writeStringNoLog(password, 24, "password");
        outMsg.writeInt8(0x03, "client type");
        outMsg.writeString(email, 40, "email");
    }
}

ServerInfo *LoginHandler::getCharServer() const
{
    return &charServer;
}

void LoginHandler::processLoginError2(Net::MessageIn &msg)
{
    const uint32_t code = msg.readInt32("error");
    logger->log("Login::error code: %u", code);

    switch (code)
    {
        case 0:
            // TRANSLATORS: error message
            errorMessage = _("Unregistered ID.");
            break;
        case 1:
            // TRANSLATORS: error message
            errorMessage = _("Wrong password.");
            LoginDialog::savedPassword.clear();
            break;
        case 2:
            // TRANSLATORS: error message
            errorMessage = _("Account expired.");
            break;
        case 3:
            // TRANSLATORS: error message
            errorMessage = _("Rejected from server.");
            break;
        case 4:
            // TRANSLATORS: error message
            errorMessage = _("You have been permanently banned from "
                              "the game. Please contact the GM team.");
            break;
        case 5:
            // TRANSLATORS: error message
            errorMessage = _("Client too old.");
            break;
        case 6:
            // TRANSLATORS: error message
            errorMessage = strprintf(_("You have been temporarily "
                                        "banned from the game until "
                                        "%s.\nPlease contact the GM "
                                        "team via the forums."),
                                        msg.readString(20, "date").c_str());
            break;
        case 7:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("Server overpopulated.");
            break;
        case 9:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("This user name is already taken.");
            break;
        case 10:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("Wrong name.");
            break;
        case 11:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("Incorrect email.");
            break;
        case 99:
            // look like unused
            // TRANSLATORS: error message
            errorMessage = _("Username permanently erased.");
            break;
        default:
            // TRANSLATORS: error message
            errorMessage = _("Unknown error.");
            break;
    }
    client->setState(STATE_ERROR);
}

void LoginHandler::processUpdateHost2(Net::MessageIn &msg A_UNUSED)
{
}

void LoginHandler::sendVersion() const
{
    createOutPacket(CMSG_SERVER_VERSION_REQUEST);
    outMsg.writeInt32(CLIENT_PROTOCOL_VERSION, "protocol version");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
    outMsg.writeInt32(0, "unused");
}

void LoginHandler::processServerVersion(Net::MessageIn &msg)
{
    msg.readInt16("len");
    msg.readInt32("unused");
    serverVersion = msg.readInt32("server version");
    if (serverVersion > 0)
        logger->log("Evol2 server version: %d", serverVersion);
    else
        logger->log("Hercules without version");
    client->setState(STATE_LOGIN);
}

int LoginHandler::supportedOptionalActions() const
{
    return serverFeatures->haveEmailOnRegister()
        ? Net::RegistrationOptions::SetEmailOnRegister
        | Net::RegistrationOptions::SetGenderOnRegister
        : Net::RegistrationOptions::SetGenderOnRegister;
}

}  // namespace EAthena
