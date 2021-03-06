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

#include "net/messagein.h"

#include "net/packetcounters.h"

#include "utils/stringutils.h"

#include "logger.h"

#include "debug.h"

#define MAKEWORD(low, high) \
    (static_cast<uint16_t>((static_cast<unsigned char>(low)) | \
    (static_cast<uint16_t>(static_cast<unsigned char>(high))) << 8))

namespace Net
{

MessageIn::MessageIn(const char *const data, const unsigned int length) :
    mData(data),
    mLength(length),
    mPos(0),
    mId(0),
    mIgnore(false)
{
    PacketCounters::incInPackets();
}

MessageIn::~MessageIn()
{
    if (mLength)
    {
        if (mPos != mLength && mPos != 2)
        {
            logger->log("Wrong actual or planned inbound packet size!");
            logger->log(" packet id: %u 0x%x",
                static_cast<unsigned int>(mId),
                static_cast<unsigned int>(mId));
            logger->log(" planned size: %u", mLength);
            logger->log(" read size: %u", mPos);
        }
    }
    else
    {
        logger->log("Zero packet size: %d", static_cast<int>(mId));
    }
}

unsigned char MessageIn::readUInt8(const char *const str)
{
    unsigned char value = static_cast<unsigned char>(-1);
    if (mPos < mLength)
        value = static_cast<unsigned char>(mData[mPos]);

    DEBUGLOG2("readUInt8:  " + toStringPrint(static_cast<unsigned int>(value)),
        mPos, str);
    mPos += 1;
    PacketCounters::incInBytes(1);
    return value;
}

signed char MessageIn::readInt8(const char *const str)
{
    signed char value = static_cast<signed char>(-1);
    if (mPos < mLength)
        value = static_cast<signed char>(mData[mPos]);

    DEBUGLOG2("readInt8:   " + toStringPrint(static_cast<unsigned int>(
        static_cast<unsigned char>(value))),
        mPos, str);
    mPos += 1;
    PacketCounters::incInBytes(1);
    return value;
}

uint8_t MessageIn::fromServerDirection(const uint8_t serverDir)
{
    // Translate from eAthena format
    switch (serverDir)
    {
        case 0:
            return 1;
        case 1:
            return 3;
        case 2:
            return 2;
        case 3:
            return 6;
        case 4:
            return 4;
        case 5:
            return 12;
        case 6:
            return 8;
        case 7:
            return 9;
        case 8:
            return 8;
        default:
            logger->log("incorrect direction: %d",
                static_cast<int>(serverDir));
            return 0;
    }
}

void MessageIn::readCoordinates(uint16_t &restrict x,
                                uint16_t &restrict y,
                                uint8_t &restrict direction,
                                const char *const str)
{
    if (mPos + 3 <= mLength)
    {
        const char *const data = mData + static_cast<size_t>(mPos);
        uint16_t temp = MAKEWORD(data[1] & 0x00c0, data[0] & 0x00ff);
        x = static_cast<uint16_t>(temp >> 6);
        temp = MAKEWORD(data[2] & 0x00f0, data[1] & 0x003f);
        y = static_cast<uint16_t>(temp >> 4);

        const uint8_t serverDir = static_cast<uint8_t>(data[2] & 0x000f);
        direction = fromServerDirection(serverDir);

        DEBUGLOG2(std::string("readCoordinates: ").append(toString(
            static_cast<int>(x))).append(",").append(toString(
            static_cast<int>(y))).append(",").append(toString(
            static_cast<int>(serverDir))), mPos, str);
    }
    else
    {
        x = 0;
        y = 0;
        direction = 0;
        logger->log("error: wrong readCoordinates packet");
    }
    mPos += 3;
    PacketCounters::incInBytes(3);
}

void MessageIn::readCoordinatePair(uint16_t &restrict srcX,
                                   uint16_t &restrict srcY,
                                   uint16_t &restrict dstX,
                                   uint16_t &restrict dstY,
                                   const char *const str)
{
    if (mPos + 5 <= mLength)
    {
        const char *const data = mData + static_cast<size_t>(mPos);
        uint16_t temp = MAKEWORD(data[3], data[2] & 0x000f);
        dstX = static_cast<uint16_t>(temp >> 2);

        dstY = MAKEWORD(data[4], data[3] & 0x0003);

        temp = MAKEWORD(data[1], data[0]);
        srcX = static_cast<uint16_t>(temp >> 6);

        temp = MAKEWORD(data[2], data[1] & 0x003f);
        srcY = static_cast<uint16_t>(temp >> 4);

        DEBUGLOG2(std::string("readCoordinatePair: ").append(toString(
            static_cast<int>(srcX))).append(",").append(toString(
            static_cast<int>(srcY))).append(" ").append(toString(
            static_cast<int>(dstX))).append(",").append(toString(
            static_cast<int>(dstY))), mPos, str);
    }
    else
    {
        srcX = 0;
        srcY = 0;
        dstX = 0;
        dstY = 0;
        logger->log("error: wrong readCoordinatePair packet");
    }
    mPos += 5;
    PacketCounters::incInBytes(5);
}

void MessageIn::skip(const unsigned int length, const char *const str)
{
    DEBUGLOG2("skip: " + toString(static_cast<int>(length)), mPos, str);
    mPos += length;
    PacketCounters::incInBytes(length);
}

void MessageIn::skipToEnd(const char *const str)
{
    const int diff = static_cast<int>(mLength - mPos);
    if (diff)
    {
        DEBUGLOG2("skip: " + toString(diff), mPos, str);
        mPos = mLength;
        PacketCounters::incInBytes(diff);
    }
}

std::string MessageIn::readString(int length, const char *const dstr)
{
    // Get string length
    if (length < 0)
        length = readInt16("len");

    // Make sure the string isn't erroneous
    if (length < 0 || mPos + length > mLength)
    {
        DEBUGLOG2("readString error", mPos, dstr);
        mPos = mLength + 1;
        return "";
    }

    // Read the string
    const char *const stringBeg = mData + static_cast<size_t>(mPos);
    const char *const stringEnd
        = static_cast<const char *const>(memchr(stringBeg, '\0', length));

    const std::string str(stringBeg, stringEnd
        ? stringEnd - stringBeg : static_cast<size_t>(length));
    DEBUGLOG2("readString: " + str, mPos, dstr);
    mPos += length;
    PacketCounters::incInBytes(length);
    return str;
}

std::string MessageIn::readRawString(int length, const char *const dstr)
{
    // Get string length
    if (length < 0)
        length = readInt16("len");

    // Make sure the string isn't erroneous
    if (length < 0 || mPos + length > mLength)
    {
        mPos = mLength + 1;
        return "";
    }

    // Read the string
    const char *const stringBeg = mData + static_cast<size_t>(mPos);
    const char *const stringEnd
        = static_cast<const char *const>(memchr(stringBeg, '\0', length));
    std::string str(stringBeg, stringEnd
        ? stringEnd - stringBeg : static_cast<size_t>(length));

    DEBUGLOG2("readString: " + str, mPos, dstr);

    if (stringEnd)
    {
        const size_t len2 = static_cast<size_t>(length)
            - (stringEnd - stringBeg) - 1;
        const char *const stringBeg2 = stringEnd + 1;
        const char *const stringEnd2
            = static_cast<const char *const>(memchr(stringBeg2, '\0', len2));
        const std::string hiddenPart = std::string(stringBeg2,
            stringEnd2 ? stringEnd2 - stringBeg2 : len2);
        if (hiddenPart.length() > 0)
        {
            DEBUGLOG2("readString2: " + hiddenPart, mPos, dstr);
            return str.append("|").append(hiddenPart);
        }
    }
    mPos += length;
    PacketCounters::incInBytes(length);

    return str;
}

unsigned char *MessageIn::readBytes(int length, const char *const dstr)
{
    // Get string length
    if (length < 0)
        length = readInt16("len");

    // Make sure the string isn't erroneous
    if (length < 0 || mPos + length > mLength)
    {
        DEBUGLOG2("readBytesString error", mPos, dstr);
        mPos = mLength + 1;
        return nullptr;
    }

    unsigned char *const buf
        = new unsigned char[static_cast<size_t>(length + 2)];

    memcpy(buf, mData + static_cast<size_t>(mPos), length);
    buf[length] = 0;
    buf[length + 1] = 0;
    mPos += length;

#ifdef ENABLEDEBUGLOG
    std::string str;
    for (int f = 0; f < length; f ++)
        str.append(strprintf("%02x", static_cast<unsigned>(buf[f])));
    str += " ";
    for (int f = 0; f < length; f ++)
    {
        if (buf[f])
            str.append(strprintf("%c", buf[f]));
        else
            str.append("_");
    }
    if (dstr)
        logger->dlog(dstr);
    logger->dlog("ReadBytes: " + str);
#endif

    PacketCounters::incInBytes(length);
    return buf;
}

}  // namespace Net
