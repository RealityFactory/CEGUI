/***********************************************************************
 *    filename:   InputInjection.cpp
 *    created:    21/5/2013
 *    author:     Timotei Dolean
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

#include <boost/test/unit_test.hpp>

#include <vector>
#include <iostream>

#include "CEGUI/CEGUI.h"

using namespace CEGUI;

struct InputInjectionFixture
{
    InputInjectionFixture()
        : d_buttonHandledCount(0)
        , d_windowHandledCount(0)
    {
        d_window = WindowManager::getSingleton().createWindow("DefaultWindow");
        d_window->setPosition(UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));
        d_window->setSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));

        d_button = WindowManager::getSingleton().createWindow("CEGUI/PushButton");
        d_button->setPosition(UVector2(cegui_reldim(0.2f), cegui_reldim(0.2f)));
        d_button->setSize(USize(cegui_reldim(0.2f), cegui_reldim(0.2f)));

        d_window->addChild(d_button);

        System::getSingleton().getDefaultGUIContext().setRootWindow(d_window);
        System::getSingleton().notifyDisplaySizeChanged(Sizef(100, 100));

        d_windowConnections.push_back(
            d_window->subscribeEvent(Window::EventMouseClick,
                Event::Subscriber(&InputInjectionFixture::handleWindowEvent, this))
            );
        d_buttonConnections.push_back(
            d_button->subscribeEvent(PushButton::EventClicked,
                Event::Subscriber(&InputInjectionFixture::handleButtonEvent, this))
            );
    }

    ~InputInjectionFixture()
    {
        disconnectConnections(d_windowConnections);
        disconnectConnections(d_buttonConnections);

        System::getSingleton().getDefaultGUIContext().setRootWindow(0);

        WindowManager::getSingleton().destroyWindow(d_window);
    }

    bool handleButtonEvent(const EventArgs& e)
    {
        ++d_buttonHandledCount;
        return true;
    }

    bool handleWindowEvent(const EventArgs& e)
    {
        ++d_windowHandledCount;
        return true;
    }

    void disconnectConnections(std::vector<Event::Connection>& connections)
    {
        for(std::vector<Event::Connection>::iterator itor = connections.begin();
            itor != connections.end(); ++itor)
        {
            (*itor)->disconnect();
        }
    }

    int d_buttonHandledCount;
    int d_windowHandledCount;

    Window* d_window;
    Window* d_button;

    std::vector<Event::Connection> d_windowConnections;
    std::vector<Event::Connection> d_buttonConnections;
};

BOOST_FIXTURE_TEST_SUITE(InputInjection, InputInjectionFixture)

BOOST_AUTO_TEST_CASE(OneClickWindow)
{
    System::getSingleton().getDefaultGUIContext().injectMouseButtonClick(
        LeftButton);

    BOOST_CHECK_EQUAL(d_windowHandledCount, 1);
    BOOST_CHECK_EQUAL(d_buttonHandledCount, 0);
}

BOOST_AUTO_TEST_CASE(MultipleClicksWindow)
{
    System::getSingleton().getDefaultGUIContext().injectMouseButtonClick(
        LeftButton);
    System::getSingleton().getDefaultGUIContext().injectMouseButtonClick(
        LeftButton);

    BOOST_CHECK_EQUAL(d_windowHandledCount, 2);
    BOOST_CHECK_EQUAL(d_buttonHandledCount, 0);
}

BOOST_AUTO_TEST_CASE(OneClickButton)
{
    //TODO: It doesn't work with injectMouseClick, but only with mouseDown + Up
    // Shouldn't it?
    System::getSingleton().getDefaultGUIContext().injectMousePosition(30.0f, 30.0f);
    System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
        LeftButton);
    System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
        LeftButton);

    BOOST_CHECK_EQUAL(d_windowHandledCount, 0);
    BOOST_CHECK_EQUAL(d_buttonHandledCount, 1);
}

BOOST_AUTO_TEST_SUITE_END()
