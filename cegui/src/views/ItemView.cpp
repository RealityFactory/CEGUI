/***********************************************************************
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Implementation of the base class for all item model-based views.
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
#include "CEGUI/views/ItemView.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/widgets/Tooltip.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
ItemViewWindowRenderer::ItemViewWindowRenderer(const String& name) :
    WindowRenderer(name, "ItemView")
{
}

//----------------------------------------------------------------------------//
ItemView* ItemViewWindowRenderer::getView() const
{
    return static_cast<ItemView*>(d_window);
}

//----------------------------------------------------------------------------//
const String& PropertyHelper<ScrollbarDisplayMode>::getDataTypeName()
{
    static String type("ScrollbarDisplayMode");

    return type;
}

//----------------------------------------------------------------------------//
PropertyHelper<ScrollbarDisplayMode>::return_type
PropertyHelper<ScrollbarDisplayMode>::fromString(const String& str)
{
    if (str == "Shown") return SDM_Shown;
    if (str == "Hidden") return SDM_Hidden;

    // default
    return SDM_WhenNeeded;
}

//----------------------------------------------------------------------------//
PropertyHelper<ScrollbarDisplayMode>::string_return_type
PropertyHelper<ScrollbarDisplayMode>::toString(pass_type val)
{
    switch(val)
    {
    case SDM_Shown: return "Shown";
    case SDM_Hidden: return "Hidden";
    case SDM_WhenNeeded: return "WhenNeeded";
    default: return "InvalidDisplayMode";
    }
}

//----------------------------------------------------------------------------//
const String& PropertyHelper<ViewSortMode>::getDataTypeName()
{
    static String type("ViewSortMode");

    return type;
}

//----------------------------------------------------------------------------//
PropertyHelper<ViewSortMode>::return_type
PropertyHelper<ViewSortMode>::fromString(const String& str)
{
    if (str == "Ascending") return VSM_Ascending;
    if (str == "Descending") return VSM_Descending;

    // default
    return VSM_None;
}

//----------------------------------------------------------------------------//
PropertyHelper<ViewSortMode>::string_return_type
PropertyHelper<ViewSortMode>::toString(pass_type val)
{
    switch (val)
    {
    case VSM_Ascending: return "Ascending";
    case VSM_Descending: return "Descending";
    default: return "None";
    }
}

//----------------------------------------------------------------------------//
const Colour ItemView::DefaultTextColour = 0xFFFFFFFF;
const Colour ItemView::DefaultSelectionColour = Colour(0xFF4444AA);
const String ItemView::HorzScrollbarName("__auto_hscrollbar__");
const String ItemView::VertScrollbarName("__auto_vscrollbar__");
const String ItemView::EventVertScrollbarDisplayModeChanged("VertScrollbarDisplayModeChanged");
const String ItemView::EventHorzScrollbarDisplayModeChanged("HorzScrollbarDisplayModeChanged");
const String ItemView::EventSelectionChanged("SelectionChanged");
const String ItemView::EventSortModeChanged("SortModeChanged");

//----------------------------------------------------------------------------//
ItemView::ItemView(const String& type, const String& name) :
    Window(type, name),
    d_itemModel(0),
    d_textColourRect(ColourRect(DefaultTextColour)),
    d_selectionColourRect(ColourRect(DefaultSelectionColour)),
    d_isDirty(true),
    d_needsFullRender(true),
    d_lastSelectedIndex(0),
    d_selectionBrush(0),
    d_vertScrollbarDisplayMode(SDM_WhenNeeded),
    d_horzScrollbarDisplayMode(SDM_WhenNeeded),
    d_isItemTooltipsEnabled(false),
    d_isMultiSelectEnabled(false),
    d_sortMode(VSM_None),
    d_renderedMaxWidth(0),
    d_renderedTotalHeight(0),
    d_eventChildrenAddedConnection(0),
    d_eventChildrenRemovedConnection(0)
{
    addItemViewProperties();
}

//----------------------------------------------------------------------------//
ItemView::~ItemView()
{
    disconnectModelEvents();
}

//----------------------------------------------------------------------------//
void ItemView::addItemViewProperties()
{
    const String& propertyOrigin = "ItemView";

    CEGUI_DEFINE_PROPERTY(ItemView, Image*,
        "SelectionBrushImage",
        "Property to get/set the selection brush image for the item view. Value should be \"set:[imageset name] image:[image name]\".",
        &ItemView::setSelectionBrushImage, &ItemView::getSelectionBrushImage, 0
        )

    CEGUI_DEFINE_PROPERTY(ItemView, ScrollbarDisplayMode,
        "VertScrollbarDisplayMode",
        "Property to get/set the display mode of the vertical scroll bar of the item view. Value can be \"Shown\", \"Hidden\" or \"WhenNeeded\".",
        &ItemView::setVertScrollbarDisplayMode,
        &ItemView::getVertScrollbarDisplayMode, SDM_WhenNeeded
        )

    CEGUI_DEFINE_PROPERTY(ItemView, ScrollbarDisplayMode,
        "HorzScrollbarDisplayMode",
        "Property to get/set the display mode of the horizontal scroll bar of the item view. Value can be \"Shown\", \"Hidden\" or \"WhenNeeded\".",
        &ItemView::setHorzScrollbarDisplayMode,
        &ItemView::getHorzScrollbarDisplayMode, SDM_WhenNeeded
        )

    CEGUI_DEFINE_PROPERTY(ItemView, bool,
        "ItemTooltips", "Property to access the show item tooltips setting of the item view. Value is either \"True\" or \"False\".",
        &ItemView::setItemTooltipsEnabled, &ItemView::isItemTooltipsEnabled, false
        )

    CEGUI_DEFINE_PROPERTY(ItemView, bool,
        "MultiSelect", "Property to get/set the multi-select setting of the item view. Value is either \"True\" or \"False\".",
        &ItemView::setMultiSelectEnabled, &ItemView::isMultiSelectEnabled, false
        )

    CEGUI_DEFINE_PROPERTY(ItemView, ViewSortMode,
        "SortMode", "Property to get/set how the itemview is sorting its items. Value is either \"None\", \"Ascending\" or \"Descending\".",
        &ItemView::setSortMode, &ItemView::getSortMode, VSM_None
        )
}

//----------------------------------------------------------------------------//
void ItemView::initialiseComponents(void)
{
    getVertScrollbar()->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&ItemView::onScrollPositionChanged, this));
    getHorzScrollbar()->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&ItemView::onScrollPositionChanged, this));

    performChildWindowLayout();
}

//----------------------------------------------------------------------------//
void ItemView::setModel(ItemModel* item_model)
{
    if (item_model == d_itemModel)
        return;

    if (d_itemModel != 0)
    {
        disconnectModelEvents();
        d_lastSelectedIndex = ModelIndex(0);
    }

    d_itemModel = item_model;

    connectToModelEvents(d_itemModel);
    d_indexSelectionStates.clear();
    d_needsFullRender = true;

    WindowEventArgs args(this);
    onSelectionChanged(args);
}

//----------------------------------------------------------------------------//
void ItemView::connectToModelEvents(ItemModel* d_itemModel)
{
    if (d_itemModel == 0)
        return;

    d_eventChildrenAddedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenAdded,
        &ItemView::onChildrenAdded, this);
    d_eventChildrenRemovedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenRemoved,
        &ItemView::onChildrenRemoved, this);
    d_eventChildrenDataChangedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenDataChanged,
        &ItemView::onChildrenDataChanged, this);
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenAdded(const EventArgs& args)
{
    const ModelEventArgs& model_args = static_cast<const ModelEventArgs&>(args);

    for (SelectionStatesVector::iterator itor = d_indexSelectionStates.begin();
        itor != d_indexSelectionStates.end(); ++itor)
    {
        ModelIndexSelectionState& state = *itor;

        if (state.d_childId >= model_args.d_startId &&
            d_itemModel->areIndicesEqual(state.d_parentIndex, model_args.d_parentIndex))
        {
            state.d_childId += model_args.d_count;
            state.d_selectedIndex = d_itemModel->makeIndex(state.d_childId, state.d_parentIndex);
        }
    }

    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenRemoved(const EventArgs& args)
{
    if (d_itemModel == 0)
        return false;

    const ModelEventArgs& model_args = static_cast<const ModelEventArgs&>(args);

    SelectionStatesVector::iterator itor = d_indexSelectionStates.begin();
    while (itor != d_indexSelectionStates.end())
    {
        ModelIndexSelectionState& state = *itor;

        if (state.d_childId >= model_args.d_startId &&
            state.d_childId <= model_args.d_startId + model_args.d_count)
        {
            if (d_itemModel->areIndicesEqual(d_lastSelectedIndex, state.d_selectedIndex))
                d_lastSelectedIndex = ModelIndex(0);

            itor = d_indexSelectionStates.erase(itor);
        }
        else
        {
            ++itor;
        }
    }

    WindowEventArgs wargs(this);
    onSelectionChanged(wargs);
    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenDataChanged(const EventArgs& args)
{
    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
void ItemView::onSelectionChanged(WindowEventArgs& args)
{
    invalidateView(false);
    fireEvent(EventSelectionChanged, args);
}

//----------------------------------------------------------------------------//
bool ItemView::onScrollPositionChanged(const EventArgs& args)
{
    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
void ItemView::onPointerPressHold(PointerEventArgs& e)
{
    if (e.source != PS_Left)
        return;

    handleSelection(e.position, true, false, false);

    ++e.handled;
    Window::onPointerPressHold(e);
}

//----------------------------------------------------------------------------//
void ItemView::onPointerMove(PointerEventArgs& e)
{
    if (d_isItemTooltipsEnabled)
        setupTooltip(e.position);

    ++e.handled;
    Window::onPointerMove(e);
}

//----------------------------------------------------------------------------//
void ItemView::disconnectModelEvents()
{
    if (d_eventChildrenAddedConnection != 0)
        d_eventChildrenAddedConnection->disconnect();

    if (d_eventChildrenRemovedConnection != 0)
        d_eventChildrenRemovedConnection->disconnect();

    if (d_eventChildrenDataChangedConnection != 0)
        d_eventChildrenDataChangedConnection->disconnect();
}

//----------------------------------------------------------------------------//
void ItemView::invalidateView(bool recursive)
{
    //TODO: allow invalidation only of certain parts (e.g.: items/indices)
    updateScrollbars();
    setIsDirty(true);
    invalidate(recursive);
}

//----------------------------------------------------------------------------//
ItemModel* ItemView::getModel() const
{
    return d_itemModel;
}

//----------------------------------------------------------------------------//
void ItemView::prepareForRender()
{
}

//----------------------------------------------------------------------------//
const ColourRect& ItemView::getTextColourRect() const
{
    return d_textColourRect;
}

//----------------------------------------------------------------------------//
void ItemView::setTextColourRect(const ColourRect& colour_rect)
{
    d_textColourRect = colour_rect;
}

//----------------------------------------------------------------------------//
const ColourRect& ItemView::getSelectionColourRect() const
{
    return d_selectionColourRect;
}

//----------------------------------------------------------------------------//
void ItemView::setSelectionColourRect(const ColourRect& colour_rect)
{
    d_selectionColourRect = colour_rect;
}

//----------------------------------------------------------------------------//
const std::vector<ModelIndexSelectionState>& ItemView::getIndexSelectionStates() const
{
    return d_indexSelectionStates;
}

//----------------------------------------------------------------------------//
bool ItemView::isDirty() const
{
    return d_isDirty;
}

//----------------------------------------------------------------------------//
void ItemView::setIsDirty(bool value)
{
    d_isDirty = value;
}

//----------------------------------------------------------------------------//
int ItemView::getSelectedIndexPosition(const ModelIndex& index) const
{
    if (d_itemModel == 0)
        return false;

    for (size_t i = 0; i < d_indexSelectionStates.size(); ++i)
    {
        if (d_itemModel->areIndicesEqual(index, d_indexSelectionStates.at(i).d_selectedIndex))
            return i;
    }

    return -1;
}

//----------------------------------------------------------------------------//
bool ItemView::isIndexSelected(const ModelIndex& index) const
{
    return getSelectedIndexPosition(index) != -1;
}

//----------------------------------------------------------------------------//
bool ItemView::setSelectedItem(const ModelIndex& index)
{
    // simple calls of this method shouldn't do cumulative selection
    return handleSelection(index, true, false, false);
}

//----------------------------------------------------------------------------//
bool ItemView::setItemSelectionState(const ModelIndex& index, bool selected)
{
    return handleSelection(index, selected, d_isMultiSelectEnabled, false);
}

//----------------------------------------------------------------------------//
void ItemView::setSelectionBrushImage(const String& name)
{
    setSelectionBrushImage(&ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void ItemView::setSelectionBrushImage(const Image* image)
{
    d_selectionBrush = image;
    invalidateView(false);
}

//----------------------------------------------------------------------------//
const Image* ItemView::getSelectionBrushImage(void) const
{
    return d_selectionBrush;
}

//----------------------------------------------------------------------------//
Scrollbar* ItemView::getVertScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(VertScrollbarName));
}

//----------------------------------------------------------------------------//
Scrollbar* ItemView::getHorzScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(HorzScrollbarName));
}

//----------------------------------------------------------------------------//
void ItemView::updateScrollbars()
{
    Rectf render_area = getViewRenderer()->getViewRenderArea();

    updateScrollbar(getVertScrollbar(), render_area.getHeight(),
        d_renderedTotalHeight, d_vertScrollbarDisplayMode);
    updateScrollbar(getHorzScrollbar(), render_area.getWidth(),
        d_renderedMaxWidth, d_horzScrollbarDisplayMode);
}

//----------------------------------------------------------------------------//
void ItemView::updateScrollbar(Scrollbar* scrollbar, float available_area,
    float rendered_area, ScrollbarDisplayMode display_mode)
{
    scrollbar->setDocumentSize(rendered_area);
    scrollbar->setPageSize(available_area);
    scrollbar->setStepSize(ceguimax(1.0f, rendered_area / 10.0f));
    scrollbar->setScrollPosition(scrollbar->getScrollPosition());

    if (display_mode == SDM_Hidden)
    {
        scrollbar->hide();
        return;
    }

    if (display_mode == SDM_Shown ||
        rendered_area > available_area)
    {
        scrollbar->show();
        return;
    }

    scrollbar->hide();
}

//----------------------------------------------------------------------------//
void ItemView::setVertScrollbarDisplayMode(ScrollbarDisplayMode mode)
{
    updateScrollbarDisplayMode(d_vertScrollbarDisplayMode, mode,
        EventVertScrollbarDisplayModeChanged);
}

//----------------------------------------------------------------------------//
ScrollbarDisplayMode ItemView::getVertScrollbarDisplayMode() const
{
    return d_vertScrollbarDisplayMode;
}

//----------------------------------------------------------------------------//
void ItemView::setHorzScrollbarDisplayMode(ScrollbarDisplayMode mode)
{
    updateScrollbarDisplayMode(d_horzScrollbarDisplayMode, mode,
        EventHorzScrollbarDisplayModeChanged);
}

//----------------------------------------------------------------------------//
ScrollbarDisplayMode ItemView::getHorzScrollbarDisplayMode() const
{
    return d_horzScrollbarDisplayMode;
}

//----------------------------------------------------------------------------//
ItemViewWindowRenderer* ItemView::getViewRenderer()
{
    if (d_windowRenderer == 0)
    {
        CEGUI_THROW(InvalidRequestException(
            "The view should have a window renderer attached!"));
    }

    return static_cast<ItemViewWindowRenderer*>(d_windowRenderer);
}

//----------------------------------------------------------------------------//
void ItemView::updateScrollbarDisplayMode(ScrollbarDisplayMode& target_mode,
    ScrollbarDisplayMode new_mode, String change_event)
{
    if (target_mode == new_mode)
        return;

    target_mode = new_mode;

    updateScrollbars();
    invalidateView(false);
    WindowEventArgs args(this);
    fireEvent(change_event, args);
}

//----------------------------------------------------------------------------//
void ItemView::onScroll(PointerEventArgs& e)
{
    handleOnScroll(getVertScrollbar(), e.scroll);

    ++e.handled;
    Window::onScroll(e);
}

//----------------------------------------------------------------------------//
void ItemView::handleOnScroll(Scrollbar* scrollbar, float scroll)
{
    if (scrollbar->isEffectiveVisible() &&
        scrollbar->getDocumentSize() > scrollbar->getPageSize())
    {
        scrollbar->setScrollPosition(
            scrollbar->getScrollPosition() + scrollbar->getStepSize() * -scroll);
    }
}

//----------------------------------------------------------------------------//
bool ItemView::isItemTooltipsEnabled() const
{
    return d_isItemTooltipsEnabled;
}

//----------------------------------------------------------------------------//
void ItemView::setItemTooltipsEnabled(bool enabled)
{
    d_isItemTooltipsEnabled = enabled;
}

//----------------------------------------------------------------------------//
void ItemView::setupTooltip(Vector2f position)
{
    if (d_itemModel == 0)
        return;

    static ModelIndex last_model_index;

    ModelIndex index = indexAt(position);
    if (d_itemModel->areIndicesEqual(index, last_model_index))
        return;

    Tooltip* tooltip = getTooltip();
    if (tooltip == 0)
        return;

    if (tooltip->getTargetWindow() != this)
        tooltip->setTargetWindow(this);
    else
        tooltip->positionSelf();

    last_model_index = index;

    if (!d_itemModel->isValidIndex(index))
        setTooltipText("");
    else
        setTooltipText(d_itemModel->getData(index, IDR_Tooltip));
}

//----------------------------------------------------------------------------//
bool ItemView::isMultiSelectEnabled() const
{
    return d_isMultiSelectEnabled;
}

//----------------------------------------------------------------------------//
void ItemView::setMultiSelectEnabled(bool enabled)
{
    d_isMultiSelectEnabled = enabled;
}

//----------------------------------------------------------------------------//
void ItemView::onSemanticInputEvent(SemanticEventArgs& e)
{
    if (e.d_semanticValue == SV_SelectRange ||
        e.d_semanticValue == SV_SelectCumulative)
    {
        handleSelection(getGUIContext().getPointerIndicator().getPosition(),
            true, d_isMultiSelectEnabled, e.d_semanticValue == SV_SelectRange);
    }

    handleSelectionNavigation(e);

    ++e.handled;
    Window::onSemanticInputEvent(e);
}

//----------------------------------------------------------------------------//
bool ItemView::handleSelection(const Vector2f& position, bool should_select,
    bool is_cumulative, bool is_range)
{
    return handleSelection(indexAt(position), should_select, is_cumulative, is_range);
}

//----------------------------------------------------------------------------//
bool ItemView::handleSelection(const ModelIndex& index, bool should_select,
    bool is_cumulative, bool is_range)
{
    if (d_itemModel == 0 ||
        !d_itemModel->isValidIndex(index))
        return false;

    int index_position = getSelectedIndexPosition(index);
    if (index_position != -1)
    {
        if (!should_select)
        {
            d_indexSelectionStates.erase(d_indexSelectionStates.begin() + index_position);

            WindowEventArgs args(this);
            onSelectionChanged(args);
            return true;
        }

        // if we select the node again, and we don't cumulate selection, we need
        // to make just that one be selected now
        if (is_cumulative)
            return true;
    }

    if (!is_cumulative)
        d_indexSelectionStates.clear();

    ModelIndex parent_index = d_itemModel->getParentIndex(index);
    size_t end_child_id = d_itemModel->getChildId(index);
    size_t start_child_id = end_child_id;
    if (is_range && is_cumulative && d_lastSelectedIndex.d_modelData != 0)
    {
        start_child_id = d_itemModel->getChildId(d_lastSelectedIndex);
    }

    for (size_t id = start_child_id; id <= end_child_id; ++id)
    {
        ModelIndexSelectionState selection_state;
        selection_state.d_selectedIndex = d_itemModel->makeIndex(id, parent_index);

        // ignore already selected indices
        if (getSelectedIndexPosition(selection_state.d_selectedIndex) != -1)
            continue;

        selection_state.d_childId = id;
        selection_state.d_parentIndex = parent_index;

        d_indexSelectionStates.push_back(selection_state);
    }

    d_lastSelectedIndex = index;

    WindowEventArgs args(this);
    onSelectionChanged(args);
    return true;
}

//----------------------------------------------------------------------------//
ViewSortMode ItemView::getSortMode() const
{
    return d_sortMode;
}

//----------------------------------------------------------------------------//
void ItemView::setSortMode(ViewSortMode sort_mode)
{
    if (d_sortMode == sort_mode)
        return;

    d_sortMode = sort_mode;

    resortView();

    WindowEventArgs args(this);
    onSortModeChanged(args);
}

//----------------------------------------------------------------------------//
void ItemView::onSortModeChanged(WindowEventArgs& args)
{
    invalidateView(false);
    //TODO: make all events be triggered on view's event namespace.
    fireEvent(EventSortModeChanged, args);
}

//----------------------------------------------------------------------------//
void ItemView::clearSelections()
{
    d_indexSelectionStates.clear();
}

//----------------------------------------------------------------------------//
void ItemView::handleSelectionNavigation(SemanticEventArgs& e)
{
    ModelIndex parent_index = d_itemModel->getRootIndex();
    int last_selected_child_id = -1;
    if (!d_indexSelectionStates.empty())
    {
        ModelIndexSelectionState last_selection = d_indexSelectionStates.back();
        last_selected_child_id = last_selection.d_childId;
        parent_index = last_selection.d_parentIndex;
    }

    size_t children_count = d_itemModel->getChildCount(parent_index);
    if (children_count == 0)
        return;

    int next_selected_child_id = last_selected_child_id;
    if (e.d_semanticValue == SV_GoDown)
    {
        next_selected_child_id = std::min(
            next_selected_child_id + 1,
            static_cast<int>(children_count)-1
            );
    }
    else if (e.d_semanticValue == SV_GoUp)
    {
        next_selected_child_id = std::max(0, next_selected_child_id - 1);
    }

    if (next_selected_child_id == -1 ||
        next_selected_child_id == last_selected_child_id)
        return;

    setSelectedItem(d_itemModel->makeIndex(
        static_cast<size_t>(next_selected_child_id), parent_index));
}

//----------------------------------------------------------------------------//
float ItemView::getRenderedMaxWidth() const
{
    return d_renderedMaxWidth;
}

//----------------------------------------------------------------------------//
float ItemView::getRenderedTotalHeight() const
{
    return d_renderedTotalHeight;
}
}
