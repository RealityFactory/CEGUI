/************************************************************************
	filename: 	CEGUIRenderableElement.cpp
	created:	14/4/2004
	author:		Paul D Turner
	
	purpose:	Implements RenderableElement base class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "CEGUIRenderableElement.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Constructor
*************************************************************************/
RenderableElement::RenderableElement(void) :
	d_area(0, 0, 0, 0),
	d_next(NULL)
{
}


/*************************************************************************
	Destructor
*************************************************************************/
RenderableElement::~RenderableElement(void)
{
}


/*************************************************************************
	Draw the element chain starting with this element.
*************************************************************************/
void RenderableElement::draw(const Vector3& position, const Rect& clip_rect)
{
	Vector3	final_pos(position);
	final_pos.d_x += d_area.d_left;
	final_pos.d_y += d_area.d_top;

	// call implementation function to perform actual rendering
	draw_impl(final_pos, clip_rect);

	// render next element in the chain if any.
	if (d_next != NULL)
	{
		d_next->draw(position, clip_rect);
	}

}

} // End of  CEGUI namespace section
