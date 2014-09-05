/***********************************************************************
	created:	21/2/2004
	author:		Paul D Turner

    purpose:    Implements the PointerIndicator class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/PointerIndicator.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/System.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/Image.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/CoordConverter.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
bool PointerIndicator::s_initialPositionSet = false;
glm::vec2 PointerIndicator::s_initialPosition(0.0f, 0.0f);

/*************************************************************************
	Event name constants
*************************************************************************/
const String PointerIndicator::EventNamespace("PointerIndicator");
const String PointerIndicator::EventImageChanged("ImageChanged");
const String PointerIndicator::EventDefaultImageChanged("DefaultImageChanged");


/*************************************************************************
	constructor
*************************************************************************/
PointerIndicator::PointerIndicator(void) :
    d_indicatorImage(0),
    d_defaultIndicatorImage(0),
    d_position(0.0f, 0.0f),
    d_visible(true),
    d_customSize(0.0f, 0.0f),
    d_customOffset(0.0f, 0.0f),
    d_cachedGeometryValid(false)
{
    const Rectf screenArea(glm::vec2(0, 0),
                           System::getSingleton().getRenderer()->getDisplaySize());
	// default constraint is to whole screen
	setConstraintArea(&screenArea);

    if (s_initialPositionSet)
        setPosition(s_initialPosition);
    else
        // pointer defaults to middle of the constrained area
        setPosition(0.5f * glm::vec2(screenArea.getWidth(),
                                     screenArea.getHeight()));
}


/*************************************************************************
	Destructor
*************************************************************************/
PointerIndicator::~PointerIndicator(void)
{
    destroyGeometryBuffers();
}


/*************************************************************************
    Set the current pointer indicator image
*************************************************************************/
void PointerIndicator::setImage(const Image* image)
{
    if (image == d_indicatorImage)
        return;

	d_indicatorImage = image;
    d_cachedGeometryValid = false;

	PointerIndicatorEventArgs args(this);
	args.d_image = image;
	onImageChanged(args);
}


/*************************************************************************
	Set the current pointer indicator image
*************************************************************************/
void PointerIndicator::setImage(const String& name)
{
	setImage(&ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void PointerIndicator::setDefaultImage(const Image* image)
{
    if (image == d_defaultIndicatorImage)
        return;

	d_defaultIndicatorImage = image;
    d_cachedGeometryValid = d_indicatorImage != 0;

	PointerIndicatorEventArgs args(this);
	args.d_image = image;
	onDefaultImageChanged(args);
}

//----------------------------------------------------------------------------//
void PointerIndicator::setDefaultImage(const String& name)
{
	setDefaultImage(&ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
const Image* PointerIndicator::getDefaultImage() const
{
    return d_defaultIndicatorImage;
}

/*************************************************************************
	Draw the pointer indicator
*************************************************************************/
void PointerIndicator::draw()
{
    if (!d_visible || !d_indicatorImage)
        return;

    if (!d_cachedGeometryValid)
        cacheGeometry();

    const size_t geom_buffer_count = d_geometryBuffers.size();
    for (size_t i = 0; i < geom_buffer_count; ++i)
        d_geometryBuffers[i]->draw();
}


/*************************************************************************
	Set the current pointer indicator position
*************************************************************************/
void PointerIndicator::setPosition(const glm::vec2& position)
{
    d_position = position;
	constrainPosition();

    updateGeometryBuffersTranslation();
}


/*************************************************************************
	Offset the pointer indicator position by the deltas specified in 'offset'.
*************************************************************************/
void PointerIndicator::offsetPosition(const glm::vec2& offset)
{
    d_position += offset;
	constrainPosition();

    updateGeometryBuffersTranslation();
}


/*************************************************************************
	Checks the pointer indicator position is within the current 'constrain'
	Rect and adjusts as required.
*************************************************************************/
void PointerIndicator::constrainPosition()
{
    const Rectf absarea(getConstraintArea());

    if (d_position.x >= absarea.d_max.d_x)
        d_position.x = absarea.d_max.d_x -1;

    if (d_position.y >= absarea.d_max.d_y)
        d_position.y = absarea.d_max.d_y -1;

    if (d_position.y < absarea.d_min.d_y)
        d_position.y = absarea.d_min.d_y;

    if (d_position.x < absarea.d_min.d_x)
        d_position.x = absarea.d_min.d_x;
}


/*************************************************************************
	Set the area that the pointer indicator is constrained to.
*************************************************************************/
void PointerIndicator::setConstraintArea(const Rectf* area)
{
    const Rectf renderer_area(glm::vec2(0, 0),
                              System::getSingleton().getRenderer()->getDisplaySize());

	if (!area)
	{
		d_constraints.d_min.d_x = cegui_reldim(renderer_area.d_min.d_x / renderer_area.getWidth());
		d_constraints.d_min.d_y = cegui_reldim(renderer_area.d_min.d_y / renderer_area.getHeight());
		d_constraints.d_max.d_x = cegui_reldim(renderer_area.d_max.d_x / renderer_area.getWidth());
		d_constraints.d_max.d_y = cegui_reldim(renderer_area.d_max.d_y / renderer_area.getHeight());
	}
	else
	{
        Rectf finalArea(area->getIntersection(renderer_area));
		d_constraints.d_min.d_x = cegui_reldim(finalArea.d_min.d_x / renderer_area.getWidth());
		d_constraints.d_min.d_y = cegui_reldim(finalArea.d_min.d_y / renderer_area.getHeight());
		d_constraints.d_max.d_x = cegui_reldim(finalArea.d_max.d_x / renderer_area.getWidth());
		d_constraints.d_max.d_y = cegui_reldim(finalArea.d_max.d_y / renderer_area.getHeight());
	}

	constrainPosition();
}


/*************************************************************************
	Set the area that the pointer indicator is constrained to.
*************************************************************************/
void PointerIndicator::setUnifiedConstraintArea(const URect* area)
{
    const Rectf renderer_area(glm::vec2(0, 0),
                              System::getSingleton().getRenderer()->getDisplaySize());

	if (area)
	{
        d_constraints = *area;
	}
	else
	{
		d_constraints.d_min.d_x = cegui_reldim(renderer_area.d_min.d_x / renderer_area.getWidth());
		d_constraints.d_min.d_y = cegui_reldim(renderer_area.d_min.d_y / renderer_area.getHeight());
		d_constraints.d_max.d_x = cegui_reldim(renderer_area.d_max.d_x / renderer_area.getWidth());
		d_constraints.d_max.d_y = cegui_reldim(renderer_area.d_max.d_y / renderer_area.getHeight());
	}

	constrainPosition();
}

/*************************************************************************
	Set the area that the pointer indicator is constrained to.
*************************************************************************/
Rectf PointerIndicator::getConstraintArea(void) const
{
    return Rectf(CoordConverter::asAbsolute(d_constraints, System::getSingleton().getRenderer()->getDisplaySize()));
}

/*************************************************************************
	Set the area that the pointer indicator is constrained to.
*************************************************************************/
const URect& PointerIndicator::getUnifiedConstraintArea(void) const
{
    return d_constraints;
}

/*************************************************************************
	Return the current pointer indicator position in display resolution
	independant values.
*************************************************************************/
glm::vec2 PointerIndicator::getDisplayIndependantPosition(void) const
{
    const Sizef dsz(System::getSingleton().getRenderer()->getDisplaySize());

    return glm::vec2(d_position.x / (dsz.d_width - 1.0f),
                     d_position.y / (dsz.d_height - 1.0f));
}

//----------------------------------------------------------------------------//
void PointerIndicator::notifyDisplaySizeChanged(const Sizef& new_size)
{
    updateGeometryBuffersClipping(Rectf(glm::vec2(0.0f, 0.0f), new_size));

    // invalidate to regenerate geometry at (maybe) new size
    d_cachedGeometryValid = false;
}

//----------------------------------------------------------------------------//
void PointerIndicator::setExplicitRenderSize(const Sizef& size)
{
    d_customSize = size;
    d_cachedGeometryValid = false;
}

//----------------------------------------------------------------------------//
const Sizef& PointerIndicator::getExplicitRenderSize() const
{
    return d_customSize;
}

//----------------------------------------------------------------------------//
void PointerIndicator::cacheGeometry()
{
    d_cachedGeometryValid = true;
    destroyGeometryBuffers();

    // if no image, nothing more to do.
    if (!d_indicatorImage)
        return;

    if (d_customSize.d_width != 0.0f || d_customSize.d_height != 0.0f)
    {
        calculateCustomOffset();
        d_indicatorImage->render(d_geometryBuffers, d_customOffset, d_customSize);
    }
    else
    {
        d_indicatorImage->render(d_geometryBuffers, glm::vec2(0, 0));
    }

    const Rectf clipping_area(glm::vec2(0, 0),
        System::getSingleton().getRenderer()->getDisplaySize());
    updateGeometryBuffersClipping(clipping_area);
    updateGeometryBuffersTranslation();
}

//----------------------------------------------------------------------------//
void PointerIndicator::calculateCustomOffset() const
{
    const Sizef sz(d_indicatorImage->getRenderedSize());
    const glm::vec2 offset(d_indicatorImage->getRenderedOffset());

    d_customOffset.x =
        d_customSize.d_width / sz.d_width * offset.x - offset.x;
    d_customOffset.y =
        d_customSize.d_height / sz.d_height * offset.y - offset.y;
}

//----------------------------------------------------------------------------//
void PointerIndicator::setInitialPointerPosition(const glm::vec2& position)
{
    s_initialPosition = position;
    s_initialPositionSet = true;
}

//----------------------------------------------------------------------------//
void PointerIndicator::invalidate()
{
    d_cachedGeometryValid = false;
}

//----------------------------------------------------------------------------//
void PointerIndicator::onImageChanged(PointerIndicatorEventArgs& e)
{
    fireEvent(EventImageChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void PointerIndicator::onDefaultImageChanged(PointerIndicatorEventArgs& e)
{
    fireEvent(EventDefaultImageChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void PointerIndicator::destroyGeometryBuffers()
{
    const size_t geom_buffer_count = d_geometryBuffers.size();
    for (size_t i = 0; i < geom_buffer_count; ++i)
        System::getSingleton().getRenderer()->destroyGeometryBuffer(*d_geometryBuffers.at(i));

    d_geometryBuffers.clear();
}

//----------------------------------------------------------------------------//
void PointerIndicator::updateGeometryBuffersTranslation()
{
    const size_t geom_buffer_count = d_geometryBuffers.size();
    for (size_t i = 0; i < geom_buffer_count; ++i)
    {
        CEGUI::GeometryBuffer*& currentBuffer = d_geometryBuffers[i];
        currentBuffer->setTranslation(glm::vec3(d_position, 0));
    }
}

//----------------------------------------------------------------------------//
void PointerIndicator::updateGeometryBuffersClipping(const Rectf& clipping_area)
{
    const size_t geom_buffer_count = d_geometryBuffers.size();
    for (size_t i = 0; i < geom_buffer_count; ++i)
    {
        CEGUI::GeometryBuffer*& currentBuffer = d_geometryBuffers[i];
        currentBuffer->setClippingRegion(clipping_area);
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
