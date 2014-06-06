/***********************************************************************
    filename:   Sample_ModelView.cpp
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

/** This sample uses most of the code from the 'HelloWorld' sample.
    Thus, most of the clarifying comments have been removed for brevity. **/

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool ModelViewDemo::initialise(CEGUI::GUIContext* gui_context)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);

    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    gui_context->getPointerIndicator().setDefaultImage("TaharezLook/MouseArrow");

    WindowManager& win_mgr = WindowManager::getSingleton();
    d_root = win_mgr.loadLayoutFromFile("ModelViewDemo.layout");

    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    gui_context->setDefaultFont(&defaultFont);
    gui_context->setRootWindow(d_root);

    d_inventoryModel.load();
    d_newItemsCount = 0;

    ListView* list_view = static_cast<ListView*>(win_mgr.createWindow("TaharezLook/ListView", "listView"));
    list_view->setPosition(UVector2(cegui_reldim(0.1f), cegui_reldim(0.1f)));
    list_view->setModel(&d_inventoryModel);
    d_root->addChild(list_view);

    TreeView* tree_view = static_cast<TreeView*>(win_mgr.createWindow("TaharezLook/TreeView", "treeView"));
    tree_view->setPosition(UVector2(cegui_reldim(0.3f), cegui_reldim(0.1f)));
    tree_view->setModel(&d_inventoryModel);
    d_root->addChild(tree_view);

    Window* btn_add_item = d_root->getChild("btnAddRandomItem");
    btn_add_item->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleAddRandomItem, this));

    Window* btn_remove_item = d_root->getChild("btnRemoveSelectedListItem");
    btn_remove_item->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleRemoveSelectedListItem, this));

    Window* btn_clear_list = d_root->getChild("btnClearItems");
    btn_clear_list->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewDemo::handleClearItems, this));

    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void ModelViewDemo::deinitialise()
{
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleClearItems(const CEGUI::EventArgs& e)
{
    d_inventoryModel.clear();
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleRemoveSelectedListItem(const CEGUI::EventArgs& e)
{
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewDemo::handleAddRandomItem(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    InventoryItem new_item = InventoryItem::make(
        "New random item #" + PropertyHelper<int>::toString(d_newItemsCount), 0.3f);

    InventoryItem new_subitem = InventoryItem::make(
        "New sub item #" + PropertyHelper<int>::toString(d_newItemsCount), 1.3f);

    new_item.d_items.push_back(new_subitem);

    d_inventoryModel.addItem(new_item);

    d_newItemsCount++;
    return true;
}

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static ModelViewDemo sample;
    return sample;
}
