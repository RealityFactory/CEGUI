/************************************************************************
	filename: 	WLCheckbox.cpp
	created:	10/8/2004
	author:		Paul D Turner
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
#include "WLCheckbox.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLCheckbox::WidgetTypeName[]		= "WindowsLook/Checkbox";
const utf8	WLCheckbox::ImagesetName[]			= "WindowsLook";
const utf8	WLCheckbox::NormalImageName[]		= "CheckboxNormal";
const utf8	WLCheckbox::HighlightImageName[]	= "CheckboxHover";
const utf8	WLCheckbox::CheckMarkImageName[]	= "CheckboxMark";

const float	WLCheckbox::LabelPadding			= 4.0f;

// colours
const colour WLCheckbox::EnabledTextLabelColour		= 0x000000;
const colour WLCheckbox::DisabledTextLabelColour	= 0x888888;


/*************************************************************************
	Constructor for WindowsLook Checkbox objects.
*************************************************************************/
WLCheckbox::WLCheckbox(const String& type, const String& name) :
	Checkbox(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// setup cache of image pointers
	d_normalImage		= &iset->getImage(NormalImageName);
	d_hoverImage		= &iset->getImage(HighlightImageName);
	d_checkMarkImage	= &iset->getImage(CheckMarkImageName);

	// set default colours for text
	d_normalColour	 = EnabledTextLabelColour;
	d_hoverColour	 = EnabledTextLabelColour;
	d_pushedColour	 = EnabledTextLabelColour;
	d_disabledColour = DisabledTextLabelColour;
}


/*************************************************************************
	Destructor for WLCheckbox objects.
*************************************************************************/
WLCheckbox::~WLCheckbox(void)
{
}


/*************************************************************************
	render the Checkbox in the normal state.	
*************************************************************************/
void WLCheckbox::drawNormal(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// calculate colours to use.
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

	//
	// draw the images
	//
	Vector3 pos(absrect.d_left, absrect.d_top + ((absrect.getHeight() - d_normalImage->getHeight()) * 0.5f), z);
	d_normalImage->draw(pos, clipper, colours);

	if (d_selected)
	{
		d_checkMarkImage->draw(pos, clipper, colours);
	}

	//
	// Draw label text
	//
	absrect.d_top	+= (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
	absrect.d_left	+= d_normalImage->getWidth();
	colours.d_top_left = colours.d_top_right = colours.d_bottom_left = colours.d_bottom_right = (d_normalColour | alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(1), clipper, LeftAligned, colours);
}


/*************************************************************************
	render the Checkbox in the hover / highlighted state.
*************************************************************************/
void WLCheckbox::drawHover(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// calculate colours to use.
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

	//
	// draw the images
	//
	Vector3 pos(absrect.d_left, absrect.d_top + ((absrect.getHeight() - d_hoverImage->getHeight()) * 0.5f), z);
	d_hoverImage->draw(pos, clipper, colours);

	if (d_selected)
	{
		d_checkMarkImage->draw(pos, clipper, colours);
	}

	//
	// Draw label text
	//
	absrect.d_top	+= (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
	absrect.d_left	+= d_hoverImage->getWidth();
	colours.d_top_left = colours.d_top_right = colours.d_bottom_left = colours.d_bottom_right = (d_hoverColour | alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(1), clipper, LeftAligned, colours);
}


/*************************************************************************
	render the Checkbox in the pushed state.	
*************************************************************************/
void WLCheckbox::drawPushed(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// calculate colours to use.
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

	//
	// draw the images
	//
	Vector3 pos(absrect.d_left, absrect.d_top + ((absrect.getHeight() - d_normalImage->getHeight()) * 0.5f), z);
	d_normalImage->draw(pos, clipper, colours);

	if (d_selected)
	{
		d_checkMarkImage->draw(pos, clipper, colours);
	}

	//
	// Draw label text
	//
	absrect.d_top	+= (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
	absrect.d_left	+= d_normalImage->getWidth();
	colours.d_top_left = colours.d_top_right = colours.d_bottom_left = colours.d_bottom_right = (d_pushedColour | alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(1), clipper, LeftAligned, colours);
}


/*************************************************************************
	render the Checkbox in the disabled state	
*************************************************************************/
void WLCheckbox::drawDisabled(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// calculate colours to use.
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

	//
	// draw the images
	//
	Vector3 pos(absrect.d_left, absrect.d_top + ((absrect.getHeight() - d_normalImage->getHeight()) * 0.5f), z);
	d_normalImage->draw(pos, clipper, colours);

	if (d_selected)
	{
		d_checkMarkImage->draw(pos, clipper, colours);
	}

	//
	// Draw label text
	//
	absrect.d_top	+= (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
	absrect.d_left	+= d_normalImage->getWidth();
	colours.d_top_left = colours.d_top_right = colours.d_bottom_left = colours.d_bottom_right = (d_disabledColour | alpha_comp);
	getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(1), clipper, LeftAligned, colours);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLCheckbox
*************************************************************************/
Window* WLCheckboxFactory::createWindow(const String& name)
{
	WLCheckbox* wnd = new WLCheckbox(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
