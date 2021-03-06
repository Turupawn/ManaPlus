/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  Aethyra Development Team
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

#ifndef GUI_THEME_H
#define GUI_THEME_H

#include "listeners/configlistener.h"

#include "render/graphics.h"

#include "gui/palette.h"

#include "utils/stringvector.h"

#include <map>

#include "localconsts.h"

class DyePalette;
class Image;
class ImageSet;
class Skin;
class Theme;

struct ThemeInfo;

const int THEME_PALETTES = 5;

extern Theme *theme;

class Theme final : public Palette,
                    public ConfigListener
{
    public:
        Theme();

        ~Theme();

        A_DELETE_COPY(Theme)

        static void prepareThemePath();

        static void selectSkin();

        static std::string getThemePath() A_WARN_UNUSED
        { return mThemePath; }

        static std::string getThemeName() A_WARN_UNUSED
        { return mThemeName; }

        static void fillSkinsList(StringVect &list);

        static void fillFontsList(StringVect &list);

        static void fillSoundsList(StringVect &list);

        /**
         * Returns the patch to the given gui resource relative to the theme
         * or, if it isn't in the theme, relative to 'graphics/gui'.
         */
        static std::string resolveThemePath(const std::string &path)
                                            A_WARN_UNUSED;

        static Image *getImageFromTheme(const std::string &path) A_WARN_UNUSED;

        static ImageSet *getImageSetFromTheme(const std::string &path,
                                              const int w,
                                              const int h) A_WARN_UNUSED;

        static ImageSet *getImageSetFromThemeXml(const std::string &name,
                                          const std::string &name2,
                                          const int w,
                                          const int h)A_WARN_UNUSED;
        enum ThemePalette
        {
            BROWSERBOX = 0,
            BROWSERBOX_OUTLINE,
            SELFNICK,
            SELFNICK_OUTLINE,
            TEXT,
            TEXT_OUTLINE,
            CARET,
            SHADOW,
            OUTLINE,
            BORDER,
            PROGRESS_BAR,
            PROGRESS_BAR_OUTLINE,
            BUTTON,
            BUTTON_OUTLINE,
            BUTTON_DISABLED,
            BUTTON_DISABLED_OUTLINE,
            BUTTON_HIGHLIGHTED,
            BUTTON_HIGHLIGHTED_OUTLINE,
            BUTTON_PRESSED,
            BUTTON_PRESSED_OUTLINE,
            CHECKBOX,
            CHECKBOX_OUTLINE,
            DROPDOWN,
            DROPDOWN_OUTLINE,
            LABEL,
            LABEL_OUTLINE,
            LISTBOX,
            LISTBOX_OUTLINE,
            LISTBOX_SELECTED,
            LISTBOX_SELECTED_OUTLINE,
            RADIOBUTTON,
            RADIOBUTTON_OUTLINE,
            POPUP,
            POPUP_OUTLINE,
            TAB,
            TAB_OUTLINE,
            TAB_HIGHLIGHTED,
            TAB_HIGHLIGHTED_OUTLINE,
            TAB_SELECTED,
            TAB_SELECTED_OUTLINE,
            TEXTBOX,
            TEXTFIELD,
            TEXTFIELD_OUTLINE,
            WINDOW,
            WINDOW_OUTLINE,
            BATTLE_CHAT_TAB,
            BATTLE_CHAT_TAB_OUTLINE,
            CHANNEL_CHAT_TAB,
            CHANNEL_CHAT_TAB_OUTLINE,
            PARTY_CHAT_TAB,
            PARTY_CHAT_TAB_OUTLINE,
            PARTY_SOCIAL_TAB,
            PARTY_SOCIAL_TAB_OUTLINE,
            GUILD_CHAT_TAB,
            GUILD_CHAT_TAB_OUTLINE,
            GUILD_SOCIAL_TAB,
            GUILD_SOCIAL_TAB_OUTLINE,
            GM_CHAT_TAB,
            GM_CHAT_TAB_OUTLINE,
            BATTLE_CHAT_TAB_HIGHLIGHTED,
            BATTLE_CHAT_TAB_HIGHLIGHTED_OUTLINE,
            CHANNEL_CHAT_TAB_HIGHLIGHTED,
            CHANNEL_CHAT_TAB_HIGHLIGHTED_OUTLINE,
            PARTY_CHAT_TAB_HIGHLIGHTED,
            PARTY_CHAT_TAB_HIGHLIGHTED_OUTLINE,
            PARTY_SOCIAL_TAB_HIGHLIGHTED,
            PARTY_SOCIAL_TAB_HIGHLIGHTED_OUTLINE,
            GUILD_CHAT_TAB_HIGHLIGHTED,
            GUILD_CHAT_TAB_HIGHLIGHTED_OUTLINE,
            GUILD_SOCIAL_TAB_HIGHLIGHTED,
            GUILD_SOCIAL_TAB_HIGHLIGHTED_OUTLINE,
            GM_CHAT_TAB_HIGHLIGHTED,
            GM_CHAT_TAB_HIGHLIGHTED_OUTLINE,
            BATTLE_CHAT_TAB_SELECTED,
            BATTLE_CHAT_TAB_SELECTED_OUTLINE,
            CHANNEL_CHAT_TAB_SELECTED,
            CHANNEL_CHAT_TAB_SELECTED_OUTLINE,
            PARTY_CHAT_TAB_SELECTED,
            PARTY_CHAT_TAB_SELECTED_OUTLINE,
            PARTY_SOCIAL_TAB_SELECTED,
            PARTY_SOCIAL_TAB_SELECTED_OUTLINE,
            GUILD_CHAT_TAB_SELECTED,
            GUILD_CHAT_TAB_SELECTED_OUTLINE,
            GUILD_SOCIAL_TAB_SELECTED,
            GUILD_SOCIAL_TAB_SELECTED_OUTLINE,
            GM_CHAT_TAB_SELECTED,
            GM_CHAT_TAB_SELECTED_OUTLINE,
            BACKGROUND,
            BACKGROUND_GRAY,
            SCROLLBAR_GRAY,
            DROPDOWN_SHADOW,
            HIGHLIGHT,
            HIGHLIGHT_OUTLINE,
            TAB_FLASH,
            TAB_FLASH_OUTLINE,
            TAB_PLAYER_FLASH,
            TAB_PLAYER_FLASH_OUTLINE,
            SHOP_WARNING,
            ITEM_EQUIPPED,
            ITEM_EQUIPPED_OUTLINE,
            ITEM_NOT_EQUIPPED,
            ITEM_NOT_EQUIPPED_OUTLINE,
            CHAT,
            CHAT_OUTLINE,
            GM,
            GM_OUTLINE,
            GLOBAL,
            GLOBAL_OUTLINE,
            PLAYER,
            PLAYER_OUTLINE,
            WHISPER_TAB,
            WHISPER_TAB_OUTLINE,
            WHISPER_TAB_OFFLINE,
            WHISPER_TAB_OFFLINE_OUTLINE,
            WHISPER_TAB_HIGHLIGHTED,
            WHISPER_TAB_HIGHLIGHTED_OUTLINE,
            WHISPER_TAB_OFFLINE_HIGHLIGHTED,
            WHISPER_TAB_OFFLINE_HIGHLIGHTED_OUTLINE,
            WHISPER_TAB_SELECTED,
            WHISPER_TAB_SELECTED_OUTLINE,
            WHISPER_TAB_OFFLINE_SELECTED,
            WHISPER_TAB_OFFLINE_SELECTED_OUTLINE,
            IS,
            IS_OUTLINE,
            SERVER,
            SERVER_OUTLINE,
            LOGGER,
            LOGGER_OUTLINE,
            HYPERLINK,
            HYPERLINK_OUTLINE,
            UNKNOWN_ITEM,
            UNKNOWN_ITEM_OUTLINE,
            GENERIC,
            GENERIC_OUTLINE,
            HEAD,
            HEAD_OUTLINE,
            USABLE,
            USABLE_OUTLINE,
            TORSO,
            TORSO_OUTLINE,
            ONEHAND,
            ONEHAND_OUTLINE,
            LEGS,
            LEGS_OUTLINE,
            FEET,
            FEET_OUTLINE,
            TWOHAND,
            TWOHAND_OUTLINE,
            SHIELD,
            SHIELD_OUTLINE,
            RING,
            RING_OUTLINE,
            NECKLACE,
            NECKLACE_OUTLINE,
            ARMS,
            ARMS_OUTLINE,
            AMMO,
            AMMO_OUTLINE,
            SERVER_VERSION_NOT_SUPPORTED,
            SERVER_VERSION_NOT_SUPPORTED_OUTLINE,
            WARNING,
            WARNING_OUTLINE,
            CHARM,
            CHARM_OUTLINE,
            PLAYER_ADVANCED,
            PLAYER_ADVANCED_OUTLINE,
            BUBBLE_NAME,
            BUBBLE_NAME_OUTLINE,
            BUBBLE_TEXT,
            BUBBLE_TEXT_OUTLINE,
            BLACK,
            BLACK_OUTLINE,
            RED,
            RED_OUTLINE,
            GREEN,
            GREEN_OUTLINE,
            BLUE,
            BLUE_OUTLINE,
            ORANGE,
            ORANGE_OUTLINE,
            YELLOW,
            YELLOW_OUTLINE,
            PINK,
            PINK_OUTLINE,
            PURPLE,
            PURPLE_OUTLINE,
            GRAY,
            GRAY_OUTLINE,
            BROWN,
            BROWN_OUTLINE,
            STATUSBAR_ON,
            STATUSBAR_OFF,
            TABLE_BACKGROUND,
            SLOTS_BAR,
            SLOTS_BAR_OUTLINE,
            HP_BAR,
            HP_BAR_OUTLINE,
            MP_BAR,
            MP_BAR_OUTLINE,
            NO_MP_BAR,
            NO_MP_BAR_OUTLINE,
            XP_BAR,
            XP_BAR_OUTLINE,
            WEIGHT_BAR,
            WEIGHT_BAR_OUTLINE,
            MONEY_BAR,
            MONEY_BAR_OUTLINE,
            ARROWS_BAR,
            ARROWS_BAR_OUTLINE,
            STATUS_BAR,
            STATUS_BAR_OUTLINE,
            JOB_BAR,
            JOB_BAR_OUTLINE,
            OLDCHAT,
            OLDCHAT_OUTLINE,
            AWAYCHAT,
            AWAYCHAT_OUTLINE,
            SKILL_COOLDOWN,
            TEXT_DISABLED,
            TEXT_DISABLED_OUTLINE,
            THEME_COLORS_END
        };

        enum ProgressPalette
        {
            PROG_HP = 0,
            PROG_HP_POISON,
            PROG_MP,
            PROG_NO_MP,
            PROG_EXP,
            PROG_INVY_SLOTS,
            PROG_WEIGHT,
            PROG_JOB,
            PROG_UPDATE,
            PROG_MONEY,
            PROG_ARROWS,
            PROG_STATUS,
            THEME_PROG_END
        };

        static Color getProgressColor(const int type,
                                      const float progress) A_WARN_UNUSED;

        /**
         * Loads a skin.
         */
        Skin *load(const std::string &filename,
                   const std::string &filename2,
                   const bool full = true,
                   const std::string
                   &restrict defaultPath = getThemePath()) A_WARN_UNUSED;

        Skin *loadSkinRect(ImageRect &image,
                           const std::string &name,
                           const std::string &name2,
                           const int start = 0,
                           const int end = 8) A_WARN_UNUSED;

        void unload(Skin *const skin);

        /**
         * Updates the alpha values of all of the skins.
         */
        void updateAlpha();

        /**
         * Get the minimum opacity allowed to skins.
         */
        float getMinimumOpacity() const A_WARN_UNUSED
        { return mMinimumOpacity; }

        /**
         * Set the minimum opacity allowed to skins.
         * Set a negative value to free the minimum allowed.
         */
        void setMinimumOpacity(const float minimumOpacity);

        void optionChanged(const std::string &) override final;

        void loadRect(ImageRect &image,
                      const std::string &name,
                      const std::string &name2,
                      const int start = 0,
                      const int end = 8);

        static void unloadRect(const ImageRect &rect,
                               const int start = 0,
                               const int end = 8);

        static Image *getImageFromThemeXml(const std::string &name,
                                           const std::string &name2)
                                           A_WARN_UNUSED;

        static ThemeInfo *loadInfo(const std::string &themeName) A_WARN_UNUSED;

    private:
        Skin *readSkin(const std::string &filename0,
                       const bool full) A_WARN_UNUSED;

        // Map containing all window skins
        typedef std::map<std::string, Skin*> Skins;
        typedef Skins::iterator SkinIterator;

        Skins mSkins;

        static std::string mThemePath;
        static std::string mThemeName;
        static std::string mScreenDensity;

        static bool tryThemePath(const std::string &themePath) A_WARN_UNUSED;

        void loadColors(std::string file = "");

        /**
         * Tells if the current skins opacity
         * should not get less than the given value
         */
        float mMinimumOpacity;

        typedef std::vector<DyePalette*> ProgressColors;
        ProgressColors mProgressColors;
};

#endif  // GUI_THEME_H
