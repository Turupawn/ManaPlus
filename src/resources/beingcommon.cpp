/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2015  The ManaPlus Developers
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

#include "resources/beingcommon.h"

#include "configuration.h"
#include "logger.h"

#include "utils/files.h"

#include "resources/beinginfo.h"
#include "resources/spritereference.h"

#include <algorithm>

#include "debug.h"

void BeingCommon::readBasicAttributes(BeingInfo *const info,
                                      XmlNodePtrConst node,
                                      const std::string &hoverCursor)
{
    info->setTargetCursorSize(XML::getProperty(node,
        "targetCursor", "medium"));

    info->setHoverCursor(XML::getProperty(node, "hoverCursor", hoverCursor));

    info->setTargetOffsetX(XML::getProperty(node, "targetOffsetX", 0));
    info->setTargetOffsetY(XML::getProperty(node, "targetOffsetY", 0));

    info->setNameOffsetX(XML::getProperty(node, "nameOffsetX", 0));
    info->setNameOffsetY(XML::getProperty(node, "nameOffsetY", 0));
    info->setSortOffsetY(XML::getProperty(node, "sortOffsetY", 0));

    info->setHpBarOffsetX(XML::getProperty(node, "hpBarOffsetX", 0));
    info->setHpBarOffsetY(XML::getProperty(node, "hpBarOffsetY", 0));
}

void BeingCommon::getIncludeFiles(const std::string &dir,
                                  StringVect &list,
                                  const std::string &ext)
{
    const std::string path = dir + "/";
    StringVect tempList;
    Files::getFilesWithDir(path, tempList);
    FOR_EACH (StringVectCIter, it, tempList)
    {
        const std::string &str = *it;
        if (findLast(str, ext))
            list.push_back(str);
    }
    std::sort(list.begin(), list.end());
}

bool BeingCommon::readObjectNodes(XmlNodePtrConst &spriteNode,
                                  SpriteDisplay &display,
                                  BeingInfo *const currentInfo,
                                  const std::string &dbName)
{
    if (xmlNameEqual(spriteNode, "sprite"))
    {
        if (!spriteNode->xmlChildrenNode)
            return true;

        SpriteReference *const currentSprite = new SpriteReference;
        currentSprite->sprite = reinterpret_cast<const char*>(
            spriteNode->xmlChildrenNode->content);

        currentSprite->variant = XML::getProperty(
            spriteNode, "variant", 0);
        display.sprites.push_back(currentSprite);
        return true;
    }
    else if (xmlNameEqual(spriteNode, "sound"))
    {
        if (!spriteNode->xmlChildrenNode)
            return true;

        const std::string event = XML::getProperty(
            spriteNode, "event", "");
        const int delay = XML::getProperty(
            spriteNode, "delay", 0);
        const char *const filename = reinterpret_cast<const char*>(
            spriteNode->xmlChildrenNode->content);

        if (event == "hit")
        {
            currentInfo->addSound(ItemSoundEvent::HIT, filename, delay);
        }
        else if (event == "miss")
        {
            currentInfo->addSound(ItemSoundEvent::MISS, filename, delay);
        }
        else if (event == "hurt")
        {
            currentInfo->addSound(ItemSoundEvent::HURT, filename, delay);
        }
        else if (event == "die")
        {
            currentInfo->addSound(ItemSoundEvent::DIE, filename, delay);
        }
        else if (event == "move")
        {
            currentInfo->addSound(ItemSoundEvent::MOVE, filename, delay);
        }
        else if (event == "sit")
        {
            currentInfo->addSound(ItemSoundEvent::SIT, filename, delay);
        }
        else if (event == "sittop")
        {
            currentInfo->addSound(ItemSoundEvent::SITTOP, filename, delay);
        }
        else if (event == "spawn")
        {
            currentInfo->addSound(ItemSoundEvent::SPAWN, filename, delay);
        }
        else
        {
            logger->log((dbName + ": Warning, sound effect %s for "
                "unknown event %s of monster %s").c_str(),
                filename, event.c_str(),
                currentInfo->getName().c_str());
        }
        return true;
    }
    else if (xmlNameEqual(spriteNode, "attack"))
    {
        const int attackId = XML::getProperty(spriteNode, "id", 0);
        const int effectId = XML::getProperty(spriteNode, "effect-id",
            paths.getIntValue("effectId"));
        const int hitEffectId = XML::getProperty(spriteNode, "hit-effect-id",
            paths.getIntValue("hitEffectId"));
        const int criticalHitEffectId = XML::getProperty(spriteNode,
            "critical-hit-effect-id",
            paths.getIntValue("criticalHitEffectId"));
        const int missEffectId = XML::getProperty(spriteNode, "miss-effect-id",
            paths.getIntValue("missEffectId"));

        const std::string spriteAction = XML::getProperty(spriteNode, "action",
            "attack");
        const std::string skySpriteAction = XML::getProperty(spriteNode,
            "skyaction", "skyattack");
        const std::string waterSpriteAction = XML::getProperty(spriteNode,
            "wateraction", "waterattack");

        const std::string missileParticle = XML::getProperty(spriteNode,
            "missile-particle", "");

        currentInfo->addAttack(attackId, spriteAction, skySpriteAction,
            waterSpriteAction, effectId, hitEffectId,
            criticalHitEffectId, missEffectId, missileParticle);
        return true;
    }
    else if (xmlNameEqual(spriteNode, "particlefx"))
    {
        if (!spriteNode->xmlChildrenNode)
            return true;

        display.particles.push_back(reinterpret_cast<const char*>(
            spriteNode->xmlChildrenNode->content));
        return true;
    }
    return false;
}
