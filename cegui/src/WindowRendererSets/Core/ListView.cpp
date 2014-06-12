/***********************************************************************
    filename:   ListView.cpp
    created:    Mon May 26 2014
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
#include "CEGUI/WindowRendererSets/Core/ListView.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"

#include "CEGUI/Colour.h"
#include "CEGUI/Vector.h"
#include "CEGUI/Font.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
const String FalagardListView::TypeName("Core/ListView");

//----------------------------------------------------------------------------//
FalagardListView::FalagardListView(const String& type) :
    ItemViewRenderer(type)
{
}

//----------------------------------------------------------------------------//
void FalagardListView::render()
{
    const StateImagery* imagery;
    const WidgetLookFeel& wlf = getLookNFeel();
    ListView* list_view = static_cast<ListView*>(d_window);

    list_view->prepareForRender();

    imagery = &wlf.getStateImagery(list_view->isEffectiveDisabled() ? "Disabled" : "Enabled");
    imagery->render(*list_view);

    render(list_view);
}

//----------------------------------------------------------------------------//
void FalagardListView::render(ListView* list_view)
{
    Rectf items_area(getItemRenderingArea(false, false));
    Vector3f item_pos(items_area.left(), items_area.top(), 0.0f);

    for (size_t i = 0; i < list_view->getItems().size(); ++i)
    {
        ListViewItemRenderingState item = list_view->getItems().at(i);
        RenderedString& rendered_string = item.d_string;
        Sizef size(item.d_size);

        size.d_width = ceguimax(items_area.getWidth(), size.d_width);

        Rectf item_rect;
        item_rect.left(item_pos.d_x);
        item_rect.top(item_pos.d_y);
        item_rect.setSize(size);

        Rectf item_clipper(item_rect.getIntersection(items_area));

        ColourRect colour_rect(Colour(0.5f, 0.5f, 0.5f));
        if (list_view->getSelectionBrushImage() != 0 && item.d_isSelected)
        {
            list_view->getSelectionBrushImage()->render(
                list_view->getGeometryBuffers(),
                item_rect,
                &item_clipper,
                true,
                colour_rect);
        }

        renderString(list_view, rendered_string, item_rect.getPosition(),
            list_view->getFont(), &item_clipper);

        item_pos.d_y += size.d_height;
    }
}
}
