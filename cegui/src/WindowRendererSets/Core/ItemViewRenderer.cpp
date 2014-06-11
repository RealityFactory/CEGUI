/***********************************************************************
    filename:   ItemViewRenderer.cpp
    created:    Mon Jun 02 2014
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
#include "CEGUI/WindowRendererSets/Core/ItemViewRenderer.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/Font.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
ItemViewRenderer::ItemViewRenderer(const String& type) :
    WindowRenderer(type)
{
}

//----------------------------------------------------------------------------//
Rectf ItemViewRenderer::getItemRenderingArea(bool hscroll, bool vscroll) const
{
    const WidgetLookFeel& wlf = getLookNFeel();
    String scroll_suffix;

    if (vscroll)
        scroll_suffix += "V";

    if (hscroll)
        scroll_suffix += "H";

    if(!scroll_suffix.empty())
        scroll_suffix += "Scroll";

    const String area_names[] = { "ItemRenderingArea", "ItemRenderArea" };
    const String suffixes[] = { scroll_suffix, "" };

    for (size_t suffix_id = 0; suffix_id < 2; suffix_id++)
    {
        const String& suffix = suffixes[suffix_id];

        for (size_t area_id = 0; area_id < 2; ++area_id)
        {
            const String full_area_name(area_names[area_id] + suffix);

            if (wlf.isNamedAreaDefined(full_area_name))
                return wlf.getNamedArea(full_area_name).getArea().getPixelRect(*d_window);
        }
    }

    CEGUI_THROW(UnknownObjectException("There is no item rendering area defined!"));
}

//----------------------------------------------------------------------------//
void ItemViewRenderer::renderString(Window* window, RenderedString &rendered_string,
    Vector2f draw_pos, const Font* font, const Rectf* item_clipper)
{
    for (size_t i = 0; i < rendered_string.getLineCount(); ++i)
    {
        draw_pos.d_y += CoordConverter::alignToPixels(
            (font->getLineSpacing() - font->getFontHeight()) * 0.5f);

        rendered_string.draw(window, i, window->getGeometryBuffers(),
            draw_pos, 0, item_clipper, 0.0f);

        draw_pos.d_y += rendered_string.getPixelSize(window, i).d_height;
    }
}
}
