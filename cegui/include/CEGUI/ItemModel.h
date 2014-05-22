/***********************************************************************
    filename:   ItemModel.h
    created:    Tue May 20 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Provides ModelIndex and ItemModel which are the basis of the
                Model-View architecture.
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
#ifndef _CEGUIItemModel_h_
#define _CEGUIItemModel_h_

#include "CEGUI/Base.h"
#include "CEGUI/String.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Class that holds an index to a model item, specified by row and column.
*/
class CEGUIEXPORT ModelIndex
{
public:
    /*!
    \brief
        Constructs an index with the specified model data

    \param model_data
        Optional model data associated with this index.
    */
    ModelIndex(void* model_data = 0) :
        d_modelData(model_data)
    {
    }

    /*!
    \brief
        Pointer to model-specific data.

    \remark
        DO NOT USE/INTERPRET in view. This is just a simple way for the model
        to be able to manage its data and logic.
    */
    void* d_modelData;
};

/*!
\brief
    Abstract class defining the interface between the view and the model.
    This is used by views to query data that is to be shown.
*/
class CEGUIEXPORT ItemModel
{
public:
    virtual ~ItemModel();

    //! Name of the event triggered after new rows were added
    static const String EventRowsAdded;
    //! Name of the event triggered after existing rows were removed
    static const String EventRowsRemoved;
    //! Name of the event triggered after existing rows' data was changed
    static const String EventRowsDataChanged;
    //! Name of the event triggered after new columns were added
    static const String EventColumnsAdded;
    //! Name of the event triggered after existing columns were removed
    static const String EventColumnsRemoved;
    //! Name of the event triggered after existing columns' data was changed
    static const String EventColumnsDataChanged;

    /*!
    \brief
        Returns true if the specified ModelIndex is valid, false otherwise.

    \param model_index
        The ModelIndex for which to get the validity.
    */
    virtual bool isValidIndex(const ModelIndex& model_index) const = 0;

    /*!
    \brief
        Creates a new ModelIndex from the specified row, column and parent index.

    \param row
        The row of the new index.

    \param column
        The column of the new index.

    \param model_index
        The parent ModelIndex of the new index.

    \return
        The newly created ModelIndex.
    */
    virtual ModelIndex makeIndex(int row, int column, const ModelIndex& model_index) = 0;

    /*!
    \brief
        Returns the ModelIndex which is parent for the specified ModelIndex.

    \param model_index
        The ModelIndex for which to compute the parent index.

    \return
        The parent ModelIndex.
    */
    virtual ModelIndex getParentIndex(const ModelIndex& model_index) = 0;

    /*!
    \brief
        Returns the numbers of rows of the specified ModelIndex.

    \param model_index
        The ModelIndex for which to compute the number of rows.
    */
    virtual int getRowCount(const ModelIndex& model_index) = 0;

    /*!
    \brief
        Returns the number of columns of the specified ModelIndex.

    \param model_index
        The ModelIndex for which to compute the number of columns.
    */
    virtual int getColumnCount(const ModelIndex& model_index) = 0;

    /*!
    \brief
        Returns the string representation of the specified ModelIndex

    \param model_index
        The ModelIndex for which to compute the string representation
    */
    virtual String getString(const ModelIndex& model_index) = 0;

    /*!
    \brief
        Returns the Image representation of the specified ModelIndex

    \param model_index
        The ModelIndex for which to compute the image representation

    \return
        A pointer to the Image representation
    */
    virtual const Image* getImage(const ModelIndex& model_index) = 0;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIItemModel_h
