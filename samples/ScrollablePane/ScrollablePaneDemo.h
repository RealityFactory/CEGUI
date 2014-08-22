/***********************************************************************
created:    Aug 22 2014
author:     Luca Ebach <lucaebach@gmail.com> 
            (based on origingal code by Tomas Lindquist Olsen)
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#ifndef _ScrollablePaneDemo_h_
#define _ScrollablePaneDemo_h_

/***********************************************************************
created:    Wed Aug 2 2006
author:     Tomas Lindquist Olsen
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/System.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/InputAggregator.h"
#include "CEGUI/InputEventReceiver.h"
#include "CEGUI/Font.h"
#include "CEGUI/Window.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/GUIContext.h"

#include "CEGUI/widgets/ScrollablePane.h"
#include "CEGUI/widgets/ScrolledContainer.h"

#include "SampleBase.h"


/*************************************************************************
Custom implementation of InputAggregator
*************************************************************************/
enum SampleSemanticValue
{
    // we start from the user-defined value
    SpawnNewDialog = CEGUI::SV_UserDefinedSemanticValue
};

class SampleInputAggregator : public CEGUI::InputAggregator
{
public:
    SampleInputAggregator(CEGUI::InputEventReceiver* input_receiver) :
        CEGUI::InputAggregator(input_receiver)
    {
    }

    void initialise()
    {
        d_keyValuesMappings[CEGUI::Key::Space] = SpawnNewDialog;
    }
};

// ScrollablePane demo sample class
class ScrollablePaneSample : public Sample
{
public:
    // method to initialse the samples windows and events.
    virtual bool initialise(CEGUI::GUIContext* guiContext);
    // method to perform any required cleanup operations.
    virtual void deinitialise();

private:
    // creates the menubar with content
    void createMenu(CEGUI::Window* bar);

    // quit menu item handler
    bool fileQuit(const CEGUI::EventArgs&);

    // new dialog menu item handler
    bool demoNewDialog(const CEGUI::EventArgs& e);
    bool semanticEventHandler(const CEGUI::EventArgs& e);

    // member data
    CEGUI::WindowManager* d_wm; // we will use the window manager alot
    CEGUI::System* d_system;    // the gui system
    CEGUI::Window* d_root;      // the gui sheet
    CEGUI::Font* d_font;        // the font we use
    CEGUI::ScrollablePane* d_pane; // the scrollable pane. center piece of the demo

    CEGUI::GUIContext* d_guiContext;
};

#endif // _ScrollablePaneDemo