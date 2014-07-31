/***********************************************************************
    created:    Wed May 23 2014
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
#include "CEGUI/views/StandardItemModel.h"
#include "CEGUI/Exceptions.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
StandardItem::StandardItem() : d_text(""), d_id(0)
{
}

//----------------------------------------------------------------------------//
StandardItem::StandardItem(String text, uint id) : d_text(text), d_id(id)
{
}
//----------------------------------------------------------------------------//
StandardItem::~StandardItem()
{
}

//----------------------------------------------------------------------------//
bool StandardItem::operator==(const StandardItem& other) const
{
    return getText() == other.getText();
}

//----------------------------------------------------------------------------//
bool StandardItem::operator<(const StandardItem& other) const
{
    return getText() < other.getText();
}

//----------------------------------------------------------------------------//
StandardItemModel::~StandardItemModel()
{
    clear(false);
}

//----------------------------------------------------------------------------//
bool StandardItemModel::isValidIndex(const ModelIndex& model_index) const
{
    return model_index.d_modelData != 0 && getChildId(model_index) != -1;
}

//----------------------------------------------------------------------------//
ModelIndex StandardItemModel::makeIndex(size_t child, const ModelIndex& parent_index)
{
    if (child >= d_items.size())
        return ModelIndex();

    return ModelIndex(d_items[child]);
}

//----------------------------------------------------------------------------//
bool StandardItemModel::areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2) const
{
    return compareIndices(index1, index2) == 0;
}

//----------------------------------------------------------------------------//
int StandardItemModel::compareIndices(const ModelIndex& index1, const ModelIndex& index2) const
{
    if (!isValidIndex(index1) || !isValidIndex(index2))
        return false;

    if (*getItemForIndex(index1) < *getItemForIndex(index2))
        return -1;

    return *getItemForIndex(index1) == *getItemForIndex(index2) ? 0 : 1;
}

//----------------------------------------------------------------------------//
ModelIndex StandardItemModel::getParentIndex(const ModelIndex& model_index) const
{
    return getRootIndex();
}

//----------------------------------------------------------------------------//
int StandardItemModel::getChildId(const ModelIndex& model_index) const
{
    return getChildId(getItemForIndex(model_index));
}

//----------------------------------------------------------------------------//
int StandardItemModel::getChildId(const StandardItem* item) const
{
    std::vector<StandardItem*>::const_iterator itor =
        std::find(d_items.begin(), d_items.end(), item);

    if (itor == d_items.end())
        return -1;

    return std::distance(d_items.begin(), itor);
}

//----------------------------------------------------------------------------//
ModelIndex StandardItemModel::getRootIndex() const
{
    return ModelIndex();
}

//----------------------------------------------------------------------------//
size_t StandardItemModel::getChildCount(const ModelIndex& model_index) const
{
    return d_items.size();
}

//----------------------------------------------------------------------------//
String StandardItemModel::getData(const ModelIndex& model_index, ItemDataRole role /*= IDR_Text*/)
{
    if (!isValidIndex(model_index))
        return "";

    return getItemForIndex(model_index)->getText();
}

//----------------------------------------------------------------------------//
void StandardItemModel::addItem(String text)
{
    addItem(new StandardItem(text));
}

//----------------------------------------------------------------------------//
void StandardItemModel::addItem(StandardItem* item)
{
    if (item == 0)
        return;

    addItemAtPosition(item, d_items.empty() ? 0 : d_items.size() - 1);
}

//----------------------------------------------------------------------------//
void StandardItemModel::addItemAtPosition(StandardItem* item, size_t pos)
{
    if (pos > d_items.size())
        CEGUI_THROW(InvalidRequestException("The specified position is out of range."));

    d_items.insert(d_items.begin() + pos, item);
    notifyChildrenAdded(getRootIndex(), pos, 1);
}

//----------------------------------------------------------------------------//
void StandardItemModel::insertItem(StandardItem* item, const StandardItem* position)
{
    int child_id = getChildId(position);

    addItemAtPosition(item, child_id == -1 ? 0 : static_cast<size_t>(child_id));
}

//----------------------------------------------------------------------------//
void StandardItemModel::removeItem(const StandardItem* item)
{
    int child_id = getChildId(item);
    if (child_id != -1)
    {
        std::vector<StandardItem*>::iterator itor = d_items.begin() + child_id;
        StandardItem* item = *itor;
        d_items.erase(itor);
        delete item;
    }
}

//----------------------------------------------------------------------------//
StandardItem* StandardItemModel::getItemForIndex(const ModelIndex& index) const
{
    if (index.d_modelData == 0)
        return 0;

    return static_cast<StandardItem*>(index.d_modelData);
}

//----------------------------------------------------------------------------//
void StandardItemModel::clear(bool notify)
{
    size_t item_count = d_items.size();
    while (!d_items.empty())
    {
        StandardItem* item = d_items.back();
        d_items.pop_back();
        delete item;
    }

    if (notify)
    {
        notifyChildrenRemoved(getRootIndex(), 0, item_count);
    }
}

//----------------------------------------------------------------------------//
ModelIndex StandardItemModel::getIndexForItem(const StandardItem* item) const
{
    int child_id = getChildId(item);

    return ModelIndex(child_id == -1 ? 0 : d_items.at(child_id));
}

}
