/***********************************************************************
    filename:   CEGUIScrollablePane.cpp
    created:    1/3/2005
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
#include "elements/CEGUIScrollablePane.h"
#include "elements/CEGUIScrolledContainer.h"
#include "elements/CEGUIScrollbar.h"
#include "CEGUIWindowManager.h"
#include "CEGUIExceptions.h"
#include <math.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String ScrollablePane::WidgetTypeName("CEGUI/ScrollablePane");
const String ScrollablePane::EventNamespace("ScrollablePane");
const String ScrollablePane::EventContentPaneChanged("ContentPaneChanged");
const String ScrollablePane::EventVertScrollbarModeChanged("VertScrollbarModeChanged");
const String ScrollablePane::EventHorzScrollbarModeChanged("HorzScrollbarModeChanged");
const String ScrollablePane::EventAutoSizeSettingChanged("AutoSizeSettingChanged");
const String ScrollablePane::EventContentPaneScrolled("ContentPaneScrolled");
const String ScrollablePane::VertScrollbarName( "__auto_vscrollbar__" );
const String ScrollablePane::HorzScrollbarName( "__auto_hscrollbar__" );
const String ScrollablePane::ScrolledContainerName( "__auto_container__" );
//----------------------------------------------------------------------------//
ScrollablePaneWindowRenderer::ScrollablePaneWindowRenderer(const String& name) :
WindowRenderer(name, ScrollablePane::EventNamespace)
{
}

//----------------------------------------------------------------------------//
ScrollablePane::ScrollablePane(const String& type, const String& name) :
    Window(type, name),
    d_forceVertScroll(false),
    d_forceHorzScroll(false),
    d_contentRect(0, 0, 0, 0),
    d_vertStep(0.1f),
    d_vertOverlap(0.01f),
    d_horzStep(0.1f),
    d_horzOverlap(0.01f)
{
    addScrollablePaneProperties();
    
    // create scrolled container widget
    ScrolledContainer* container = static_cast<ScrolledContainer*>(
        WindowManager::getSingleton().createWindow(
            ScrolledContainer::WidgetTypeName,
            ScrolledContainerName));

    // add scrolled container widget as child
    addChild(container);
}

//----------------------------------------------------------------------------//
ScrollablePane::~ScrollablePane(void)
{}

//----------------------------------------------------------------------------//
const ScrolledContainer* ScrollablePane::getContentPane(void) const
{
    return getScrolledContainer();
}

//----------------------------------------------------------------------------//
bool ScrollablePane::isVertScrollbarAlwaysShown(void) const
{
    return d_forceVertScroll;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setShowVertScrollbar(bool setting)
{
    if (d_forceVertScroll != setting)
    {
        d_forceVertScroll = setting;
        
        configureScrollbars();
        WindowEventArgs args(this);
        onVertScrollbarModeChanged(args);
    }
}

//----------------------------------------------------------------------------//
bool ScrollablePane::isHorzScrollbarAlwaysShown(void) const
{
    return d_forceHorzScroll;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setShowHorzScrollbar(bool setting)
{
    if (d_forceHorzScroll != setting)
    {
        d_forceHorzScroll = setting;
        
        configureScrollbars();
        WindowEventArgs args(this);
        onHorzScrollbarModeChanged(args);
    }
}

//----------------------------------------------------------------------------//
bool ScrollablePane::isContentPaneAutoSized(void) const
{
    return getScrolledContainer()->isContentPaneAutoSized();
}

//----------------------------------------------------------------------------//
void ScrollablePane::setContentPaneAutoSized(bool setting)
{
    getScrolledContainer()->setContentPaneAutoSized(setting);
}

//----------------------------------------------------------------------------//
const Rectf& ScrollablePane::getContentPaneArea(void) const
{
    return getScrolledContainer()->getContentArea();
}

//----------------------------------------------------------------------------//
void ScrollablePane::setContentPaneArea(const Rectf& area)
{
    getScrolledContainer()->setContentArea(area);
}

//----------------------------------------------------------------------------//
float ScrollablePane::getHorizontalStepSize(void) const
{
    return d_horzStep;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setHorizontalStepSize(float step)
{
    d_horzStep = step;
    configureScrollbars();
}

//----------------------------------------------------------------------------//
float ScrollablePane::getHorizontalOverlapSize(void) const
{
    return d_horzOverlap;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setHorizontalOverlapSize(float overlap)
{
    d_horzOverlap = overlap;
    configureScrollbars();
}

//----------------------------------------------------------------------------//
float ScrollablePane::getHorizontalScrollPosition(void) const
{
    Scrollbar* horzScrollbar = getHorzScrollbar();
    float docSz = horzScrollbar->getDocumentSize();
    return (docSz != 0) ? horzScrollbar->getScrollPosition() / docSz : 0.0f;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setHorizontalScrollPosition(float position)
{
    Scrollbar* horzScrollbar = getHorzScrollbar();
    horzScrollbar->setScrollPosition(
                        horzScrollbar->getDocumentSize() * position);
}

//----------------------------------------------------------------------------//
float ScrollablePane::getVerticalStepSize(void) const
{
    return d_vertStep;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setVerticalStepSize(float step)
{
    d_vertStep = step;
    configureScrollbars();
}

//----------------------------------------------------------------------------//
float ScrollablePane::getVerticalOverlapSize(void) const
{
    return d_vertOverlap;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setVerticalOverlapSize(float overlap)
{
    d_vertOverlap = overlap;
    configureScrollbars();
}

//----------------------------------------------------------------------------//
float ScrollablePane::getVerticalScrollPosition(void) const
{
    Scrollbar* vertScrollbar = getVertScrollbar();
    float docSz = vertScrollbar->getDocumentSize();
    return (docSz != 0) ? vertScrollbar->getScrollPosition() / docSz : 0.0f;
}

//----------------------------------------------------------------------------//
void ScrollablePane::setVerticalScrollPosition(float position)
{
    Scrollbar* vertScrollbar = getVertScrollbar();
    vertScrollbar->setScrollPosition(
                        vertScrollbar->getDocumentSize() * position);
}

//----------------------------------------------------------------------------//
void ScrollablePane::initialiseComponents(void)
{
    // get horizontal scrollbar
    Scrollbar* horzScrollbar = getHorzScrollbar();
    
    // get vertical scrollbar
    Scrollbar* vertScrollbar = getVertScrollbar();
    
    // get scrolled container widget
    ScrolledContainer* container = getScrolledContainer();
    
    // do a bit of initialisation
    horzScrollbar->setAlwaysOnTop(true);
    vertScrollbar->setAlwaysOnTop(true);
    // container pane is always same size as this parent pane,
    // scrolling is actually implemented via positioning and clipping tricks.
    container->setSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    
    // subscribe to events we need to hear about
    vertScrollbar->subscribeEvent(
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&ScrollablePane::handleScrollChange, this));

    horzScrollbar->subscribeEvent(
            Scrollbar::EventScrollPositionChanged,
            Event::Subscriber(&ScrollablePane::handleScrollChange, this));

    d_contentChangedConn = container->subscribeEvent(
            ScrolledContainer::EventContentChanged,
            Event::Subscriber(&ScrollablePane::handleContentAreaChange, this));

    d_autoSizeChangedConn = container->subscribeEvent(
            ScrolledContainer::EventAutoSizeSettingChanged,
            Event::Subscriber(&ScrollablePane::handleAutoSizePaneChanged, this));
    
    // finalise setup
    configureScrollbars();
}

//----------------------------------------------------------------------------//
void ScrollablePane::configureScrollbars(void)
{
    // controls should all be valid by this stage
    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();
    
    // enable required scrollbars
    vertScrollbar->setVisible(isVertScrollbarNeeded());
    horzScrollbar->setVisible(isHorzScrollbarNeeded());
    
    // Check if the addition of the horizontal scrollbar means we
    // now also need the vertical bar.
    if (horzScrollbar->isEffectiveVisible())
    {
        vertScrollbar->setVisible(isVertScrollbarNeeded());
    }
    
    performChildWindowLayout();
    
    // get viewable area
    Rectf viewableArea(getViewableArea());
    
    // set up vertical scroll bar values
    vertScrollbar->setDocumentSize(fabsf(d_contentRect.getHeight()));
    vertScrollbar->setPageSize(viewableArea.getHeight());
    vertScrollbar->setStepSize(ceguimax(1.0f, viewableArea.getHeight() * d_vertStep));
    vertScrollbar->setOverlapSize(ceguimax(1.0f, viewableArea.getHeight() * d_vertOverlap));
    vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition());
    
    // set up horizontal scroll bar values
    horzScrollbar->setDocumentSize(fabsf(d_contentRect.getWidth()));
    horzScrollbar->setPageSize(viewableArea.getWidth());
    horzScrollbar->setStepSize(ceguimax(1.0f, viewableArea.getWidth() * d_horzStep));
    horzScrollbar->setOverlapSize(ceguimax(1.0f, viewableArea.getWidth() * d_horzOverlap));
    horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition());
}

//----------------------------------------------------------------------------//
bool ScrollablePane::isHorzScrollbarNeeded(void) const
{
    return ((fabs(d_contentRect.getWidth()) > getViewableArea().getWidth()) ||
            d_forceHorzScroll);
}

//----------------------------------------------------------------------------//
bool ScrollablePane::isVertScrollbarNeeded(void) const
{
    return ((fabs(d_contentRect.getHeight()) > getViewableArea().getHeight()) ||
            d_forceVertScroll);
}

//----------------------------------------------------------------------------//
void ScrollablePane::updateContainerPosition(void)
{
    // basePos is the position represented by the scrollbars
    // (these are negated so pane is scrolled in the correct directions)
    UVector2 basePos(cegui_absdim(-getHorzScrollbar()->getScrollPosition()),
                     cegui_absdim(-getVertScrollbar()->getScrollPosition()));
    
    // this bias is the absolute position that 0 on the scrollbars represent.
    // Allows the pane to function correctly with negatively positioned content.
    UVector2 bias(cegui_absdim(d_contentRect.d_min.d_x),
                  cegui_absdim(d_contentRect.d_min.d_y));
    
    // set the new container pane position to be what the scrollbars request
    // minus any bias generated by the location of the content.
    getScrolledContainer()->setPosition(basePos - bias);
}

//----------------------------------------------------------------------------//
void ScrollablePane::onContentPaneChanged(WindowEventArgs& e)
{
    fireEvent(EventContentPaneChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ScrollablePane::onVertScrollbarModeChanged(WindowEventArgs& e)
{
    fireEvent(EventVertScrollbarModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ScrollablePane::onHorzScrollbarModeChanged(WindowEventArgs& e)
{
    fireEvent(EventHorzScrollbarModeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ScrollablePane::onAutoSizeSettingChanged(WindowEventArgs& e)
{
    fireEvent(EventAutoSizeSettingChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ScrollablePane::onContentPaneScrolled(WindowEventArgs& e)
{
    updateContainerPosition();
    fireEvent(EventContentPaneScrolled, e, EventNamespace);
}

//----------------------------------------------------------------------------//
bool ScrollablePane::handleScrollChange(const EventArgs&)
{
    WindowEventArgs args(this);
    onContentPaneScrolled(args);
    return true;
}

//----------------------------------------------------------------------------//
bool ScrollablePane::handleContentAreaChange(const EventArgs&)
{
    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();
    
    // get updated extents of the content
    Rectf contentArea(getScrolledContainer()->getContentArea());
    
    // calculate any change on the top and left edges.
    float xChange = contentArea.d_min.d_x - d_contentRect.d_min.d_x;
    float yChange = contentArea.d_min.d_y - d_contentRect.d_min.d_y;
    
    // store new content extents information
    d_contentRect = contentArea;
    
    configureScrollbars();
    
    // update scrollbar positions (which causes container pane to be moved as needed).
    horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() - xChange);
    vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() - yChange);
    
    // this call may already have been made if the scroll positions changed.  The call
    // is required here for cases where the top/left 'bias' has changed; in which
    // case the scroll position notification may or may not have been fired.
    if (xChange || yChange)
        updateContainerPosition();
    
    // fire event
    WindowEventArgs args(this);
    onContentPaneChanged(args);
    
    return true;
}

//----------------------------------------------------------------------------//
bool ScrollablePane::handleAutoSizePaneChanged(const EventArgs&)
{
    // just forward event to client.
    WindowEventArgs args(this);
    fireEvent(EventAutoSizeSettingChanged, args, EventNamespace);
    return args.handled;
}

//----------------------------------------------------------------------------//
void ScrollablePane::addChild_impl(Window* wnd)
{
    // null is not a valid window pointer!
    assert(wnd != 0);
    
    // See if this is an internally generated window
    // (will have AutoWidgetNameSuffix in the name)
    if (wnd->getName().find(AutoWidgetNameSuffix) != String::npos)
    {
        // This is an internal widget, so should be added normally.
        Window::addChild_impl(wnd);
    }
    // this is a client window/widget, so should be added to the pane container.
    else
    {
        // container should always be valid by the time we're adding client
        // controls
        getScrolledContainer()->addChild(wnd);
    }
}

//----------------------------------------------------------------------------//
void ScrollablePane::removeChild_impl(Window* wnd)
{
    // null is not a valid window pointer!
    assert(wnd != 0);
    
    // See if this is an internally generated window
    // (will have AutoWidgetNameSuffix in the name)
    if (wnd->getName().find(AutoWidgetNameSuffix) != String::npos)
    {
        // This is an internal widget, so should be removed normally.
        Window::removeChild_impl(wnd);
    }
    // this is a client window/widget, so should be removed from the pane
    // container.
    else
    {
        // container should always be valid by the time we're handling client
        // controls
        getScrolledContainer()->removeChild(wnd);
    }
}

//----------------------------------------------------------------------------//
void ScrollablePane::onSized(WindowEventArgs& e)
{
    Window::onSized(e);
    configureScrollbars();
    updateContainerPosition();
    
    ++e.handled;
}

//----------------------------------------------------------------------------//
void ScrollablePane::onMouseWheel(MouseEventArgs& e)
{
    // base class processing.
    Window::onMouseWheel(e);
    
    Scrollbar* vertScrollbar = getVertScrollbar();
    Scrollbar* horzScrollbar = getHorzScrollbar();
    
    if (vertScrollbar->isEffectiveVisible() &&
        (vertScrollbar->getDocumentSize() > vertScrollbar->getPageSize()))
    {
        vertScrollbar->setScrollPosition(vertScrollbar->getScrollPosition() +
                            vertScrollbar->getStepSize() * -e.wheelChange);
    }
    else if (horzScrollbar->isEffectiveVisible() &&
             (horzScrollbar->getDocumentSize() > horzScrollbar->getPageSize()))
    {
        horzScrollbar->setScrollPosition(horzScrollbar->getScrollPosition() +
                            horzScrollbar->getStepSize() * -e.wheelChange);
    }
    
    ++e.handled;
}

//----------------------------------------------------------------------------//
void ScrollablePane::addScrollablePaneProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(ScrollablePane, bool,
        "ForceVertScrollbar", "Property to get/set the 'always show' setting for the vertical scroll "
        "bar of the tree.  Value is either \"True\" or \"False\".",
        &ScrollablePane::setShowVertScrollbar, &ScrollablePane::isVertScrollbarAlwaysShown, false /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, bool,
        "ForceHorzScrollbar", "Property to get/set the 'always show' setting for the horizontal "
        "scroll bar of the tree.  Value is either \"True\" or \"False\".",
        &ScrollablePane::setShowHorzScrollbar, &ScrollablePane::isHorzScrollbarAlwaysShown, false /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "HorzStepSize", "Property to get/set the step size for the horizontal Scrollbar.  Value is a float.",
        &ScrollablePane::setHorizontalStepSize, &ScrollablePane::getHorizontalStepSize, 0.1f /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "HorzOverlapSize", "Property to get/set the overlap size for the horizontal Scrollbar.  Value is a float.",
        &ScrollablePane::setHorizontalOverlapSize, &ScrollablePane::getHorizontalOverlapSize, 0.01f /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "HorzScrollPosition", "Property to get/set the scroll position of the horizontal Scrollbar as a fraction.  Value is a float.",
        &ScrollablePane::setHorizontalScrollPosition, &ScrollablePane::getHorizontalScrollPosition, 0.0f
    );

    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "VertStepSize", "Property to get/set the step size for the vertical Scrollbar.  Value is a float.",
        &ScrollablePane::setVerticalStepSize, &ScrollablePane::getVerticalStepSize, 0.1f /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "VertOverlapSize", "Property to get/set the overlap size for the vertical Scrollbar.  Value is a float.",
        &ScrollablePane::setVerticalOverlapSize, &ScrollablePane::getVerticalOverlapSize, 0.01f /* TODO: Inconsistency */
    );
    
    CEGUI_DEFINE_PROPERTY(ScrollablePane, float,
        "VertScrollPosition", "Property to get/set the scroll position of the vertical Scrollbar as a fraction.  Value is a float.",
        &ScrollablePane::setVerticalScrollPosition, &ScrollablePane::getVerticalScrollPosition, 0.0f /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(ScrollablePane, bool,
        "ContentPaneAutoSized", "Property to get/set the setting which controls whether the content pane will auto-size itself.  Value is either \"True\" or \"False\".",
        &ScrollablePane::setContentPaneAutoSized, &ScrollablePane::isContentPaneAutoSized, true
    );

    CEGUI_DEFINE_PROPERTY(ScrollablePane, Rectf,
        "ContentArea", "Property to get/set the current content area rectangle of the content pane.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom).",
        &ScrollablePane::setContentPaneArea, &ScrollablePane::getContentPaneArea, Rectf::zero() /* TODO: Inconsistency */
    );
}

//----------------------------------------------------------------------------//
Scrollbar* ScrollablePane::getVertScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(VertScrollbarName));
}

//----------------------------------------------------------------------------//
Scrollbar* ScrollablePane::getHorzScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(HorzScrollbarName));
}

//----------------------------------------------------------------------------//
ScrolledContainer* ScrollablePane::getScrolledContainer() const
{
    return static_cast<ScrolledContainer*>(getChild(ScrolledContainerName));
}

//----------------------------------------------------------------------------//
Rectf ScrollablePane::getViewableArea() const
{
    if (!d_windowRenderer)
        CEGUI_THROW(InvalidRequestException("ScrollablePane::getViewableArea: "
            "This function must be implemented by the window renderer module"));
    
    ScrollablePaneWindowRenderer* wr =
        static_cast<ScrollablePaneWindowRenderer*>(d_windowRenderer);
    return wr->getViewableArea();
}

//----------------------------------------------------------------------------//
void ScrollablePane::destroy(void)
{
    // detach from events on content pane
    d_contentChangedConn->disconnect();
    d_autoSizeChangedConn->disconnect();
    
    // now do the cleanup
    Window::destroy();
}

//----------------------------------------------------------------------------//
    
} // End of  CEGUI namespace section
