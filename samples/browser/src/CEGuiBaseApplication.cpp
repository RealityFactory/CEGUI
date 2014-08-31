/***********************************************************************
    created:    7/2/2008
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUISamplesConfig.h"
#include "CEGuiBaseApplication.h"
#include "SamplesFramework.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/Image.h"
#include "CEGUI/Font.h"
#include "CEGUI/Scheme.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/ScriptModule.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/RenderTarget.h"
#include "CEGUI/AnimationManager.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#ifdef __APPLE__
#   include <Carbon/Carbon.h>
#endif

// setup default-default path
#ifndef CEGUI_SAMPLE_DATAPATH
    #define CEGUI_SAMPLE_DATAPATH "../datafiles"
#endif

/***********************************************************************
    Static / Const data
*************************************************************************/
const char CEGuiBaseApplication::DATAPATH_VAR_NAME[] = "CEGUI_SAMPLE_DATAPATH";
SamplesFrameworkBase* CEGuiBaseApplication::d_sampleApp(0);

//----------------------------------------------------------------------------//
CEGuiBaseApplication::CEGuiBaseApplication() :
    d_quitting(false),
    d_renderer(0),
    d_imageCodec(0),
    d_resourceProvider(0),
    d_logoGeometry(0),
    d_FPSGeometry(0),
    d_FPSElapsed(1.0f),
    d_FPSFrames(0),
    d_FPSValue(0),
    d_spinLogo(false)
{
}

//----------------------------------------------------------------------------//
CEGuiBaseApplication::~CEGuiBaseApplication()
{
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::renderSingleFrame(const float elapsed)
{
    CEGUI::System& gui_system(CEGUI::System::getSingleton());

    gui_system.injectTimePulse(elapsed);
    d_sampleApp->update(static_cast<float>(elapsed));

    updateFPS(elapsed);
    updateLogo(elapsed);

    beginRendering(elapsed);

    CEGUI::Renderer* gui_renderer(gui_system.getRenderer());
    gui_renderer->beginRendering();

    d_sampleApp->renderGUIContexts();

    gui_renderer->endRendering();
    CEGUI::WindowManager::getSingleton().cleanDeadPool();

    endRendering();
}

//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::execute(SamplesFrameworkBase* sampleApp)
{
    d_sampleApp = sampleApp;

    if (!d_renderer)
        throw CEGUI::InvalidRequestException("CEGuiBaseApplication::run: "
            "Base application subclass did not create Renderer!");

    // start up CEGUI system using objects created in subclass constructor.
    CEGUI::System::create(*d_renderer, d_resourceProvider, 0, d_imageCodec);

    // initialise resource system
    initialiseResourceGroupDirectories();
    initialiseDefaultResourceGroups();

    const CEGUI::Rectf scrn(glm::vec2(0, 0), d_renderer->getDisplaySize());

    // create logo imageset and draw the image (we only ever draw this once)
    CEGUI::ImageManager::getSingleton().addBitmapImageFromFile("cegui_logo",
                                                         "logo.png");
    CEGUI::ImageManager::getSingleton().get("cegui_logo").render(
        d_logoGeometry, CEGUI::Rectf(0, 0, 183, 89), 0, false, CEGUI::ColourRect(0xFFFFFFFF));

    // initial position update of the logo
    updateLogoGeometry();
    // setup for spinning logo
    updateLogoGeometryRotation();

    // clearing this queue actually makes sure it's created(!)
    CEGUI::System::getSingleton().getDefaultGUIContext().clearGeometry(CEGUI::RQ_OVERLAY);

    // subscribe handler to render overlay items
    CEGUI::System::getSingleton().getDefaultGUIContext().
        subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
            CEGUI::Event::Subscriber(&CEGuiBaseApplication::sampleBrowserOverlayHandler,
                                     this));

    // subscribe handler to reposition logo when window is sized.
    CEGUI::System::getSingleton().subscribeEvent(
        CEGUI::System::EventDisplaySizeChanged,
        CEGUI::Event::Subscriber(&CEGuiBaseApplication::resizeHandler,
        this));

    const CEGUI::Rectf& area(CEGUI::System::getSingleton().getRenderer()->
                             getDefaultRenderTarget().getArea());
    d_sampleApp->setApplicationWindowSize(static_cast<int>(area.getWidth()),
                                          static_cast<int>(area.getHeight()));

    run();

    cleanup();
    destroyWindow();

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::cleanup()
{
    CEGUI::ImageManager::getSingleton().destroy("cegui_logo");

    const size_t logo_buffer_count = d_logoGeometry.size();
    for (size_t i = 0; i < logo_buffer_count; ++i)
        d_renderer->destroyGeometryBuffer(*d_logoGeometry.at(i));
    d_logoGeometry.clear();

    const size_t fps_buffer_count = d_FPSGeometry.size();
    for (size_t i = 0; i < fps_buffer_count; ++i)
        d_renderer->destroyGeometryBuffer(*d_FPSGeometry.at(i));
    d_FPSGeometry.clear();

    CEGUI::System::destroy();
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::initialiseResourceGroupDirectories()
{
    // initialise the required dirs for the DefaultResourceProvider
    CEGUI::DefaultResourceProvider* rp =
        static_cast<CEGUI::DefaultResourceProvider*>
            (CEGUI::System::getSingleton().getResourceProvider());

    const char* dataPathPrefix = getDataPathPrefix();
    char resourcePath[PATH_MAX];

    // for each resource type, set a resource group directory
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "schemes/");
    rp->setResourceGroupDirectory("schemes", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "imagesets/");
    rp->setResourceGroupDirectory("imagesets", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "fonts/");
    rp->setResourceGroupDirectory("fonts", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "layouts/");
    rp->setResourceGroupDirectory("layouts", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "looknfeel/");
    rp->setResourceGroupDirectory("looknfeels", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "lua_scripts/");
    rp->setResourceGroupDirectory("lua_scripts", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "xml_schemas/");
    rp->setResourceGroupDirectory("schemas", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "animations/");
    rp->setResourceGroupDirectory("animations", resourcePath);
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::initialiseDefaultResourceGroups()
{
    // set the default resource groups to be used
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
    CEGUI::AnimationManager::setDefaultResourceGroup("animations");

    // setup default group for validation schemas
    CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
        parser->setProperty("SchemaDefaultResourceGroup", "schemas");
}

//----------------------------------------------------------------------------//
const char* CEGuiBaseApplication::getDataPathPrefix() const
{
    static char dataPathPrefix[PATH_MAX];

#ifdef __APPLE__
    CFURLRef datafilesURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(),
                                                    CFSTR("datafiles"),
                                                    0, 0);
    CFURLGetFileSystemRepresentation(datafilesURL, true,
                                     reinterpret_cast<UInt8*>(dataPathPrefix),
                                     PATH_MAX);
    CFRelease(datafilesURL);
#else
    char* envDataPath = 0;

    // get data path from environment var
    envDataPath = getenv(DATAPATH_VAR_NAME);

    // set data path prefix / base directory.  This will
    // be either from an environment variable, or from
    // a compiled in default based on original configure
    // options
    if (envDataPath != 0)
        strcpy(dataPathPrefix, envDataPath);
    else
        strcpy(dataPathPrefix, CEGUI_SAMPLE_DATAPATH);
#endif

    return dataPathPrefix;
}

//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::sampleBrowserOverlayHandler(const CEGUI::EventArgs& args)
{
    if (static_cast<const CEGUI::RenderQueueEventArgs&>(args).queueID != CEGUI::RQ_OVERLAY)
        return false;

    // draw the logo
    const size_t logo_buffer_count = d_logoGeometry.size();
    for (size_t i = 0; i < logo_buffer_count; ++i)
    {
        d_logoGeometry[i]->draw();
    }

    // draw FPS value
    const size_t fps_buffer_count = d_FPSGeometry.size();
    for (size_t i = 0; i < fps_buffer_count; ++i)
        d_FPSGeometry.at(i)->draw();

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::sampleOverlayHandler(const CEGUI::EventArgs& args)
{
    if (static_cast<const CEGUI::RenderQueueEventArgs&>(args).queueID != CEGUI::RQ_OVERLAY)
        return false;

    // Draw FPS value
    const size_t bufferCount = d_FPSGeometry.size();
    for (size_t i = 0; i < bufferCount; ++i)
        d_FPSGeometry.at(i)->draw();

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::updateFPS(const float elapsed)
{
    // another frame
    ++d_FPSFrames;

    if ((d_FPSElapsed += elapsed) >= 1.0f)
    {
        if (d_FPSFrames != d_FPSValue)
        {
            d_FPSValue = d_FPSFrames;

            CEGUI::Font* fnt = CEGUI::System::getSingleton().getDefaultGUIContext().getDefaultFont();
            if (!fnt)
                return;

            // update FPS imagery
            char fps_textbuff[16];
            sprintf(fps_textbuff , "FPS: %d", d_FPSValue);

            const size_t bufferCount = d_FPSGeometry.size();
            for (size_t i = 0; i < bufferCount; ++i)
                d_renderer->destroyGeometryBuffer(*d_FPSGeometry.at(i));
            d_FPSGeometry.clear();

            fnt->drawText(d_FPSGeometry, fps_textbuff, glm::vec2(0, 0), 0, false,
                          CEGUI::Colour(0xFFFFFFFF));

            updateFPSGeometry();
        }

        // reset counter state
        d_FPSFrames = 0;

        float modValue = 1.0f;
        d_FPSElapsed = std::modf(d_FPSElapsed, &modValue);
    }
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::updateLogo(const float elapsed)
{
    if (!d_spinLogo)
        return;

    static float rot = 0.0f;

    const size_t bufferCount = d_logoGeometry.size();
    for (size_t i = 0; i < bufferCount; ++i)
    {
        d_logoGeometry[i]->setRotation(glm::quat(glm::vec3(glm::radians(rot), 0, 0)));
    }

    rot = fmodf(rot + 180.0f * elapsed, 360.0f);
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::updateLogoGeometry()
{
    const CEGUI::Rectf scrn(d_renderer->getDefaultRenderTarget().getArea());
    const glm::vec3 position(10.0f, scrn.getSize().d_height - 89.0f, 0.0f);

    const size_t bufferCount = d_logoGeometry.size();
    for (size_t i = 0; i < bufferCount; ++i)
    {
        d_logoGeometry[i]->setClippingRegion(scrn);
        d_logoGeometry[i]->setTranslation(position);
    }
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::updateFPSGeometry()
{
    const CEGUI::Rectf scrn(d_renderer->getDefaultRenderTarget().getArea());
    const glm::vec3 position(scrn.getSize().d_width - 120.0f, 0.0f, 0.0f);

    const size_t bufferCount = d_FPSGeometry.size();
    for (size_t i = 0; i < bufferCount; ++i)
    {
        d_FPSGeometry[i]->setClippingRegion(scrn);
        d_FPSGeometry[i]->setTranslation(position);
    }
}


//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::resizeHandler(const CEGUI::EventArgs& /*args*/)
{
    // clear FPS geometry and see that it gets recreated in the next frame
    const size_t bufferCount = d_FPSGeometry.size();
    for (size_t i = 0; i < bufferCount; ++i)
        d_renderer->destroyGeometryBuffer(*d_FPSGeometry.at(i));
    d_FPSGeometry.clear();

    d_FPSValue = 0;

    const CEGUI::Rectf& area(CEGUI::System::getSingleton().getRenderer()->
                             getDefaultRenderTarget().getArea());
    d_sampleApp->handleNewWindowSize(area.getWidth(), area.getHeight());

    updateLogoGeometry();
    updateFPSGeometry();

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::registerSampleOverlayHandler(CEGUI::GUIContext* gui_context)
{
    // clearing this queue actually makes sure it's created(!)
    gui_context->clearGeometry(CEGUI::RQ_OVERLAY);

    // subscribe handler to render overlay items
    gui_context->subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
        CEGUI::Event::Subscriber(&CEGuiBaseApplication::sampleOverlayHandler,
        this));
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::updateLogoGeometryRotation()
{
    const size_t bufferCount = d_logoGeometry.size();
    for (size_t i = 0; i < bufferCount; ++i)
    {
        d_logoGeometry[i]->setPivot(glm::vec3(91.5f, 44.5f, 0));
    }
}


//----------------------------------------------------------------------------//