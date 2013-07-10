/***********************************************************************
	filename: 	InputAggregator.cpp
	created:	10/7/2013
	author:		Timotei Dolean <timotei21@gmail.com>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/InputAggregator.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    bool InputAggregator::injectTimePulse(float timeElapsed)
    {
        return true;
    }

    bool InputAggregator::injectMouseMove(float delta_x, float delta_y)
    {
        return true;
    }
    bool InputAggregator::injectMouseLeaves()
    {
        return true;
    }

    bool InputAggregator::injectMouseButtonDown(MouseButton button)
    {
        return true;
    }
    bool InputAggregator::injectMouseButtonUp(MouseButton button)
    {
        return true;
    }

    bool InputAggregator::injectKeyDown(Key::Scan scan_code)
    {
        return true;
    }
    bool InputAggregator::injectKeyUp(Key::Scan scan_code)
    {
        return true;
    }

    bool InputAggregator::injectChar(String::value_type code_point)
    {
        return true;
    }
    bool InputAggregator::injectMouseWheelChange(float delta)
    {
        return true;
    }
    bool InputAggregator::injectMousePosition(float x_pos, float y_pos)
    {
        return true;
    }
    
    bool InputAggregator::injectMouseButtonClick(const MouseButton button)
    {
        return true;
    }
    bool InputAggregator::injectMouseButtonDoubleClick(const MouseButton button)
    {
        return true;
    }
    bool InputAggregator::injectMouseButtonTripleClick(const MouseButton button)
    {
        return true;
    }
    
    bool InputAggregator::injectCopyRequest()
    {
        return true;
    }

    bool InputAggregator::injectCutRequest()
    {
        return true;
    }

    bool InputAggregator::injectPasteRequest()
    {
        return true;
    }
} // End of  CEGUI namespace section
