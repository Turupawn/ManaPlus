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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "render/graphics.h"

#include "main.h"

#include "configuration.h"
#include "graphicsmanager.h"
#include "logger.h"

#include "render/mglxinit.h"

#include "resources/imagehelper.h"
#include "resources/openglimagehelper.h"

#ifdef USE_OPENGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif
#include "render/mgldefines.h"
#endif

#include "debug.h"

#ifdef USE_OPENGL
#ifndef GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
#endif
#endif

Graphics *mainGraphics = nullptr;

Graphics::Graphics() :
    mWidth(0),
    mHeight(0),
    mActualWidth(0),
    mActualHeight(0),
    mClipStack(),
    mWindow(nullptr),
#ifdef USE_SDL2
    mRenderer(nullptr),
#endif
#ifdef USE_OPENGL
    mGLContext(nullptr),
#endif
    mBpp(0),
    mAlpha(false),
    mFullscreen(false),
    mHWAccel(false),
    mRedraw(false),
    mDoubleBuffer(false),
    mRect(),
    mSecure(false),
    mOpenGL(RENDER_SOFTWARE),
    mEnableResize(false),
    mNoFrame(false),
    mName("Unknown"),
    mStartFreeMem(0),
    mSync(false),
    mScale(1),
    mColor(),
    mColor2()
{
    mRect.x = 0;
    mRect.y = 0;
    mRect.w = 0;
    mRect.h = 0;
}

Graphics::~Graphics()
{
    endDraw();
#ifdef USE_SDL2
    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
#ifdef USE_OPENGL
    if (mGLContext)
    {
        SDL_GL_DeleteContext(mGLContext);
        mGLContext = nullptr;
    }
#endif
#endif
}

void Graphics::setSync(const bool sync)
{
    mSync = sync;
}

void Graphics::setMainFlags(const int w, const int h,
                            const int scale,
                            const int bpp,
                            const bool fs,
                            const bool hwaccel,
                            const bool resize,
                            const bool noFrame)
{
    logger->log("graphics backend: %s", getName().c_str());
    logger->log("Setting video mode %dx%d %s",
            w, h, fs ? "fullscreen" : "windowed");

    mBpp = bpp;
    mFullscreen = fs;
    mHWAccel = hwaccel;
    mEnableResize = resize;
    mNoFrame = noFrame;
    mActualWidth = w;
    mActualHeight = h;
    setScale(scale);
}

void Graphics::setScale(int scale)
{
    if (isAllowScale())
    {
        if (!scale)
            scale = 1;
        int scaleW = mActualWidth / scale;
        int scaleH = mActualHeight / scale;
        if (scaleW < 470)
        {
            scale = mActualWidth / 470;
            if (scale < 1)
                scale = 1;
            scaleH = mActualHeight / scale;
        }
        if (scaleH < 320)
        {
            scale = mActualHeight / 320;
            if (scale < 1)
                scale = 1;
        }
        logger->log("set scale: %d", scale);
        mScale = scale;
        mWidth = mActualWidth / mScale;
        mHeight = mActualHeight / mScale;
    }
    else
    {
        mScale = 1;
        mWidth = mActualWidth;
        mHeight = mActualHeight;
    }
    mRect.w = static_cast<RectSize>(mWidth);
    mRect.h = static_cast<RectSize>(mHeight);
}

int Graphics::getOpenGLFlags() const
{
#ifdef USE_OPENGL

#ifdef USE_SDL2
    int displayFlags = SDL_WINDOW_OPENGL;
    if (mFullscreen)
        displayFlags |= SDL_WINDOW_FULLSCREEN;
#else
    int displayFlags = SDL_ANYFORMAT | SDL_OPENGL;
#endif  // USE_SDL2

    if (mFullscreen)
    {
        displayFlags |= SDL_FULLSCREEN;
    }
    else
    {
        // Resizing currently not supported on Windows, where it would require
        // reuploading all textures.
#if !defined(_WIN32)
        if (mEnableResize)
            displayFlags |= SDL_RESIZABLE;
#endif
    }

    if (mNoFrame)
        displayFlags |= SDL_NOFRAME;

    return displayFlags;
#else
    return 0;
#endif  // USE_OPENGL
}

bool Graphics::setOpenGLMode()
{
#ifdef USE_OPENGL
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    if (!(mWindow = graphicsManager.createWindow(
        mActualWidth, mActualHeight,
        mBpp, getOpenGLFlags())))
    {
        logger->log("Window/context creation failed");
        mRect.w = 0;
        mRect.h = 0;
        return false;
    }

#if defined(USE_OPENGL) && defined(USE_X11)
    Glx::initFunctions();
#endif
#ifdef USE_SDL2
    int w1 = 0;
    int h1 = 0;
    SDL_GetWindowSize(mWindow, &w1, &h1);
    mRect.w = static_cast<int32_t>(w1 / mScale);
    mRect.h = static_cast<int32_t>(h1 / mScale);

    createGLContext();
#else  // USE_SDL2

    createGLContext();
    mRect.w = static_cast<uint16_t>(mWindow->w / mScale);
    mRect.h = static_cast<uint16_t>(mWindow->h / mScale);

#endif  // USE_SDL2

#ifdef __APPLE__
    if (mSync)
    {
        const GLint VBL = 1;
        CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &VBL);
    }
#endif

    graphicsManager.setGLVersion();
    graphicsManager.logVersion();

    // Setup OpenGL
    glViewport(0, 0, mActualWidth, mActualHeight);
    int gotDoubleBuffer = 0;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &gotDoubleBuffer);
    logger->log("Using OpenGL %s double buffering.",
                (gotDoubleBuffer ? "with" : "without"));

    graphicsManager.initOpenGL();
    initArrays(graphicsManager.getMaxVertices());
    graphicsManager.updateTextureFormat();
    updateMemoryInfo();

    GLint texSize;
    bool rectTex = graphicsManager.supportExtension(
        "GL_ARB_texture_rectangle");
    if (rectTex
        && OpenGLImageHelper::getInternalTextureType() == 4
        && getOpenGL() != RENDER_GLES_OPENGL
        && getOpenGL() != RENDER_MODERN_OPENGL
        && config.getBoolValue("rectangulartextures")
        && !graphicsManager.isUseTextureSampler())
    {
        logger->log1("using GL_ARB_texture_rectangle");
        OpenGLImageHelper::mTextureType = GL_TEXTURE_RECTANGLE_ARB;
        glEnable(GL_TEXTURE_RECTANGLE_ARB);
        glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, &texSize);
        OpenGLImageHelper::mTextureSize = texSize;
        logger->log("OpenGL texture size: %d pixels (rectangle textures)",
            OpenGLImageHelper::mTextureSize);
    }
    else
    {
        OpenGLImageHelper::mTextureType = GL_TEXTURE_2D;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
        OpenGLImageHelper::mTextureSize = texSize;
        logger->log("OpenGL texture size: %d pixels",
            OpenGLImageHelper::mTextureSize);
    }
    return videoInfo();
#else  // USE_OPENGL

    return false;
#endif  // USE_OPENGL
}

int Graphics::getSoftwareFlags() const
{
#ifdef USE_SDL2
    int displayFlags = SDL_WINDOW_SHOWN;
#else
    int displayFlags = SDL_ANYFORMAT;

    if (mHWAccel)
        displayFlags |= SDL_HWSURFACE | SDL_DOUBLEBUF;
    else
        displayFlags |= SDL_SWSURFACE;
#endif

    if (mFullscreen)
        displayFlags |= SDL_FULLSCREEN;
    else if (mEnableResize)
        displayFlags |= SDL_RESIZABLE;

    if (mNoFrame)
        displayFlags |= SDL_NOFRAME;
    return displayFlags;
}

#ifdef USE_OPENGL
void Graphics::createGLContext()
{
#ifdef USE_SDL2
    mGLContext = SDL_GL_CreateContext(mWindow);
#endif
}
#endif

void Graphics::updateMemoryInfo()
{
#ifdef USE_OPENGL
    if (mStartFreeMem)
        return;

    if (graphicsManager.supportExtension("GL_NVX_gpu_memory_info"))
    {
        glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
            &mStartFreeMem);
        logger->log("free video memory: %d", mStartFreeMem);
    }
#endif
}

int Graphics::getMemoryUsage() const
{
#ifdef USE_OPENGL
    if (!mStartFreeMem)
        return 0;

    if (graphicsManager.supportExtension("GL_NVX_gpu_memory_info"))
    {
        GLint val;
        glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
            &val);
        return mStartFreeMem - val;
    }
#endif
    return 0;
}

#ifdef USE_SDL2
void Graphics::dumpRendererInfo(const char *const str,
                                const SDL_RendererInfo &info)
{
    logger->log(str, info.name);
    logger->log("flags:");
    if (info.flags & SDL_RENDERER_SOFTWARE)
        logger->log(" software");
    if (info.flags & SDL_RENDERER_ACCELERATED)
        logger->log(" accelerated");
    if (info.flags & SDL_RENDERER_PRESENTVSYNC)
        logger->log(" vsync");
    if (info.flags & SDL_RENDERER_TARGETTEXTURE)
        logger->log(" texture target");
}
#endif

bool Graphics::videoInfo()
{
    logger->log("SDL video info");
#ifdef USE_SDL2
    logger->log("Using video driver: %s", SDL_GetCurrentVideoDriver());

    if (mRenderer)
    {
        SDL_RendererInfo info;
        SDL_GetRendererInfo(mRenderer, &info);
        dumpRendererInfo("Current SDL renderer name: %s", info);

        const int num = SDL_GetNumRenderDrivers();
        logger->log("Known renderers");
        for (int f = 0; f < num; f ++)
        {
            if (!SDL_GetRenderDriverInfo(f, &info))
                dumpRendererInfo("renderer name: %s", info);
        }
    }
#else
    char videoDriverName[65];
    if (SDL_VideoDriverName(videoDriverName, 64))
        logger->log("Using video driver: %s", videoDriverName);
    else
        logger->log1("Using video driver: unknown");
    mDoubleBuffer = ((mWindow->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF);
    logger->log("Double buffer mode: %s", mDoubleBuffer ? "yes" : "no");

    ImageHelper::dumpSurfaceFormat(mWindow);

    const SDL_VideoInfo *const vi = SDL_GetVideoInfo();
    if (!vi)
        return false;

    logger->log("Possible to create hardware surfaces: %s",
            ((vi->hw_available) ? "yes" : "no"));
    logger->log("Window manager available: %s",
            ((vi->wm_available) ? "yes" : "no"));
    logger->log("Accelerated hardware to hardware blits: %s",
            ((vi->blit_hw) ? "yes" : "no"));
    logger->log("Accelerated hardware to hardware colorkey blits: %s",
            ((vi->blit_hw_CC) ? "yes" : "no"));
    logger->log("Accelerated hardware to hardware alpha blits: %s",
            ((vi->blit_hw_A) ? "yes" : "no"));
    logger->log("Accelerated software to hardware blits: %s",
            ((vi->blit_sw) ? "yes" : "no"));
    logger->log("Accelerated software to hardware colorkey blits: %s",
            ((vi->blit_sw_CC) ? "yes" : "no"));
    logger->log("Accelerated software to hardware alpha blits: %s",
            ((vi->blit_sw_A) ? "yes" : "no"));
    logger->log("Accelerated color fills: %s",
            ((vi->blit_fill) ? "yes" : "no"));
#endif

    return true;
}

bool Graphics::setFullscreen(const bool fs)
{
    if (mFullscreen == fs)
        return true;

    return setVideoMode(mActualWidth, mActualHeight, mScale, mBpp, fs,
        mHWAccel, mEnableResize, mNoFrame);
}

bool Graphics::resizeScreen(const int width, const int height)
{
#ifdef USE_SDL2
    endDraw();

    mRect.w = static_cast<int32_t>(width / mScale);
    mRect.h = static_cast<int32_t>(height / mScale);
    mWidth = width / mScale;
    mHeight = height / mScale;
    mActualWidth = width;
    mActualHeight = height;

#ifdef USE_OPENGL
    // +++ probably this way will not work in windows/mac
    // Setup OpenGL
    glViewport(0, 0, mActualWidth, mActualHeight);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
#else
    // +++ need impliment resize in soft mode
#endif  // USE_OPENGL

    screenResized();
    beginDraw();
    return true;

#else
    const int prevWidth = mWidth;
    const int prevHeight = mHeight;

    endDraw();

    const bool success = setVideoMode(width, height, mScale, mBpp,
        mFullscreen, mHWAccel, mEnableResize, mNoFrame);

    // If it didn't work, try to restore the previous size. If that didn't
    // work either, bail out (but then we're in deep trouble).
    if (!success)
    {
        if (!setVideoMode(prevWidth, prevHeight, mScale, mBpp,
            mFullscreen, mHWAccel, mEnableResize, mNoFrame))
        {
            return false;
        }
    }

    screenResized();
    beginDraw();

    return success;
#endif  // USE_SDL2
}

int Graphics::getWidth() const
{
    return mWidth;
}

int Graphics::getHeight() const
{
    return mHeight;
}

void Graphics::drawNet(const int x1, const int y1,
                       const int x2, const int y2,
                       const int width, const int height)
{
    for (int y = y1; y < y2; y += height)
        drawLine(x1, y, x2, y);

    for (int x = x1; x < x2; x += width)
        drawLine(x, y1, x, y2);
}

void Graphics::setWindowSize(const int width A_UNUSED,
                             const int height A_UNUSED)
{
#ifdef USE_SDL2
    SDL_SetWindowSize(mWindow, width, height);
#endif
}

void Graphics::pushClipArea(const Rect &area)
{
    // Ignore area with a negate width or height
    // by simple pushing an empty clip area
    // to the stack.
    if (area.width < 0 || area.height < 0)
    {
        ClipRect carea;
        mClipStack.push(carea);
        return;
    }

    if (mClipStack.empty())
    {
        ClipRect carea;
        carea.x = area.x;
        carea.y = area.y;
        carea.width = area.width;
        carea.height = area.height;
        carea.xOffset = area.x;
        carea.yOffset = area.y;
        mClipStack.push(carea);
        return;
    }

    const ClipRect &top = mClipStack.top();
    ClipRect carea;
    carea = area;
    carea.xOffset = top.xOffset + carea.x;
    carea.yOffset = top.yOffset + carea.y;
    carea.x += top.xOffset;
    carea.y += top.yOffset;

    // Clamp the pushed clip rectangle.
    if (carea.x < top.x)
        carea.x = top.x;

    if (carea.y < top.y)
        carea.y = top.y;

    if (carea.x + carea.width > top.x + top.width)
    {
        carea.width = top.x + top.width - carea.x;

        if (carea.width < 0)
            carea.width = 0;
    }

    if (carea.y + carea.height > top.y + top.height)
    {
        carea.height = top.y + top.height - carea.y;

        if (carea.height < 0)
            carea.height = 0;
    }

    mClipStack.push(carea);
}

void Graphics::popClipArea()
{
    if (mClipStack.empty())
        return;

    mClipStack.pop();
}

const ClipRect *Graphics::getCurrentClipArea() const
{
    if (mClipStack.empty())
        return nullptr;

    return &mClipStack.top();
}

#ifdef USE_OPENGL
void Graphics::setOpenGLFlags()
{
    glEnable(GL_SCISSOR_TEST);

    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DITHER);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_COLOR_LOGIC_OP);
    glDisable(GL_DEPTH_BOUNDS_TEST_EXT);
    glDisable(GL_DEPTH_CLAMP);
    glDisable(GL_RASTERIZER_DISCARD);
    glDisable(GL_SAMPLE_MASK);

#ifndef ANDROID
#ifndef __MINGW32__
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
#endif
#endif
    glHint(GL_TEXTURE_COMPRESSION_HINT_ARB, GL_FASTEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
#endif
