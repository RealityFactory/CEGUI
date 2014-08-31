/***********************************************************************
	created:	25/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of common Titlebar parts.
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
#include "CEGUI/widgets/Titlebar.h"
#include "CEGUI/widgets/FrameWindow.h"
#include "CEGUI/PointerIndicator.h"
#include "CEGUI/CoordConverter.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String Titlebar::EventNamespace("Titlebar");
const String Titlebar::WidgetTypeName("CEGUI/Titlebar");

/*************************************************************************
	Constructor
*************************************************************************/
Titlebar::Titlebar(const String& type, const String& name) :
	Window(type, name)
{
	addTitlebarProperties();
	setAlwaysOnTop(true);

	// basic initialisation
	d_dragging = false;
	d_dragEnabled = true;
}

/*************************************************************************
	Destructor
*************************************************************************/
Titlebar::~Titlebar(void)
{
}


/*************************************************************************
	Return whether this title bar will respond to dragging.
*************************************************************************/
bool Titlebar::isDraggingEnabled(void) const
{
	return d_dragEnabled;
}


/*************************************************************************
	Set whether this title bar widget will respond to dragging.
*************************************************************************/
void Titlebar::setDraggingEnabled(bool setting)
{
	if (d_dragEnabled != setting)
	{
		d_dragEnabled = setting;

		// stop dragging now if the setting has been disabled.
		if ((!d_dragEnabled) && d_dragging)
		{
			releaseInput();
		}

		// call event handler.
		WindowEventArgs args(this);
		onDraggingModeChanged(args);
	}

}

bool Titlebar::isDragged() const
{
    return d_dragging;
}

const glm::vec2& Titlebar::getDragPoint() const
{
    return d_dragPoint;
}

/*************************************************************************
	Handler for pointer movement events
*************************************************************************/
void Titlebar::onPointerMove(PointerEventArgs& e)
{
	// Base class processing.
	Window::onPointerMove(e);

	if (d_dragging && (d_parent != 0))
	{
        // calculate amount that window has been moved
        const glm::vec2 delta(CoordConverter::screenToWindow(*this, e.position) - d_dragPoint);

		// move the window.  *** Again: Titlebar objects should only be attached to FrameWindow derived classes. ***
		((FrameWindow*)d_parent)->offsetPixelPosition(delta);

		++e.handled;
	}
}


/*************************************************************************
    Handler for pointer press events
*************************************************************************/
void Titlebar::onPointerPressHold(PointerEventArgs& e)
{
	// Base class processing
    Window::onPointerPressHold(e);

    if (e.source == PS_Left)
	{
		if ((d_parent != 0) && d_dragEnabled)
		{
            // we want all pointer inputs from now on
			if (captureInput())
			{
				// initialise the dragging state
				d_dragging = true;
				d_dragPoint = CoordConverter::screenToWindow(*this, e.position);

                // store old constraint area
                d_oldCursorArea = getGUIContext().
                    getPointerIndicator().getConstraintArea();

				// setup new constraint area to be the intersection of the old area and our grand-parent's clipped inner-area
				Rectf constrainArea;

				if ((d_parent == 0) || (getParent()->getParent() == 0))
				{
                    const Rectf screen(glm::vec2(0, 0), getRootContainerSize());
					constrainArea = screen.getIntersection(d_oldCursorArea);
				}
				else 
				{
					constrainArea = getParent()->getParent()->getInnerRectClipper().getIntersection(d_oldCursorArea);
				}

                getGUIContext().getPointerIndicator().
                    setConstraintArea(&constrainArea);
			}
		}

		++e.handled;
	}
}


/*************************************************************************
    Handler for pointer activation events
*************************************************************************/
void Titlebar::onPointerActivate(PointerEventArgs& e)
{
	// Base class processing
    Window::onPointerActivate(e);

    if (e.source == PS_Left)
	{
		releaseInput();
		++e.handled;
	}
}

void Titlebar::onSemanticInputEvent(SemanticEventArgs& e)
{
    // Base class processing
    Window::onSemanticInputEvent(e);

    if (isDisabled())
        return;

    if (e.d_semanticValue == SV_SelectWord && e.d_payload.source == PS_Left)
    {

        // if we do not have a parent window, then obviously nothing should happen.
        if (d_parent)
        {
            // we should only ever be attached to a FrameWindow (or derived) class
            ((FrameWindow*)d_parent)->toggleRollup();
        }

        ++e.handled;
    }
}

/*************************************************************************
    Handler for if the window loses capture of the pointer.
*************************************************************************/
void Titlebar::onCaptureLost(WindowEventArgs& e)
{
	// Base class processing
	Window::onCaptureLost(e);

    // when we lose out hold on the pointer inputs, we are no longer dragging.
	d_dragging = false;

	// restore old constraint area
	getGUIContext().
        getPointerIndicator().setConstraintArea(&d_oldCursorArea);
}


/*************************************************************************
	Handler for when the font for this Window is changed
*************************************************************************/
void Titlebar::onFontChanged(WindowEventArgs& e)
{
	Window::onFontChanged(e);

	if (d_parent)
	{
		getParent()->performChildWindowLayout();
	}
}


/*************************************************************************
	Add title bar specific properties
*************************************************************************/
void Titlebar::addTitlebarProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(Titlebar, bool,
        "DraggingEnabled", "Property to get/set the state of the dragging enabled setting for the Titlebar.  Value is either \"true\" or \"false\".",
        &Titlebar::setDraggingEnabled, &Titlebar::isDraggingEnabled, true
    );
}

} // End of  CEGUI namespace section
