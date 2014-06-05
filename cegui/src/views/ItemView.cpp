/***********************************************************************
    filename:   ItemView.cpp
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

namespace CEGUI
{

//----------------------------------------------------------------------------//
ViewRenderingState::ViewRenderingState() :
    d_isDirty(true)
{
}

//----------------------------------------------------------------------------//
ViewRenderingState::~ViewRenderingState()
{

}

//----------------------------------------------------------------------------//
ItemView::ItemView(const String& type, const String& name) :
    Window(type, name),
    d_itemModel(0)
{
}

//----------------------------------------------------------------------------//
ItemView::~ItemView()
{
}

//----------------------------------------------------------------------------//
void ItemView::setModel(ItemModel* item_model)
{
    if (item_model == d_itemModel)
        return;

    d_itemModel = item_model;
    getRenderingState()->d_isDirty = true;
}

}
