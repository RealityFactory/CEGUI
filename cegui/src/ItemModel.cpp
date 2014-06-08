/***********************************************************************
    filename:   ItemModel.cpp
    created:    Wed May 21 2014
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
#include "CEGUI/ItemModel.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String ItemModel::EventChildrenAdded("ChildrenAdded");
const String ItemModel::EventChildrenRemoved("ChildrenRemoved");
const String ItemModel::EventChildrenDataChanged("ChildrenDataChanged");

//----------------------------------------------------------------------------//
bool ModelIndex::operator==(const ModelIndex& other) const
{
    return d_modelData == other.d_modelData;
}

//----------------------------------------------------------------------------//
bool ModelIndex::operator!=(const ModelIndex& other) const
{
    return !(*this == other);
}

//----------------------------------------------------------------------------//
ItemModel::~ItemModel()
{
}

//----------------------------------------------------------------------------//
void ItemModel::notifyChildrenAdded(ModelIndex start_index, size_t count)
{
    ModelEventArgs args(this, start_index, count);
    fireEvent(EventChildrenAdded, args);
}

//----------------------------------------------------------------------------//
void ItemModel::notifyChildrenRemoved(ModelIndex start_index, size_t count)
{
    ModelEventArgs args(this, start_index, count);
    fireEvent(EventChildrenRemoved, args);
}

}
