/***********************************************************************
    filename:   SVGImage.cpp
    created:    30th July 2013
    author:     Lukas Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/svg/SVGImage.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/svg/SVGTesselator.h"
#include "CEGUI/svg/SVGData.h"
#include "CEGUI/svg/SVGBasicShape.h"



// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
SVGImage::SVGImage(const String& name) :
    Image(name),
    d_svgData(0)
{
}

//----------------------------------------------------------------------------//
SVGImage::SVGImage(const String& name, SVGData& svg_data) :
    Image(name,
          Vector2f(0.0f, 0.0f),
          Rectf(Vector2f(0.0f, 0.0f),
                Vector2f(svg_data.getWidth(), svg_data.getHeight())),
          ASM_Disabled,
          Sizef(640, 480)),
    d_svgData(&svg_data)
{
}

//----------------------------------------------------------------------------//
SVGImage::SVGImage(const XMLAttributes& attributes) :
    Image("") //TODO define and fill XML elements
{
}

//----------------------------------------------------------------------------//
void SVGImage::setSVGData(SVGData* svg_Data)
{
    d_svgData = svg_Data;
}

//----------------------------------------------------------------------------//
SVGData* SVGImage::getSVGData()
{
    return d_svgData;
}

//----------------------------------------------------------------------------//
void SVGImage::render(std::vector<GeometryBuffer*>& geometry_buffers,
                      const ImageRenderSettings& render_settings) const
{
    Rectf dest(render_settings.d_destArea);
    // apply rendering offset to the destination Rect
    dest.offset(d_scaledOffset);

    const CEGUI::Rectf*const&  clip_area = render_settings.d_clipArea;
    // Calculate the actual (clipped) area to which we want to render to
    Rectf final_rect(clip_area ? dest.getIntersection(*clip_area) : dest );

    // check if our Image is totally clipped and return if it is
    if ((final_rect.getWidth() == 0) || (final_rect.getHeight() == 0))
        return;

    // Calculate the scale factor for our Image which is the scaling of the Image
    // area to the destination area of our render call
    const Vector2f scale_factor(dest.getWidth() / d_imageArea.getWidth(), dest.getHeight() / d_imageArea.getHeight());

    // URGENT FIXME: Shouldn't this be in the hands of the user?
    final_rect.d_min.d_x = CoordConverter::alignToPixels(final_rect.d_min.d_x);
    final_rect.d_min.d_y = CoordConverter::alignToPixels(final_rect.d_min.d_y);
    final_rect.d_max.d_x = CoordConverter::alignToPixels(final_rect.d_max.d_x);
    final_rect.d_max.d_y = CoordConverter::alignToPixels(final_rect.d_max.d_y);

    SVGImageRenderSettings svg_render_settings(render_settings,
                                               scale_factor);

    const std::vector<SVGBasicShape*>& shapes = d_svgData->getShapes();
    const unsigned int shape_count = shapes.size();
    for (unsigned int i = 0; i < shape_count; ++i)
        shapes[i]->render(geometry_buffers, svg_render_settings);
}

//----------------------------------------------------------------------------//
}

