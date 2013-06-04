/***********************************************************************
    filename:   MenuNavigationDemo.cpp
    created:    30/5/2013
    author:     Timotei Dolean
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include "MenuNavigationDemo.h"
#include "CEGUI/CEGUI.h"

#include <iostream>

/** This sample uses most of the code from the 'HelloWorld' sample. 
    Thus, most of the clarifying comments have been removed for brevity. **/

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool MenuNavigationDemo::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

     d_usedFiles = CEGUI::String(__FILE__);

    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    WindowManager& winMgr = WindowManager::getSingleton();
    d_root = winMgr.loadLayoutFromFile("MenuNavigationDemo.layout");

    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    guiContext->setDefaultFont(&defaultFont);

    guiContext->setRootWindow(d_root);

    TabControl* tabControl = static_cast<TabControl*>(d_root->getChild("TabControl"));

    tabControl->addTab(winMgr.loadLayoutFromFile("MenuNavigationDemoTabPage1.layout"));
    tabControl->addTab(winMgr.loadLayoutFromFile("MenuNavigationDemoTabPage2.layout"));

    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void MenuNavigationDemo::deinitialise()
{
}

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static MenuNavigationDemo sample;
    return sample;
}