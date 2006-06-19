/***********************************************************************
	filename: 	CEGUITabControl.cpp
	created:	08/08/2004
	author:		Steve Streeting
	
	purpose:	Implementation of Tab Control widget base class
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
#include "CEGUIExceptions.h"
#include "elements/CEGUITabControl.h"
#include "elements/CEGUITabButton.h"
#include "elements/CEGUIGUISheet.h"
#include "CEGUIFont.h"
#include "CEGUIWindowManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String TabControl::EventNamespace("TabControl");
const String TabControl::WidgetTypeName("CEGUI/TabControl");

/*************************************************************************
    TabControlWindowRenderer
*************************************************************************/
TabControlWindowRenderer::TabControlWindowRenderer(const String& name) :
    WindowRenderer(name, TabControl::EventNamespace)
{
}

/*************************************************************************
	Definition of Properties for this class
*************************************************************************/
TabControlProperties::TabHeight		            TabControl::d_tabHeightProperty;
TabControlProperties::TabTextPadding		    TabControl::d_tabTextPaddingProperty;

/*************************************************************************
	Constants
*************************************************************************/
// event names
const String TabControl::EventSelectionChanged( "TabSelectionChanged" );

/*************************************************************************
    Child Widget name suffix constants
*************************************************************************/
const String TabControl::ContentPaneNameSuffix( "__auto_TabPane__" );
const String TabControl::TabButtonNameSuffix( "__auto_btn" );
const String TabControl::TabButtonPaneNameSuffix( "__auto_TabPane__Buttons" );

/*************************************************************************
	Constructor for TabControl base class.
*************************************************************************/
TabControl::TabControl(const String& type, const String& name)
	: Window(type, name),
    d_nextTabIndex(0)
{
	addTabControlProperties();
}


/*************************************************************************
	Destructor for TabControl base class.
*************************************************************************/
TabControl::~TabControl(void)
{
    // Should be handled in superclass (all child windows)
}

/*************************************************************************
	Initialise the Window based object ready for use.
*************************************************************************/
void TabControl::initialiseComponents(void)
{
	performChildWindowLayout();
}
/*************************************************************************
Get the number of tabs
*************************************************************************/
size_t TabControl::getTabCount(void) const
{   
    return getTabPane()->getChildCount();
}
/*************************************************************************
Get the tab with a given name
*************************************************************************/
Window*	TabControl::getTabContents(const String& name) const
{
    return getTabPane()->getChild(name);
}
/*************************************************************************
Get the tab at a given ID
*************************************************************************/
Window*	TabControl::getTabContents(uint ID) const
{
    return getTabPane()->getChild(ID);
}
/*************************************************************************
Get the tab for the given index
*************************************************************************/
Window*	TabControl::getTabContentsAtIndex(size_t index) const
{
    return d_tabButtonIndexMap.find(index)->second->getTargetWindow();
}

/*************************************************************************
Return whether the tab content window is currently selected.
*************************************************************************/
bool TabControl::isTabContentsSelected(Window* wnd) const
{
	TabButton* button = getButtonForTabContents(wnd);
	return button->isSelected();
}

/*************************************************************************
Return whether the tab content window is currently selected.
*************************************************************************/
size_t TabControl::getSelectedTabIndex() const
{
    size_t index;
    TabButtonIndexMap::const_iterator i, iend;
    iend = d_tabButtonIndexMap.end();
    for (i = d_tabButtonIndexMap.begin(); i != iend; ++i)
    {
        // get corresponding tab button and content window
        TabButton* tb = i->second;
        if (tb->isSelected())
        {
            index = i->first;
			break;
        }
	}
	return index;
}

/*************************************************************************
Set the selected tab by window name
*************************************************************************/
void TabControl::setSelectedTab(const String &name)
{
    // get window
    Window* wnd = getTabPane()->getChild(name);

    selectTab_impl(wnd);
}
/*************************************************************************
Set the selected tab by window ID
*************************************************************************/
void TabControl::setSelectedTab(uint ID)
{
    // get window
    Window* wnd = getTabPane()->getChild(ID);

    selectTab_impl(wnd);
}
/*************************************************************************
Set the selected tab by window name
*************************************************************************/
void TabControl::setSelectedTabAtIndex(size_t index)
{
	Window* wnd = getTabContentsAtIndex(index);
	selectTab_impl(wnd);
}

/*************************************************************************
Set the tab height
*************************************************************************/
void TabControl::setTabHeight(const UDim& height)
{
    d_tabHeight = height;

    performChildWindowLayout();
}

/*************************************************************************
Set the tab text padding
*************************************************************************/
void TabControl::setTabTextPadding(const UDim& padding)
{
    d_tabPadding = padding;

    performChildWindowLayout();
}

/*************************************************************************
Add a new tab
*************************************************************************/
void TabControl::addTab(Window* wnd)
{
    // Create a new TabButton
    addButtonForTabContent(wnd);
    // Add the window to the content pane
    getTabPane()->addChildWindow(wnd);
    // Auto-select?
    if (getTabCount() == 1)
    {
        setSelectedTab(wnd->getName());
    }
    else
    {
        // initialise invisible content
        wnd->setVisible(false);
    }
    // Just request redraw
    performChildWindowLayout();
    requestRedraw();
    // Subscribe to text changed event so that we can resize as needed
    wnd->subscribeEvent(Window::EventTextChanged, 
        Event::Subscriber(&TabControl::handleContentWindowTextChanged, this));

}
/*************************************************************************
Remove a tab
*************************************************************************/
void TabControl::removeTab(const String& name)
{
    // do nothing if given window is not attached as a tab.
    if (!getTabPane()->isChild(name))
        return;

    Window* wnd = getTabPane()->getChild(name);

    // Was this selected?
    bool reselect = wnd->isVisible();
    // Tab buttons are the 2nd onward children
    getTabPane()->removeChildWindow(name);

    // remove button too
    removeButtonForTabContent(wnd);

    if (reselect)
    {
        // Select another tab
        if (getTabCount() > 0)
        {
            setSelectedTab(getTabPane()->getChildAtIdx(0)->getName());
        }
    }

    performChildWindowLayout();

    requestRedraw();
}
/*************************************************************************
Remove a tab by ID
*************************************************************************/
void TabControl::removeTab(uint ID)
{
    // do nothing if given window is not attached as a tab.
    if (!getTabPane()->isChild(ID))
        return;

    Window* wnd = getTabPane()->getChild(ID);

    // Was this selected?
    bool reselect = wnd->isVisible();
    // Tab buttons are the 2nd onward children
    getTabPane()->removeChildWindow(ID);

    // remove button too
    removeButtonForTabContent(wnd);

    if (reselect)
    {
        // Select another tab
        if (getTabCount() > 0)
        {
            setSelectedTab(getTabPane()->getChildAtIdx(0)->getName());
        }
    }

    performChildWindowLayout();

    requestRedraw();

}
/*************************************************************************
Add tab button
*************************************************************************/
void TabControl::addButtonForTabContent(Window* wnd)
{
    // Create the button
    TabButton* tb = createTabButton(makeButtonName(wnd));
    // Copy font
    tb->setFont(getFont());
    // Set target window
    tb->setTargetWindow(wnd);
    // Set index
    tb->setTabIndex(d_nextTabIndex++);
    // Instert into map
    d_tabButtonIndexMap.insert(
        TabButtonIndexMap::value_type(tb->getTabIndex(), tb));
    // add the button
    getTabButtonPane()->addChildWindow(tb);
    // Subscribe to clicked event so that we can change tab
    tb->subscribeEvent(TabButton::EventClicked, 
        Event::Subscriber(&TabControl::handleTabButtonClicked, this));

}

/*************************************************************************
	Return the tab button for the given tab content window
*************************************************************************/
TabButton* TabControl::getButtonForTabContents(Window* wnd) const
{
    TabButtonIndexMap::const_iterator i, iend;
    iend = d_tabButtonIndexMap.end();
    for (i = d_tabButtonIndexMap.begin(); i != iend; ++i)
    {
        // get corresponding tab button and content window
        TabButton* tb = i->second;
        Window* child = tb->getTargetWindow();
        if (child == wnd)
        {
			return tb;
        }
	}
	throw UnknownObjectException("TabControl::getButtonForTabContents - The Window object is not a tab contents.");
}
/*************************************************************************
	Calculate size and position for a tab button
*************************************************************************/
void TabControl::calculateTabButtonSizePosition(TabButton* btn, size_t targetIndex)
{
    // relative height is always 1.0 for buttons since they are embedded in a
    // panel of the correct height already
    btn->setWindowHeight(cegui_reldim(1.0f));
    btn->setWindowYPosition(cegui_absdim(0.0f));
    // x position is based on previous button
    if (targetIndex > 0)
    {
		TabButtonIndexMap::iterator iter = d_tabButtonIndexMap.begin();
		std::advance(iter, targetIndex - 1);
		Window* prevButton = iter->second;

		// position is prev pos + width
        btn->setWindowXPosition(prevButton->getWindowArea().d_max.d_x);
    }
    else
    {
        // First button
        btn->setWindowXPosition(cegui_absdim(0));
    }
    // Width is based on font size (expressed as absolute)
    Font* fnt = btn->getFont();
    btn->setWindowWidth(cegui_absdim(fnt->getTextExtent(btn->getText())) +
                                     getTabTextPadding() +
                                     getTabTextPadding());
    btn->requestRedraw();
}
/*************************************************************************
Remove tab button
*************************************************************************/
void TabControl::removeButtonForTabContent(Window* wnd)
{
    // get
    TabButton* tb = static_cast<TabButton*>(
        getTabButtonPane()->getChild(makeButtonName(wnd)));
    // remove
    d_tabButtonIndexMap.erase(tb->getTabIndex());
    getTabButtonPane()->removeChildWindow(tb);
	// destroy
	WindowManager::getSingleton().destroyWindow(tb);
}
/*************************************************************************
Remove tab button
*************************************************************************/
String TabControl::makeButtonName(Window* wnd)
{
    // derive button name
    String buttonName = TabButtonNameSuffix;
    buttonName.append(wnd->getName());
    return buttonName;
}
/*************************************************************************
Select tab implementation
*************************************************************************/
void TabControl::selectTab_impl(Window* wnd)
{
    bool modified = false;
    bool foundSelected = false;
    // Iterate in order of tab index
    TabButtonIndexMap::iterator i, iend;
    iend = d_tabButtonIndexMap.end();
    for (i = d_tabButtonIndexMap.begin(); i != iend; ++i)
    {
        // get corresponding tab button and content window
        TabButton* tb = i->second;
        Window* child = tb->getTargetWindow();
        // Should we be selecting?
        bool selectThis = (child == wnd);
        // Are we modifying this tab?
        modified = modified || (tb->isSelected() != selectThis);
        foundSelected = foundSelected || selectThis;
        // Select tab & set visible if this is the window, not otherwise
        tb->setSelected(selectThis);
        tb->setRightOfSelected(foundSelected);
        child->setVisible(selectThis);
    }
    // Trigger event?
    if (modified)
    {
        WindowEventArgs args(this);
        onSelectionChanged(args);
    }
}
/*************************************************************************
Add tab control properties
*************************************************************************/
void TabControl::addTabControlProperties(void)
{
    addProperty(&d_tabHeightProperty);
    addProperty(&d_tabTextPaddingProperty);
}
/*************************************************************************
Internal version of adding a child window
*************************************************************************/
void TabControl::addChild_impl(Window* wnd)
{
    // Look for __auto_TabPane__ in the name (hopefully no-one will use this!)
    if (wnd->getName().find(ContentPaneNameSuffix) != String::npos)
    {
        // perform normal addChild
        Window::addChild_impl(wnd);
    }
    else
    {
        // This is another control, therefore add as a tab
        addTab(wnd);
    }
}
/*************************************************************************
Internal version of removing a child window
*************************************************************************/
void TabControl::removeChild_impl(Window* wnd)
{
    // protect against possible null pointers
    if (!wnd) return;

    // Look for __auto_TabPane__ in the name (hopefully no-one will use this!)
    if (wnd->getName().find(ContentPaneNameSuffix) != String::npos)
    {
        // perform normal removeChild
        Window::removeChild_impl(wnd);
    }
    else
    {
        // This is some user window, therefore remove as a tab
        removeTab(wnd->getName());
    }
}
/*************************************************************************
Selection changed event
*************************************************************************/
void TabControl::onSelectionChanged(WindowEventArgs& e)
{
    requestRedraw();
    fireEvent(EventSelectionChanged, e, EventNamespace);
}
/*************************************************************************
Font changed event
*************************************************************************/
void TabControl::onFontChanged(WindowEventArgs& e)
{
    // Propagate font change to buttons
    TabButtonIndexMap::iterator i, iend;
    iend = d_tabButtonIndexMap.end();
    for (i = d_tabButtonIndexMap.end(); i != iend; ++i)
    {
        i->second->setFont(getFont());
    }
}
/*************************************************************************
Layout the widgets
*************************************************************************/
void TabControl::performChildWindowLayout()
{
    Window::performChildWindowLayout();

    Window* tabButtonPane = getTabButtonPane();
    TabPane* tabContentPane = getTabPane();

    // Set the size of the tab button area (full width, height from tab height)
    tabButtonPane->setWindowSize(
        UVector2(cegui_reldim(1.0f),
                    d_tabHeight) );

    tabButtonPane->setWindowPosition(
        UVector2(cegui_absdim(0),
                    cegui_absdim(0)));

    // Calculate the positions and sizes of the tab buttons
    TabButtonIndexMap::iterator i, iend;
    iend = d_tabButtonIndexMap.end();
    size_t x = 0;
    for (i = d_tabButtonIndexMap.begin(); i != iend; ++i, ++x)
    {
        TabButton* btn = i->second;
        calculateTabButtonSizePosition(btn, x);
    }

    // Set the size of the content area
    tabContentPane->setWindowSize(
        UVector2(cegui_reldim(1.0f),
                    cegui_reldim(1.0f) - d_tabHeight));

    tabContentPane->setWindowPosition(
        UVector2(cegui_reldim(0.0f), d_tabHeight) );

}
/*************************************************************************
Text changed on a content window
*************************************************************************/
bool TabControl::handleContentWindowTextChanged(const EventArgs& args)
{
    // update text
    const WindowEventArgs& wargs = static_cast<const WindowEventArgs&>(args);
    Window* tabButton = getTabButtonPane()->getChild(
        makeButtonName(wargs.window));
    tabButton->setText(wargs.window->getText());
    // sort out the layout
    performChildWindowLayout();
	requestRedraw();

	return true;
}
/*************************************************************************
Tab button clicked
*************************************************************************/
bool TabControl::handleTabButtonClicked(const EventArgs& args)
{
    const WindowEventArgs& wargs = static_cast<const WindowEventArgs&>(args);
    TabButton* tabButton = static_cast<TabButton*>(wargs.window);
    setSelectedTab(tabButton->getTargetWindow()->getName());

	return true;
}

/*************************************************************************
    Return a pointer to the tab button pane (Window)for
    this TabControl.
*************************************************************************/
Window* TabControl::getTabButtonPane() const
{
    return WindowManager::getSingleton().getWindow(getName() +
                                                   TabButtonPaneNameSuffix);
}

/*************************************************************************
    Return a pointer to the TabPane component widget for
    this TabControl.
*************************************************************************/
TabPane* TabControl::getTabPane() const
{
    return static_cast<TabPane*>(WindowManager::getSingleton().getWindow(
                                 getName() + ContentPaneNameSuffix));
}

int TabControl::writeChildWindowsXML(XMLSerializer& xml_stream) const
{
    int childOutputCount = Window::writeChildWindowsXML(xml_stream);

    // since TabControl content is actually added to the component tab
    // content pane window, this overridden function exists to dump those
    // out as if they were our own children.
    for (size_t i = 0; i < getTabCount(); ++i)
    {
        getTabContentsAtIndex(i)->writeXMLToStream(xml_stream);
        ++childOutputCount;
    }

    return childOutputCount;
}

/*************************************************************************
    create and return a pointer to a TabButton widget for use as a
    clickable tab header
*************************************************************************/
TabButton* TabControl::createTabButton(const String& name) const
{
    if (d_windowRenderer != 0)
    {
        TabControlWindowRenderer* wr = (TabControlWindowRenderer*)d_windowRenderer;
        return wr->createTabButton(name);
    }
    else
    {
        //return createTabButton_impl(name);
        throw InvalidRequestException("TabControl::createTabButton - This function must be implemented by the window renderer module");
    }
}

} // End of  CEGUI namespace section
