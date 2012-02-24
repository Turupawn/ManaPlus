/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#include "gui/setup_audio.h"

#include "configuration.h"
#include "sound.h"

#include "gui/viewport.h"

#include "gui/widgets/layouthelper.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/setupitem.h"

#include "utils/gettext.h"

#include "debug.h"

Setup_Audio::Setup_Audio()
{
    setName(_("Audio"));

    // Do the layout
    LayoutHelper h(this);
    ContainerPlacer place = h.getPlacer(0, 0);
    place(0, 0, mScroll, 10, 10);

    new SetupItemCheckBox(_("Enable Audio"), "", "sound", this, "soundEvent");

    new SetupItemCheckBox(_("Enable music"), "",
        "playMusic", this, "playMusicEvent");

    new SetupItemCheckBox(_("Enable game sfx"), "",
        "playBattleSound", this, "playBattleSoundEvent");

    new SetupItemCheckBox(_("Enable gui sfx"), "",
        "playGuiSound", this, "playGuiSoundEvent");

    new SetupItemSlider(_("Sfx volume"), "", "sfxVolume",
        this, "sfxVolumeEvent", 0, sound.getMaxVolume(), 150, true);

    new SetupItemSlider(_("Music volume"), "", "musicVolume",
        this, "musicVolumeEvent", 0, sound.getMaxVolume(), 150, true);

    new SetupItemCheckBox(_("Enable mumble voice chat"), "",
        "enableMumble", this, "enableMumbleEvent");

    new SetupItemCheckBox(_("Download music"), "",
        "download-music", this, "download-musicEvent");

    setDimension(gcn::Rectangle(0, 0, 550, 350));
}

void Setup_Audio::apply()
{
    SetupTabScroll::apply();
    if (config.getBoolValue("sound"))
    {
        sound.init();
        if (viewport && config.getBoolValue("playMusic"))
        {
            Map *map = viewport->getMap();
            if (map)
                sound.playMusic(map->getMusicFile());
        }
        else
        {
            sound.stopMusic();
        }
    }
    else
    {
        sound.close();
    }
}
