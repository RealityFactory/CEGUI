/***********************************************************************
    filename:   ListView.h
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Interface for a view that displays a list of model items.
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
#ifndef _CEGUIListView_h_
#define _CEGUIListView_h_

#include "CEGUI/views/ItemView.h"
#include <vector>

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

/*!
\brief
    Rendering state class for the ListView
*/
class CEGUIEXPORT ListViewRenderingState : public ViewRenderingState
{
public:
    std::vector<RenderedString> d_renderedStrings;
    std::vector<Sizef> d_renderedStringSizes;
};

/*!
\brief
    View that displays items in a listed fashion.
*/
class CEGUIEXPORT ListView : public ItemView
{
public:
    //! Window factory name
    static const String WidgetTypeName;
    //! Namespace for global events
    static const String EventNamespace;

    /*!
    \brief
        Creates a new instance of ListView
    */
    ListView(const String& type, const String& name);

    virtual ~ListView();

    /*!
    \brief
        Sets the specified item index's as the currently selected item.

    \param index
        The index of the item to be selected.

    \return
        True if the item has been successfully selected, false otherwise.
    */
    virtual bool setSelectedItem(ModelIndex index);

    /*!
    \brief
        Returns the ModelIndex at the specified position.

    \param position
        The position for which to get the ModelIndex.

    \return
        The ModelIndex for the position or a default-constructed ModelIndex
        if nothing was found at that position.
    */
    virtual ModelIndex indexAt(const Vector2f& position);

    virtual ListViewRenderingState* getRenderingState() { return &d_renderingState; }
    virtual void prepareForRender();

protected:
    virtual void onPointerPressHold(PointerEventArgs& e);

private:
    ListViewRenderingState d_renderingState;

    Sizef computeSizeOfRenderedString(RenderedString &rendered_string);

    static BasicRenderedStringParser d_stringParser;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
