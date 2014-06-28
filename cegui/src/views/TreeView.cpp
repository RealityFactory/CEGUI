/***********************************************************************
    filename:   TreeView.cpp
    created:    Fri Jun 06 2014
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
#include "CEGUI/views/TreeView.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
TreeViewWindowRenderer::TreeViewWindowRenderer(const String& type) :
    ItemViewWindowRenderer(type)
{
}

//----------------------------------------------------------------------------//
const String TreeView::EventNamespace("TreeView");
const String TreeView::WidgetTypeName("CEGUI/TreeView");

//----------------------------------------------------------------------------//
TreeViewItemRenderingState::TreeViewItemRenderingState() :
    d_totalChildCount(0),
    d_size(0, 0),
    d_isSelected(false),
    d_childId(0),
    d_subtreeIsExpanded(false)
{
}

//----------------------------------------------------------------------------//
TreeView::TreeView(const String& type, const String& name) :
    ItemView(type, name)
{
}

//----------------------------------------------------------------------------//
TreeView::~TreeView()
{
}

//----------------------------------------------------------------------------//
const TreeViewItemRenderingState& TreeView::getRootItemState() const
{
    return d_rootItemState;
}

//----------------------------------------------------------------------------//
void TreeView::prepareForRender()
{
    ItemView::prepareForRender();
    //TODO: better way of ignoring the null item model? E.g.: warn? Throw an exception?
    if (d_itemModel == 0 || !isDirty())
        return;

    if (d_needsFullRender)
    {
        ModelIndex root_index = d_itemModel->getRootIndex();
        d_renderedMaxWidth = 0;
        d_renderedTotalHeight = 0;

        d_rootItemState = TreeViewItemRenderingState();
        d_rootItemState.d_subtreeIsExpanded = true;

        computeRenderedChildrenForItem(d_rootItemState, root_index,
            d_renderedMaxWidth, d_renderedTotalHeight);
    }
    else
    {
        updateRenderingStateForItem(d_rootItemState,
            d_renderedMaxWidth, d_renderedTotalHeight);
    }

    updateScrollbars();
    setIsDirty(false);
    d_needsFullRender = false;
}

//----------------------------------------------------------------------------//
bool TreeView::handleSelection(const Vector2f& position, bool should_select,
    bool is_cumulative, bool is_range)
{
    return handleSelection(
        indexAtWithAction(position, &TreeView::handleSelectionAction),
        should_select, is_cumulative, is_range);
}

//----------------------------------------------------------------------------//
bool TreeView::handleSelection(const ModelIndex& index, bool should_select,
    bool is_cumulative, bool is_range)
{
    return ItemView::handleSelection(index, should_select, is_cumulative, is_range);
}

//----------------------------------------------------------------------------//
TreeViewItemRenderingState TreeView::computeRenderingStateForIndex(
    const ModelIndex& index, float& rendered_max_width,
    float& rendered_total_height)
{
    TreeViewItemRenderingState state;
    fillRenderingState(state, index, rendered_max_width, rendered_total_height);

    computeRenderedChildrenForItem(state, index, rendered_max_width,
        rendered_total_height);

    return state;
}

//----------------------------------------------------------------------------//
void TreeView::computeRenderedChildrenForItem(TreeViewItemRenderingState &item,
    const ModelIndex& index, float& rendered_max_width, float& rendered_total_height)
{
    size_t child_count = d_itemModel->getChildCount(index);
    item.d_totalChildCount = child_count;

    if (!item.d_subtreeIsExpanded)
        return;

    for (size_t child = 0; child < child_count; ++child)
    {
        ModelIndex child_index = d_itemModel->makeIndex(child, index);
        TreeViewItemRenderingState child_state =
            computeRenderingStateForIndex(child_index, rendered_max_width,
            rendered_total_height);
        child_state.d_parentIndex = index;
        child_state.d_childId = child;
        item.d_renderedChildren.push_back(child_state);
    }
}

//----------------------------------------------------------------------------//
void TreeView::updateRenderingStateForItem(TreeViewItemRenderingState& item,
    float& rendered_max_width, float& rendered_total_height)
{
    // subtract the previous height
    rendered_total_height -= item.d_size.d_height;

    fillRenderingState(item,
        d_itemModel->makeIndex(item.d_childId, item.d_parentIndex),
        rendered_max_width, rendered_total_height);

    for (ItemStateVector::iterator itor = item.d_renderedChildren.begin();
        itor != item.d_renderedChildren.end(); ++itor)
    {
        updateRenderingStateForItem(*itor, rendered_max_width, rendered_total_height);
    }
}

//----------------------------------------------------------------------------//
void TreeView::fillRenderingState(TreeViewItemRenderingState& item,
    const ModelIndex& index, float& rendered_max_width, float& rendered_total_height)
{
    String text = d_itemModel->getData(index);
    RenderedString rendered_string = getRenderedStringParser().parse(
        text, getFont(), &d_textColourRect);
    item.d_string = rendered_string;
    item.d_text = text;

    item.d_size = Sizef(
        rendered_string.getHorizontalExtent(this),
        rendered_string.getVerticalExtent(this));

    rendered_max_width = ceguimax(rendered_max_width, item.d_size.d_width);
    rendered_total_height += item.d_size.d_height;

    item.d_isSelected = isIndexSelected(index);
}

//----------------------------------------------------------------------------//
ModelIndex TreeView::indexAt(const Vector2f& position)
{
    return indexAtWithAction(position, &TreeView::noopAction);
}

//----------------------------------------------------------------------------//
CEGUI::ModelIndex TreeView::indexAtWithAction(const Vector2f& position,
    TreeViewItemAction action)
{
    if (d_itemModel == 0)
        return ModelIndex();

    //TODO: add prepareForLayout() as a cheaper operation alternative?
    prepareForRender();

    Vector2f window_position = CoordConverter::screenToWindow(*this, position);
    Rectf render_area(getViewRenderer()->getViewRenderArea());
    if (!render_area.isPointInRect(window_position))
        return ModelIndex();

    float cur_height = render_area.d_min.d_y - getVertScrollbar()->getScrollPosition();
    bool handled = false;
    return indexAtRecursive(d_rootItemState, cur_height, window_position,
        handled, action);
}

//----------------------------------------------------------------------------//
ModelIndex TreeView::indexAtRecursive(TreeViewItemRenderingState& item,
    float& cur_height, const Vector2f& window_position, bool& handled,
    TreeViewItemAction action)
{
    float next_height = cur_height + item.d_size.d_height;

    if (window_position.d_y >= cur_height &&
        window_position.d_y <= next_height)
    {
        handled = true;

        float subtree_expander_width = getViewRenderer()->getSubtreeExpanderSize().d_width;
        if (window_position.d_x >= 0 && window_position.d_x <= subtree_expander_width)
        {
            (this->*action)(item, true);
            return ModelIndex();
        }

        (this->*action)(item, false);
        return ModelIndex(d_itemModel->makeIndex(item.d_childId, item.d_parentIndex));
    }

    cur_height = next_height;

    for (size_t i = 0; i < item.d_renderedChildren.size(); ++i)
    {
        ModelIndex index = indexAtRecursive(item.d_renderedChildren.at(i),
            cur_height, window_position, handled, action);
        if (handled)
            return index;
    }

    return ModelIndex();
}

//----------------------------------------------------------------------------//
TreeViewWindowRenderer* TreeView::getViewRenderer()
{
    return static_cast<TreeViewWindowRenderer*>(ItemView::getViewRenderer());
}

//----------------------------------------------------------------------------//
void TreeView::toggleSubtree(TreeViewItemRenderingState& item)
{
    if (d_itemModel == 0)
        return;

    item.d_subtreeIsExpanded = !item.d_subtreeIsExpanded;

    if (item.d_subtreeIsExpanded)
    {
        computeRenderedChildrenForItem(item,
            d_itemModel->makeIndex(item.d_childId, item.d_parentIndex),
            d_renderedMaxWidth, d_renderedTotalHeight);
    }
    else
    {
        clearItemRenderedChildren(item, d_renderedTotalHeight);
    }

    updateScrollbars();
    invalidate(false);
}

//----------------------------------------------------------------------------//
void TreeView::clearItemRenderedChildren(TreeViewItemRenderingState& item,
    float& renderedTotalHeight)
{
    std::vector<TreeViewItemRenderingState>::iterator itor =
        item.d_renderedChildren.begin();

    while (itor != item.d_renderedChildren.end())
    {
        clearItemRenderedChildren(*itor, renderedTotalHeight);

        d_renderedTotalHeight -= item.d_size.d_height;
        itor = item.d_renderedChildren.erase(itor);
    }
}

//----------------------------------------------------------------------------//
void TreeView::handleSelectionAction(TreeViewItemRenderingState& item, bool toggles_expander)
{
    if (!toggles_expander)
        return;

    toggleSubtree(item);
}

//----------------------------------------------------------------------------//
bool TreeView::onChildrenRemoved(const EventArgs& args)
{
    ItemView::onChildrenRemoved(args);

    return true;
}

}
