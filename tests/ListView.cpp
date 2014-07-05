/***********************************************************************
 *    filename:   ListView.cpp
 *    created:    Sun May 25 2014
 *    author:     Timotei Dolean <timotei21@gmail.com>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
 *
 *    Permission is hereby granted, free of charge, to any person obtaining
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
#include <boost/test/unit_test.hpp>
// Yup. We need this in order to easily inject/call event handlers without having
// to go through GUIContext, or inherit from widgets in order to test them.
#define protected public

#include "CEGUI/CEGUI.h"
#include "ItemModelStub.h"
#include "CEGUI/widgets/Scrollbar.h"

using namespace CEGUI;

//----------------------------------------------------------------------------//
static const String ITEM1 = "ITEM 1";
static const String ITEM2 = "ITEM 2";
static const String ITEM3 = "ITEM 3";
static const String ITEM_WITH_6LINES = "THIS\nIS\nA\nMULTILINE\nLINE\n";

//----------------------------------------------------------------------------//
struct ListViewFixture
{
    ListViewFixture()
    {
        view = static_cast<ListView*>(WindowManager::getSingleton().createWindow("TaharezLook/ListView", "lv"));
        view->setWindowRenderer("Core/ListView");
        view->setModel(&model);
        font_height = view->getFont()->getFontHeight();
    }

    ListView* view;
    ItemModelStub model;
    float font_height;
};

BOOST_FIXTURE_TEST_SUITE(ListViewTestSuite, ListViewFixture)

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_NoItems_ReturnsInvalidIndex)
{
    ModelIndex index = view->indexAt(Vector2f(1, 0));

    BOOST_REQUIRE(index.d_modelData == 0);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSingleObject_ReturnsCorrectIndex)
{
    model.d_items.push_back(ITEM1);

    ModelIndex index = view->indexAt(Vector2f(1, font_height / 2.0f));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(ITEM1, *(static_cast<String*>(index.d_modelData)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSingleObjectListWithOffset_ReturnsCorrectIndex)
{
    float x_offset = 500;
    float y_offset = 354;
    view->setPosition(UVector2(cegui_absdim(x_offset), cegui_absdim(y_offset)));
    model.d_items.push_back(ITEM1);

    ModelIndex index = view->indexAt(Vector2f(
        x_offset + 1,
        y_offset + font_height / 2.0f));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(ITEM1, *(static_cast<String*>(index.d_modelData)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSingleObjectListWithScrollbar_ReturnsCorrectIndex)
{
    for (int i = 0; i < 100; ++i)
        model.d_items.push_back(" item .." + PropertyHelper<int>::toString(i));
    model.d_items.push_back(ITEM1);
    view->setSize(USize(cegui_absdim(100), cegui_absdim(font_height * 10)));
    view->prepareForRender();
    view->getVertScrollbar()->setUnitIntervalScrollPosition(1.0f);

    ModelIndex index = view->indexAt(Vector2f(1, 9 * font_height + font_height / 2.0f));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(ITEM1, *(static_cast<String*>(index.d_modelData)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionOutsideSingleObject_ReturnsInvalidIndex)
{
    model.d_items.push_back(ITEM1);

    ModelIndex index = view->indexAt(Vector2f(1, font_height * 2));

    BOOST_REQUIRE(index.d_modelData == 0);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(IndexAt_PositionInsideSecondObject_ReturnsCorrectIndex)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);

    ModelIndex index = view->indexAt(Vector2f(1, font_height * 2));

    BOOST_REQUIRE(index.d_modelData != 0);
    BOOST_REQUIRE_EQUAL(ITEM2, *(static_cast<String*>(index.d_modelData)));
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetSelectedItem_InitialSelection_SelectsFirstObject)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);

    bool selected = view->setSelectedItem(ModelIndex(&model.d_items.at(0)));
    view->prepareForRender();

    BOOST_REQUIRE(selected);
    BOOST_REQUIRE(view->getItems().at(0)->d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(SetSelectedItem_SecondSelection_SelectsSecondObject)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);
    view->setSelectedItem(ModelIndex(&model.d_items.at(0)));
    view->prepareForRender();

    bool selected = view->setSelectedItem(ModelIndex(&model.d_items.at(1)));
    view->prepareForRender();

    BOOST_REQUIRE(selected);
    BOOST_REQUIRE(!view->getItems().at(0)->d_isSelected);
    BOOST_REQUIRE(view->getItems().at(1)->d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemAdded_ProperSelectionIsPersisted)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);
    view->setSelectedItem(ModelIndex(&model.d_items.at(1)));
    view->prepareForRender();

    model.d_items.insert(model.d_items.begin() + 1, 1, ITEM3);
    model.notifyChildrenAdded(model.getRootIndex(), 1, 1);

    view->prepareForRender();

    BOOST_REQUIRE(!view->getItems().at(0)->d_isSelected);
    BOOST_REQUIRE(!view->getItems().at(1)->d_isSelected);
    BOOST_REQUIRE(view->getItems().at(2)->d_isSelected);
}

//----------------------------------------------------------------------------//
BOOST_AUTO_TEST_CASE(ItemRemoved_NothingIsSelected)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);
    model.d_items.push_back(ITEM3);
    view->setSelectedItem(ModelIndex(&model.d_items.at(1)));
    view->prepareForRender();

    model.d_items.erase(model.d_items.begin() + 1);
    model.notifyChildrenRemoved(model.getRootIndex(), 1, 1);

    view->prepareForRender();

    BOOST_REQUIRE(view->getItems().size() == 2);
    BOOST_REQUIRE(!view->getItems().at(0)->d_isSelected);
    BOOST_REQUIRE(!view->getItems().at(1)->d_isSelected);
}

BOOST_AUTO_TEST_CASE(ItemNameChanged_UpdatesRenderedString)
{
    model.d_items.push_back(ITEM1);
    view->prepareForRender();
    BOOST_CHECK_EQUAL(1, view->getItems().at(0)->d_string.getLineCount());

    model.d_items.at(0) = ITEM_WITH_6LINES;
    model.notifyChildrenDataChanged(model.getRootIndex(), 0, 1);

    view->prepareForRender();
    BOOST_REQUIRE_EQUAL(6, view->getItems().at(0)->d_string.getLineCount());
}

void triggerSelectRangeEvent(Vector2f position, ItemView* view)
{
    SemanticEventArgs args(view);
    args.d_semanticValue = SV_SelectRange;
    view->getGUIContext().getPointerIndicator().setPosition(position);
    view->onSemanticInputEvent(args);
}

BOOST_AUTO_TEST_CASE(SelectRange)
{
    model.d_items.push_back(ITEM1);
    model.d_items.push_back(ITEM2);
    model.d_items.push_back(ITEM3);
    view->prepareForRender();
    view->setMultiSelectEnabled(true);

    view->setSelectedItem(ModelIndex(&model.d_items.at(0)));

    triggerSelectRangeEvent(Vector2f(1, font_height * 2.0f + font_height / 2.0f), view);

    BOOST_REQUIRE_EQUAL(3, view->getIndexSelectionStates().size());
}

BOOST_AUTO_TEST_CASE(SortEnabled_IsEnabled_ListIsSorted)
{
    model.d_items.push_back(ITEM3);
    model.d_items.push_back(ITEM2);
    model.d_items.push_back(ITEM1);
    view->prepareForRender();

    view->setSortEnabled(true);
    view->prepareForRender();

    ModelIndex index = view->indexAt(Vector2f(1, font_height * 2.0f + font_height / 2.0f));
    BOOST_REQUIRE_EQUAL(ITEM3, *(static_cast<String*>(index.d_modelData)));
}

BOOST_AUTO_TEST_SUITE_END()
