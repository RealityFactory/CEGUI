/***********************************************************************
    created:    Sun May 25 2014
    author:     Timotei Dolean
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
#include "Sample_ModelView.h"
#include "CEGUI/CEGUI.h"

#include <iostream>
#include <sstream>

using namespace CEGUI;

/** This sample uses most of the code from the 'HelloWorld' sample.
    Thus, most of the clarifying comments have been removed for brevity. **/

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool ModelViewDemo::initialise(GUIContext* gui_context)
{
    d_usedFiles = String(__FILE__);

    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");

    gui_context->getPointerIndicator().setDefaultImage("TaharezLook/MouseArrow");

    WindowManager& win_mgr = WindowManager::getSingleton();
    d_root = win_mgr.loadLayoutFromFile("ModelViewDemo.layout");

    ImageManager::getSingleton().loadImageset("DriveIcons.imageset");

    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    gui_context->setDefaultFont(&defaultFont);
    gui_context->setRootWindow(d_root);
    gui_context->setDefaultTooltipType("TaharezLook/Tooltip");

    d_inventoryModel.load();
    d_newItemsCount = 0;

    d_listView = static_cast<ListView*>(win_mgr.createWindow("TaharezLook/ListView", "listView"));
    d_listView->setModel(&d_inventoryModel);
    d_listView->setItemTooltipsEnabled(true);
    d_listView->setSelectionColourRect(ColourRect(Colour(1.0f, 0, 0, 1)));
    d_root->getChild("ListViewHolder")->addChild(d_listView);

    d_treeView = static_cast<TreeView*>(win_mgr.createWindow("TaharezLook/TreeView", "treeView"));
    d_treeView->setModel(&d_inventoryModel);
    d_treeView->setItemTooltipsEnabled(true);
    d_root->getChild("TreeViewHolder")->addChild(d_treeView);

    ListWidget* list_widget = static_cast<ListWidget*>(win_mgr.createWindow("TaharezLook/ListWidget", "listWidget"));
    d_root->getChild("ListWidgetHolder")->addChild(list_widget);
    initListWidgetItems(list_widget);

    d_root->getChild("btnAddRandomItem")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleAddRandomItem, this));

    d_root->getChild("btnAddRandomItemInList")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleAddItemInList, this));

    d_root->getChild("btnAddRandomItemInTree")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleAddItemInTree, this));

    d_root->getChild("btnRemoveSelectedListItem")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleRemoveSelectedListItems, this));

    d_root->getChild("btnRemoveSelectedTreeItem")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleRemoveSelectedTreeItems, this));

    d_root->getChild("btnClearAllItems")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleClearItems, this));

    d_root->getChild("btnUpdateListItemName")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleUpdateListItemName, this));

    d_root->getChild("btnUpdateTreeItemName")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleUpdateTreeItemName, this));

    d_root->getChild("chkMultiSelectEnabled")->subscribeEvent(
        ToggleButton::EventSelectStateChanged,
        Event::Subscriber(&ModelViewDemo::toggleMultiSelect, this));

    d_root->getChild("btnSwitchSortingMode")->subscribeEvent(
        PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::toggleSorting, this));

    d_txtNewItemName = d_root->getChild("txtNewItemName");

    return true;

}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void ModelViewDemo::deinitialise()
{
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleClearItems(const EventArgs& e)
{
    d_inventoryModel.clear();
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleRemoveSelectedListItems(const EventArgs& e)
{
    removeSelectedItemsFromView(d_listView);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleRemoveSelectedTreeItems(const EventArgs& e)
{
    removeSelectedItemsFromView(d_treeView);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleAddRandomItem(const EventArgs& e)
{
    d_inventoryModel.addRandomItemWithChildren(d_inventoryModel.getRootIndex(), 0);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleAddItemInList(const EventArgs& e)
{
    const std::vector<ModelIndexSelectionState>& selections = d_listView->getIndexSelectionStates();
    if (selections.empty())
        return false;

    const ModelIndexSelectionState& selection = (*selections.begin());

    d_inventoryModel.addRandomItemWithChildren(selection.d_parentIndex, selection.d_childId + 1);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleAddItemInTree(const EventArgs& e)
{
    const std::vector<ModelIndexSelectionState>& selections = d_treeView->getIndexSelectionStates();
    if (selections.empty())
        return false;

    const ModelIndexSelectionState& selection = (*selections.begin());

    d_inventoryModel.addRandomItemWithChildren(selection.d_selectedIndex, 0);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleUpdateListItemName(const EventArgs& e)
{
    updateSelectedIndexText(d_listView, d_txtNewItemName->getText());
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleUpdateTreeItemName(const EventArgs& e)
{
    updateSelectedIndexText(d_treeView, d_txtNewItemName->getText());
    return true;
}

//----------------------------------------------------------------------------//
void ModelViewDemo::removeSelectedItemsFromView(ItemView* view)
{
    while(!view->getIndexSelectionStates().empty())
    {
        d_inventoryModel.removeItem(view->getIndexSelectionStates().back().d_selectedIndex);
    }
}

//----------------------------------------------------------------------------//
void ModelViewDemo::updateSelectedIndexText(ItemView* view, const String& text)
{
    const std::vector<ModelIndexSelectionState>& selections = view->getIndexSelectionStates();
    if (selections.empty())
        return;

    const ModelIndexSelectionState& selection = (*selections.begin());

    d_inventoryModel.updateItemName(selection.d_selectedIndex, text);
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::toggleMultiSelect(const EventArgs& e)
{
    bool enabled = d_listView->isMultiSelectEnabled();

    d_listView->setMultiSelectEnabled(!enabled);
    d_treeView->setMultiSelectEnabled(!enabled);

    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::toggleSorting(const EventArgs& e)
{
    Window* switch_button = d_root->getChild("btnSwitchSortingMode");
    ViewSortMode sort_mode = d_listView->getSortMode();

    ViewSortMode next_sort_mode = static_cast<ViewSortMode>((sort_mode + 1) % 3);

    d_listView->setSortMode(next_sort_mode);
    d_treeView->setSortMode(next_sort_mode);

    switch_button->setText(
        "Current sort mode: " + PropertyHelper<ViewSortMode>::toString(next_sort_mode));

    return true;
}

//----------------------------------------------------------------------------//
void ModelViewDemo::initListWidgetItems(ListWidget* list_widget)
{
    ImageManager::ImageIterator itor = ImageManager::getSingleton().getIterator();
    while (!itor.isAtEnd())
    {
        list_widget->addItem(itor.getCurrentKey());
        ++itor;
    }
}

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static ModelViewDemo sample;
    return sample;
}
