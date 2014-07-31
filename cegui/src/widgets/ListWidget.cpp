/***********************************************************************
    created:    Sat Jul 19 2014
    author:     Timotei Dolean <timotei21@gmail.com>
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
#include "CEGUI/widgets/ListWidget.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
const String ListWidget::EventNamespace("ListWidget");
const String ListWidget::WidgetTypeName("CEGUI/ListWidget");
const String ListWidget::EventListContentsChanged("ListContentsChanged");

//----------------------------------------------------------------------------//
ListWidget::ListWidget(const String& type, const String& name) :
    ListView(type, name)
{
}

//----------------------------------------------------------------------------//
ListWidget::~ListWidget()
{
}
//----------------------------------------------------------------------------//
void ListWidget::initialiseComponents()
{
    ListView::initialiseComponents();
    setModel(&d_itemModel);
}

//----------------------------------------------------------------------------//
void ListWidget::setItemSelectionState(size_t item_index, bool state)
{
    if (item_index > getItemCount())
    {
        CEGUI_THROW(InvalidRequestException(
            "the value passed in the 'item_index' parameter is out of range for this ListWidget."));
    }

    ListView::setItemSelectionState(
        d_itemModel.makeIndex(item_index, d_itemModel.getRootIndex()), state);
}

//----------------------------------------------------------------------------//
void ListWidget::setItemSelectionState(StandardItem* item, bool state)
{
    if (item == 0)
    {
        CEGUI_THROW(InvalidRequestException("the item passed was null."));
    }

    ListView::setSelectedItem(d_itemModel.getIndexForItem(item));
}
//----------------------------------------------------------------------------//
StandardItem* ListWidget::getFirstSelectedItem()
{
    if (d_indexSelectionStates.empty())
        return 0;

    return d_itemModel.getItemForIndex(d_indexSelectionStates.front().d_selectedIndex);
}

//----------------------------------------------------------------------------//
StandardItemModel* ListWidget::getModel()
{
    return static_cast<StandardItemModel*>(&d_itemModel);
}

//----------------------------------------------------------------------------//
void ListWidget::addItem(const String& text)
{
    d_itemModel.addItem(text);
}

//----------------------------------------------------------------------------//
void ListWidget::addItem(StandardItem* item)
{
    d_itemModel.addItem(item);
}

//----------------------------------------------------------------------------//
void ListWidget::insertItem(StandardItem* item, const StandardItem* position)
{
    d_itemModel.insertItem(item, position);
}

//----------------------------------------------------------------------------//
void ListWidget::removeItem(const StandardItem* item)
{
    d_itemModel.removeItem(item);
}

//----------------------------------------------------------------------------//
size_t ListWidget::getItemCount() const
{
    return d_itemModel.getChildCount(d_itemModel.getRootIndex());
}

//----------------------------------------------------------------------------//
StandardItem* ListWidget::getItemAtIndex(size_t index)
{
    return d_itemModel.getItemForIndex(
        d_itemModel.makeIndex(index, d_itemModel.getRootIndex()));
}

//----------------------------------------------------------------------------//
StandardItem* ListWidget::findItemWithText(const String& text, const StandardItem* start_item)
{
    // if start_item is NULL begin search at beginning, else start at item after start_item
    size_t index = (start_item == 0) ? 0 : (d_itemModel.getChildId(start_item) + 1);
    size_t list_size = getItemCount();

    while (index < list_size)
    {
        if (getItemAtIndex(index)->getText() == text)
        {
            return getItemAtIndex(index);
        }

        index++;
    }

    return 0;
}

//----------------------------------------------------------------------------//
void ListWidget::clearList()
{
    d_itemModel.clear(true);

    WindowEventArgs args(this);
    onListContentsChanged(args);
}

//----------------------------------------------------------------------------//
void ListWidget::onListContentsChanged(WindowEventArgs& args)
{
    fireEvent(EventListContentsChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
bool ListWidget::onChildrenAdded(const EventArgs& args)
{
    ListView::onChildrenAdded(args);

    WindowEventArgs evt_args(this);
    onListContentsChanged(evt_args);
    return true;
}

//----------------------------------------------------------------------------//
bool ListWidget::onChildrenRemoved(const EventArgs& args)
{
    ListView::onChildrenRemoved(args);

    WindowEventArgs evt_args(this);
    onListContentsChanged(evt_args);
    return true;
}

//----------------------------------------------------------------------------//
bool ListWidget::isItemInList(const StandardItem* item)
{
    size_t child_count = d_itemModel.getChildCount(d_itemModel.getRootIndex());
    for (size_t i = 0; i < child_count; ++i)
    {
        if (item == d_itemModel.getItemForIndex(
                d_itemModel.makeIndex(i, d_itemModel.getRootIndex())))
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
bool ListWidget::isItemSelected(const StandardItem* item)
{
    for (SelectionStatesVector::const_iterator itor = getIndexSelectionStates().begin();
        itor != getIndexSelectionStates().end(); ++itor)
    {
        if (item == d_itemModel.getItemForIndex(itor->d_selectedIndex))
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------//
bool ListWidget::isIndexSelected(size_t index)
{
    //TODO: make a macro/inline function for lists that makes an index - since Root is always the parent
    return ListView::isIndexSelected(d_itemModel.makeIndex(index, d_itemModel.getRootIndex()));
}

//----------------------------------------------------------------------------//
void ListWidget::ensureItemIsVisible(const StandardItem* item)
{
    ListView::ensureItemIsVisible(d_itemModel.getIndexForItem(item));
}
}
