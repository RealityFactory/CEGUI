/***********************************************************************
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Interface of the base class for all item model-based views.
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
#ifndef _CEGUIItemView_h_
#define _CEGUIItemView_h_

#include "CEGUI/Base.h"
#include "CEGUI/Colour.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/Rect.h"
#include "CEGUI/Window.h"
#include "CEGUI/views/ItemModel.h"
#include "CEGUI/widgets/Scrollbar.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class ItemView;

class CEGUIEXPORT ItemViewWindowRenderer : public WindowRenderer
{
public:
    ItemViewWindowRenderer(const String& name);

    /*!
    \brief
        Returns a Rectf object describing, in unclipped pixels, the window
        relative area that is to be used for rendering the view.
    */
    virtual Rectf getViewRenderArea(void) const = 0;

    /*!
    \brief
        Resize the view such that its content can be displayed without needing
        scrollbars if there is enough space, otherwise make the view as
        large as possible (without moving it).
    */
    virtual void resizeViewToContent(bool fit_width, bool fit_height) const = 0;

    inline virtual ItemView* getView() const;
};

enum ScrollbarDisplayMode
{
    SDM_Shown,
    SDM_Hidden,
    //! The scrollbar will be shown only if the underlining view's size is too small to contain its items
    SDM_WhenNeeded
};

enum ViewSortMode
{
    //! Items are not sorted, but shown in the same order as they are provided by the model
    VSM_None,
    VSM_Ascending,
    VSM_Descending
};

template<>
class CEGUIEXPORT PropertyHelper<ScrollbarDisplayMode>
{
public:
    typedef ScrollbarDisplayMode return_type;
    typedef return_type safe_method_return_type;
    typedef ScrollbarDisplayMode pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();

    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<ViewSortMode>
{
public:
    typedef ViewSortMode return_type;
    typedef return_type safe_method_return_type;
    typedef ViewSortMode pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();

    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

/*!
\brief
    Stores the selection state of a ModelIndex. This is used to regenerate
    the new proper selection index when the model changes in any way (e.g.:
    new item, removed item).
*/
struct CEGUIEXPORT ModelIndexSelectionState
{
    ModelIndex d_parentIndex;
    size_t d_childId;

    ModelIndex d_selectedIndex;
};
typedef std::vector<ModelIndexSelectionState> SelectionStatesVector;

class CEGUIEXPORT ItemViewEventArgs : public WindowEventArgs
{
public:
    ItemViewEventArgs(ItemView* wnd, ModelIndex index = ModelIndex());

    //! The index affected by the event.
    ModelIndex d_index;
};

/*!
\brief
    Abstract base class for all view classes based on ItemModel
*/
class CEGUIEXPORT ItemView : public Window
{
public:
    ItemView(const String& type, const String& name);

    virtual ~ItemView();

    static const Colour DefaultTextColour;
    static const Colour DefaultSelectionColour;
    //! Widget name for the vertical scrollbar component.
    static const String VertScrollbarName;
    //! Widget name for the horizontal scrollbar component
    static const String HorzScrollbarName;
    static const String EventVertScrollbarDisplayModeChanged;
    static const String EventHorzScrollbarDisplayModeChanged;
    static const String EventSelectionChanged;
    static const String EventMultiselectModeChanged;
    static const String EventSortModeChanged;
    //! Triggered when items are added, removed or when the view's item are cleared.
    static const String EventViewContentsChanged;

    //!Sets the ItemModel to be used inside this view.
    virtual void setModel(ItemModel* item_model);

    //! Returns the current ItemModel of this view.
    virtual ItemModel* getModel() const;

    /*!
    \brief
        Instructs this ItemView to prepare its rendering state for rendering.
        This is usually done by updating the rendering state if it got dirty
        in the meantime.
    */
    virtual void prepareForRender();

    //! Gets the colour used for rendering the text.
    const ColourRect& getTextColourRect() const;
    //! Sets the colour used for rendering the text.
    void setTextColourRect(const ColourRect& colour_rect);
    void setTextColour(Colour colour);

    //! Gets the colour used for higlighting the selection.
    const ColourRect& getSelectionColourRect() const;
    //! Sets the colour used for highlighting the selection.
    void setSelectionColourRect(const ColourRect& colour_rect);

    bool isDirty() const;
    //! Specifies whether this view requires processing before being able to render it.
    void setIsDirty(bool value);

    /*!
    \brief
       Invalidates this view by marking the rendering state as dirty.
       This also calls the base Window invalidate
    */
    virtual void invalidateView(bool recursive);

    /*!
    \brief
       Gets the current state of the indices used for selection.

    \remark
        This vector's iterator might get invalidated in the case when items are
        removed from the underlying ItemModel while iterating over this vector.

        In the following example you can see a way of removing the selecting items:
        \code
            while(!view->getIndexSelectionStates().empty())
            {
                ModelIndexSelectionState& state = view->getIndexSelectionStates().back();
                // remove item from model
            }
        \endcode
    */
    const std::vector<ModelIndexSelectionState>& getIndexSelectionStates() const;

    /*!
    \brief
        Returns the ModelIndex at the specified position.

    \return
        The ModelIndex for the position or a default-constructed ModelIndex
        if nothing was found at that position.
    */
    virtual ModelIndex indexAt(const Vector2f& position) = 0;

    /*!
    \brief
        Sets the specified item index's as the currently selected item.

    \return
        True if the item has been successfully selected, false otherwise.
    */
    virtual bool setSelectedItem(const ModelIndex& index);
    virtual bool setItemSelectionState(const ModelIndex& index, bool selected);

    /*!
    \brief
        Ensures that the item specified by the \a index is visible by setting
        the proper the vertical scrollbar's position.
    */
    virtual void ensureItemIsVisible(const ModelIndex& index);

    //! Clears all selected items
    void clearSelections();

    virtual bool isIndexSelected(const ModelIndex& index) const;

    void setSelectionBrushImage(const Image* image);
    void setSelectionBrushImage(const String& name);
    const Image* getSelectionBrushImage(void) const;

    /*!
    \brief
        Returns a pointer to the vertical scrollbar component widget for this view.

    \exception UnknownObjectException
        Thrown if the vertical Scrollbar component does not exist.
    */
    Scrollbar* getVertScrollbar() const;

    void setVertScrollbarDisplayMode(ScrollbarDisplayMode mode);
    ScrollbarDisplayMode getVertScrollbarDisplayMode() const;

    /*!
    \brief
        Returns a pointer to the horizontal scrollbar component widget for this view.

    \exception UnknownObjectException
        Thrown if the horizontal Scrollbar component does not exist.
    */
    Scrollbar* getHorzScrollbar() const;

    void setHorzScrollbarDisplayMode(ScrollbarDisplayMode mode);
    ScrollbarDisplayMode getHorzScrollbarDisplayMode() const;

    void setAutoResizeHeightEnabled(bool enabled);
    bool isAutoResizeHeightEnabled() const;

    void setAutoResizeWidthEnabled(bool enabled);
    bool isAutoResizeWidthEnabled() const;

    //! Specifies whether this view has separate tooltips for each item or not.
    bool isItemTooltipsEnabled() const;
    void setItemTooltipsEnabled(bool enabled);

    bool isMultiSelectEnabled() const;
    void setMultiSelectEnabled(bool enabled);

    ViewSortMode getSortMode() const;
    void setSortMode(ViewSortMode sort_mode);

    //! Returns the width of the rendered contents.
    float getRenderedMaxWidth() const;
    //! Returns the height of the rendered contents.
    float getRenderedTotalHeight() const;

protected:
    ItemModel* d_itemModel;
    ColourRect d_textColourRect;
    ColourRect d_selectionColourRect;
    bool d_isDirty;
    bool d_needsFullRender;
    std::vector<ModelIndexSelectionState> d_indexSelectionStates;
    ModelIndex d_lastSelectedIndex;
    const Image* d_selectionBrush;
    ScrollbarDisplayMode d_vertScrollbarDisplayMode;
    ScrollbarDisplayMode d_horzScrollbarDisplayMode;
    bool d_isItemTooltipsEnabled;
    bool d_isMultiSelectEnabled;
    ViewSortMode d_sortMode;
    bool d_isAutoResizeHeightEnabled;
    bool d_isAutoResizeWidthEnabled;

    //TODO: move this into the renderer instead?
    float d_renderedMaxWidth;
    float d_renderedTotalHeight;

    void addItemViewProperties();
    virtual void updateScrollbars();
    void updateScrollbar(Scrollbar* scrollbar, float available_area,
        float rendered_area, ScrollbarDisplayMode display_mode);
    virtual ItemViewWindowRenderer* getViewRenderer();
    void updateScrollbarDisplayMode(ScrollbarDisplayMode& target_mode,
        ScrollbarDisplayMode new_mode, String change_event);

    virtual void initialiseComponents(void);

    virtual bool onChildrenAdded(const EventArgs& args);
    virtual bool onChildrenRemoved(const EventArgs& args);
    virtual bool onChildrenDataChanged(const EventArgs& args);
    virtual bool onScrollPositionChanged(const EventArgs& args);
    virtual void onSelectionChanged(ItemViewEventArgs& args);
    virtual void onMultiselectModeChanged(WindowEventArgs& args);
    virtual void onSortModeChanged(WindowEventArgs& args);
    virtual void onViewContentsChanged(WindowEventArgs& args);

    virtual void onScroll(PointerEventArgs& e);
    virtual void onPointerPressHold(PointerEventArgs& e);
    virtual void onPointerMove(PointerEventArgs& e);
    virtual void onSemanticInputEvent(SemanticEventArgs& e);
    virtual void onParentSized(ElementEventArgs& e);

    Event::Connection d_eventChildrenAddedConnection;
    Event::Connection d_eventChildrenRemovedConnection;
    Event::Connection d_eventChildrenDataChangedConnection;
    void connectToModelEvents(ItemModel* d_itemModel);
    void disconnectModelEvents();

    void handleOnScroll(Scrollbar* scrollbar, float scroll);
    void setupTooltip(Vector2f position);
    int getSelectedIndexPosition(const ModelIndex& index) const;
    virtual bool handleSelection(const Vector2f& position, bool should_select,
        bool is_cumulative, bool is_range);
    virtual bool handleSelection(const ModelIndex& index, bool should_select,
        bool is_cumulative, bool is_range);

    virtual void resortView() = 0;
    void handleSelectionNavigation(SemanticEventArgs& e);

    //! Returns the Rectf that contains the specified \a index.
    virtual Rectf getIndexRect(const ModelIndex& index) = 0;

    void updateAutoResizeFlag(bool& flag, bool enabled);
    void resizeToContent();
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
