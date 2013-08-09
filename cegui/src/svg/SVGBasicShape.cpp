/***********************************************************************
    filename:   SVGBasicShape.cpp
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
#include "CEGUI/svg/SVGBasicShape.h"

#include "CEGUI/svg/SVGTesselator.h"


// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
SVGBasicShape::SVGBasicShape()
{
}

//----------------------------------------------------------------------------//
SVGBasicShape::~SVGBasicShape()
{
}

//----------------------------------------------------------------------------//
SVGRect::SVGRect(const float x,
                 const float y,
                 const float width,
                 const float height,
                 const float rx,
                 const float ry) :
    d_x(x),
    d_y(y),
    d_width(width),
    d_height(height),
    d_rx(rx),
    d_ry(ry)
{
}

//----------------------------------------------------------------------------//
void SVGRect::render(std::vector<GeometryBuffer*>& geometry_buffers,
                     const SVGImage::SVGImageRenderSettings& render_settings) const
{
    SVGTesselator::tesselateAndRenderRect(this,
                                          geometry_buffers,
                                          render_settings);
}

//----------------------------------------------------------------------------//
void SVGPolyline::render(std::vector<GeometryBuffer*>& geometry_buffers,
                         const SVGImage::SVGImageRenderSettings& render_settings) const
{
    SVGTesselator::tesselateAndRenderPolyline(this,
                                              geometry_buffers,
                                              render_settings);
}

//----------------------------------------------------------------------------//
}

