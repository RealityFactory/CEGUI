/***********************************************************************
created:    17/6/2006
author:     Paul D Turner
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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

// NOTE: if you don't have Asian fonts installed, the Japanese and Korean
// sample texts might show up unreadable in this source file. The sample
// should look okay though when running.

#include "Fonts.h"
#include "CEGUI/CEGUI.h"

using namespace CEGUI;

static const unsigned int LangListSize = 12;

struct FontOption
{
    CEGUI::String   d_font;
    bool            d_isEditable;
};

static struct
{
    encoded_char* Language;
    encoded_char* Font;
    encoded_char* Text;
} LangList [LangListSize] =
{
    // A list of strings in different languages
    // Feel free to add your own language here (UTF-8 ONLY!)...
    {
        (encoded_char*)"Sandbox",
            (encoded_char*)"DejaVuSans-12",
            (encoded_char*)"Try out writing text in any language here. The used font can be changed on the right.\n"
            "You can create new fonts if needed. The new font will be set as font for the selected text/language...\n"
            "You can also edit the fonts that are part of this demo or the ones you created here (however, this demo won't allow to change the fonts of the other Samples)\n"
            "Important: When you switch to a font the FIRST time, it takes some time to load it. Especially for fonts with asian characters this load time might be noticable.!"
    },
    {
        (encoded_char*)"European characters using DejaVuSans font",
            (encoded_char*)"DejaVuSans-12",
            (encoded_char*)"Try Catching The Brown Fox While It's Jumping Over The Lazy Dog\n\n"
            "Supports nearly all European unicode characters, including cyrillic:\n"
            "bokmål, česky, русский, српски / srpski, slovenščina, latviešu, Tiếng Việt, etc.\n"
            "¥¤£¢©®ÐÆ\nÄÜÖäüöß\nēĒŗŖūŪīĪāĀšŠģĢķĶļĻžŽčČņŅ\nøØæÆåÅèâïÀÁÂƒéíóúÉÍÓÚõç¿ñÑ\nускоряющпризв"
        },
        {
            (encoded_char*)"European characters using Junicode font",
                (encoded_char*)"Junicode-14",
                (encoded_char*)"Try Catching The Brown Fox While It's Jumping Over The Lazy Dog\n\n"
                "Supports most European unicode characters, but for example no cyrillic:\n"
                "bokmål, česky, slovenščina, latviešu, Tiếng Việt, etc.\n"
                "¥¤£¢©®ÐÆ\nÄÜÖäüöß\nēĒŗŖūŪīĪāĀšŠģĢķĶļĻžŽčČņŅ\nøØæÆåÅèâïÀÁÂƒéíóúÉÍÓÚõç¿ñÑ"
        },

            {
                (encoded_char*)"Korean/Chinese/Japanese using Batang font",
                    (encoded_char*)"Batang-18",
                    (encoded_char*)
                    //Japanese
                    "日本語を選択\n"
                    "トリガー検知\n"
                    "鉱石備蓄不足\n\n\n\n\n"

                    //Chinese
                    "早發白帝城 (李白)\n\n"

                    "朝辭白帝彩雲間，\n"
                    "千里江陵一日還。\n"
                    "兩岸猿聲啼不住，\n"
                    "輕舟己過萬重山。\n\n\n\n\n"

                    //Korean
                    "이몸이/죽고죽어/일백번/고쳐죽어/\n"
                    "백골이/진퇴되어/넋이라도/있고없고/\n"
                    "임 향한/일편단심이야/가실 줄이/있으랴/"
                },
                {
                    (encoded_char*)"Hebrew using TnuaLibre font",
                        (encoded_char*)"TnuaLibre-12",
                        (encoded_char*)"תנועה \n"
                        "חופשי ** אבגד  \n"
                        "1234"
                    },
                    {
                        (encoded_char*)"Old German using Fette UNZ Fraktur font",
                            (encoded_char*)"FetteUNZFraktur-20",
                            (encoded_char*)"Heute back ich, morgen brau ich,\n"
                            "Übermorgen hol ich mir der Königin ihr Kind;\n"
                            "Ach, wie gut, dass niemand weiß,\n"
                            "dass ich Rumpelstilzchen heiß"
                    },
                    {
                        (encoded_char*)"Latin using Old Fell Type font",
                            (encoded_char*)"FellType-12.5",
                            (encoded_char*)"☞Non nobis, non nobis, Domine\n"
                            "Sed nomini tuo da gloriam.☜\n"
                            "    Ð"
                        },
                        {
                            (encoded_char*)"Handwriting font",
                                (encoded_char*)"GreatVibes-22",
                                (encoded_char*)"Dear Gooby,\n\n"
                                "Handwriting is nice when you don't have to do it yourself.\n\n"
                                "Regards, Uncle Dolan."
                        },
                        {
                            (encoded_char*)"RichStyle Icons font",
                                (encoded_char*)"RichStyle-22",
                                (encoded_char*)"+ - ? B I W Y f n t ℹ ⇦ ⇧ ⇨ ⇩ ⌘ ☎ ☐ ☑ ⚖ ⚙ ⚠ ⛏ ✎ ✑ ✓ ✔ ✕ ✖ ❝ ❞ ➡ ⬀ ⬁ ⬂ ⬃ ⬅ ⬆ ⬇ ⬈ ⬉ ⬊ ⬋                       "
                            },
                            {
                                (encoded_char*)"Old Runic writing using Futhark Adapted font",
                                    (encoded_char*)"FutharkAdapted-18",
                                    (encoded_char*)"Somehow, although he is the smallest office boy around the place, none of the other lads pick on him. Scuffling and fighting almost has ceased since Kerensky came to work. That's only one of the nicknames of Leo Kobreen, and was assigned to him because of a considerable facial resemblance to the perpetually fleeing Russian statesman, and, too, because both wore quite formal standing collars."
                            },
                            {
                                (encoded_char*)"Klingon using pIqaD HaSta font",
                                    (encoded_char*)"Klingon-pIqaD-HaSta-24",
                                    (encoded_char*)"  \n\n\n"
                                    "         \n"
                                    "         \n"
                                    "  "
                                },
                                {
                                    (encoded_char*)"Pixel style font using Mizufalp font",
                                        (encoded_char*)"mizufalp-12",
                                        (encoded_char*)"Mechanic: Somebody set up us the bomb.\n"
                                        "Operator: Main screen turn on.\n"
                                        "CATS: All your base are belong to us.\n"
                                        "CATS: You have no chance to survive make your time."
                                }
};

FontsSample::FontsSample() :
    Sample(88)
{
    Sample::d_name = "FontsSample";
    Sample::d_credits = "Lukas \"Ident\" Meindl";
    Sample::d_description =
        "Showcase of different fonts packaged with CEGUI. A selection of "
        "fonts and predefined texts, with characters from a variety of languages, "
        "is given. It is possible to edit the Samplenstration text and create or "
        "edit fonts the font editor window.";
    Sample::d_summary =
        "The Sample displays the usage of CEGUI::Font's using the fonts "
        "from the datafiles folder. For the special characters, UTF-8 "
        "character encoding is supported by CEGUI and is used in the Sample. "
        "All \".font\" files found in the datafiles' font folder are "
        "loaded into the Sample. Additionally, some of the supported font "
        "file endings (.ttf, .pcf, .otf) are added to the list in the "
        "font editor window using the CEGUI ResourceProvider. CEGUI Fonts "
        "have size and autoscale settings that can be adjusted in the Sample. "
        "In CEGUI fonts can be loaded using the \".font\" files or "
        "can be created in run-time using the CEGUI::FontManager. "
        "They can be edited at any time.";
}

bool FontsSample::initialise(CEGUI::GUIContext* guiContext)
{
    d_usedFiles = CEGUI::String(__FILE__);

    d_guiContext = guiContext;

    // we will use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    d_guiContext->getPointerIndicator().setDefaultImage("Vanilla-Images/MouseArrow");

    // Create a custom font which we use to draw the list items. This custom
    // font won't get effected by the scaler and such.
    FontManager& fontManager(FontManager::getSingleton());
    CEGUI::Font& font(fontManager.createFromFile("DejaVuSans-12.font"));
    // Set it as the default
    d_guiContext->setDefaultFont(&font);

    // load all the fonts (if they are not loaded yet)
    fontManager.createAll("*.font", "fonts");

    // Fill list with all loaded font names
    retrieveLoadedFontNames(false);

    // Now that we know about all existing fonts we will initialise
    // the fonts we wanna use for this sample hardcoded
    initialiseSampleFonts();

    // Fill list with all new and thus editable font names
    retrieveLoadedFontNames(true);

    // Fill list with all available font type file names
    retrieveFontFileNames();

    // set tooltip styles (by default there is none)
    d_guiContext->setDefaultTooltipType("Vanilla/Tooltip");

    // Load the GUI layout and attach it to the context as root window
    d_root = winMgr.loadLayoutFromFile("FontsSample.layout");
    d_guiContext->setRootWindow(d_root);

    //Here we create a font and apply it to the renew font name button
    CEGUI::Font& buttonFont = fontManager.createFreeTypeFont("DejaVuSans-14", 14.f, true, "DejaVuSans.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));
    d_renewFontNameButton = static_cast<CEGUI::PushButton*>(d_root->getChild("FontsSampleWindow/FontCreator/RenewNameButton"));
    d_renewFontNameButton->setFont(&buttonFont);

    d_root->getChild("FontsSampleWindow/FontCreator/FontSizeLabel");

    //Subscribe click event for renewing font name based on font file name and size
    d_renewFontNameButton->subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(&FontsSample::handleRenewFontNameButtonClicked, this));

    // Get the editbox where we display the text
    d_textDisplayMultiLineEditbox = static_cast<CEGUI::MultiLineEditbox*>(d_root->getChild("FontsSampleWindow/MultiLineTextWindow"));
    d_textDisplayMultiLineEditbox->subscribeEvent(CEGUI::MultiLineEditbox::EventTextChanged, Event::Subscriber(&FontsSample::handleTextMultiLineEditboxTextChanged, this));

    //Get the font editor info label and apply an animation to it for blending out
    initialiseFontEditorInfoLabel();

    //Initialise the options we have for setting autoScale for fonts as strings
    initialiseAutoScaleOptionsArray();

    //Initialise the Map that connects Language/Text strings with the actual text we will display in the MultiLineEditbox
    initialiseLangToTextMap();

    // Initialise the font creator window + its subwindows
    initialiseFontCreator();

    // Initialise the widget to select fonts and its items
    initialiseFontSelector();

    // Initialise the widget to select the different language texts the relative items for each
    initialiseTextSelector();

    //Subscribe font selection event
    d_fontSelector->subscribeEvent(ListView::EventSelectionChanged, Event::Subscriber(&FontsSample::handleFontSelectionChanged, this));

    d_textSelector->setIndexSelectionState(static_cast<size_t>(0), true);

    return true;
}

// method to perform any required cleanup operations.
void FontsSample::deinitialise()
{
}

bool FontsSample::handleFontCreationButtonClicked(const EventArgs& e)
{
    FontManager& fontMgr(FontManager::getSingleton());

    CEGUI::String fontName = d_fontNameEditbox->getText();
    bool fontNameExists = fontMgr.isDefined(fontName);
    if (fontNameExists || fontName.size() == 0)
    {
        d_fontEditorInfoLabel->setText("Font name already in use.");
        return true;
    }

    CEGUI::String fontFileName = d_fontFileNameSelector->getSelectedItem()->getText();

    CEGUI::String fontSizeString = d_fontSizeEditbox->getText();
    float fontSize = CEGUI::PropertyHelper<float>::fromString(fontSizeString);
    if (fontSize == 0.0f)
        return true;

    bool antiAlias = d_fontAntiAliasCheckbox->isSelected();


    AutoScaledMode autoScaleMode = static_cast<AutoScaledMode>(getAutoScaleMode());


    String::size_type pos = fontFileName.rfind(".imageset");
    if (pos != -1)
    {
        CEGUI::Font& createdFont = fontMgr.createPixmapFont(fontName, fontFileName, Font::getDefaultResourceGroup(), autoScaleMode,
            CEGUI::Sizef(1280.0f, 720.0f), XREA_THROW);
    }
    else
    {
        CEGUI::Font& createdFont = fontMgr.createFreeTypeFont(fontName, fontSize, antiAlias, fontFileName, Font::getDefaultResourceGroup(), autoScaleMode,
            CEGUI::Sizef(1280.0f, 720.0f), XREA_THROW);
    }

    d_fontSelector->addItem(fontName);
    d_fontSelector->setIndexSelectionState(d_fontSelector->getItemCount(), true);

    return true;
}

bool FontsSample::handleFontEditButtonClicked(const EventArgs& e)
{
    FontManager& fontMgr(FontManager::getSingleton());

    CEGUI::String fontName = d_fontNameEditbox->getText();
    bool fontNameExists = fontMgr.isDefined(fontName);
    if (!fontNameExists)
    {
        d_fontEditorInfoLabel->setText("A font with this name does not exist.");
        return true;
    }

    CEGUI::Font& font(fontMgr.get(fontName));


    if (font.isPropertyPresent("PointSize"))
    {
        CEGUI::String fontSizeString = d_fontSizeEditbox->getText();
        float fontSize = CEGUI::PropertyHelper<float>::fromString(fontSizeString);
        if (fontSize != 0.0f)
            font.setProperty<float>("PointSize", fontSize);
    }

    if (font.isPropertyPresent("Antialiased"))
    {
        bool antiAlias = d_fontAntiAliasCheckbox->isSelected();
        font.setProperty("Antialiased", PropertyHelper<bool>::toString(antiAlias));
    }


    AutoScaledMode autoScaleMode = static_cast<AutoScaledMode>(getAutoScaleMode());
    font.setAutoScaled(autoScaleMode);

    return true;
}

bool FontsSample::handleFontSelectionChanged(const EventArgs& e)
{
    //Change font of the selected language/text sample
    if (d_textSelector->getFirstSelectedItem() && d_fontSelector->getFirstSelectedItem())
    {
        unsigned int index = d_textSelector->getFirstSelectedItem()->getId();

        d_languageToFontMap[LangList[index].Language] = d_fontSelector->getFirstSelectedItem()->getText();
    }

    //Change the font creatore fields according to the font
    if (d_fontSelector->getFirstSelectedItem())
    {
        CEGUI::String fontName = d_fontSelector->getFirstSelectedItem()->getText();

        if (FontManager::getSingleton().isDefined(fontName))
        {
            CEGUI::Font& font(FontManager::getSingleton().get(fontName));

            d_textDisplayMultiLineEditbox->setFont(&font);

            d_fontNameEditbox->setText(font.getName());

            AutoScaledMode autoScaleMode = font.getAutoScaled();
            CEGUI::String autoScaleString = d_autoScaleOptionsArray.at(autoScaleMode);
            d_fontAutoScaleCombobox->getEditbox()->setText(autoScaleString);
            d_fontAutoScaleCombobox->selectListItemWithEditboxText();

            if (font.isPropertyPresent("Antialiased"))
            {
                bool fontAntiAlias = font.getProperty<bool>("Antialiased");
                d_fontAntiAliasCheckbox->enable();
                d_fontAntiAliasCheckbox->setSelected(fontAntiAlias);
            }
            else
            {
                d_fontAntiAliasCheckbox->disable();
            }

            if (font.isPropertyPresent("PointSize"))
            {
                CEGUI::String fontPointSize = font.getProperty("PointSize");
                d_fontSizeEditbox->enable();
                d_fontSizeEditbox->setText(fontPointSize);
            }
            else
            {
                d_fontSizeEditbox->disable();
            }

            d_fontFileNameSelector->setText(font.getFileName());

            checkIfEditButtonShouldBeDisabled(font);


            //Change the font colour for pixmap fonts
            if (font.getTypeName().compare(Font_xmlHandler::FontTypePixmap) == 0)
                d_textDisplayMultiLineEditbox->setProperty<CEGUI::ColourRect>("NormalTextColour", CEGUI::ColourRect(CEGUI::Colour(1.0f, 1.0f, 1.0f, 1.0f)));
            else
                d_textDisplayMultiLineEditbox->setProperty<CEGUI::ColourRect>("NormalTextColour", CEGUI::ColourRect(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f)));

        }
    }

    return true;
}

bool FontsSample::handleTextSelectionChanged(const EventArgs& e)
{
    if (d_textSelector->getFirstSelectedItem())
    {
        unsigned int index = d_textSelector->getFirstSelectedItem()->getId();

        d_textDisplayMultiLineEditbox->setText(d_languageToTextMap[LangList[index].Language]);

        changeFontSelectorFontSelection(d_languageToFontMap[LangList[index].Language]);
    }

    return true;
}

bool FontsSample::handleTextMultiLineEditboxTextChanged(const EventArgs& e)
{
    if (d_textSelector->getFirstSelectedItem())
    {
        unsigned int index = d_textSelector->getFirstSelectedItem()->getId();

        d_languageToTextMap[LangList[index].Language] = d_textDisplayMultiLineEditbox->getText();
    }

    return true;
}

bool FontsSample::handleFontFileNameSelectionChanged(const EventArgs& e)
{
    generateNewFontName();

    return true;
}

bool FontsSample::handleRenewFontNameButtonClicked(const EventArgs& e)
{
    generateNewFontName();

    return true;
}

void FontsSample::initialiseAutoScaleOptionsArray()
{
    //AutoScale options in the enum order
    d_autoScaleOptionsArray.push_back("Disabled");
    d_autoScaleOptionsArray.push_back("Vertical");
    d_autoScaleOptionsArray.push_back("Horizontal");
    d_autoScaleOptionsArray.push_back("Minimum");
    d_autoScaleOptionsArray.push_back("Maximum");
    d_autoScaleOptionsArray.push_back("Both");
}


void FontsSample::retrieveLoadedFontNames(bool areEditable)
{
    FontManager& fontManager(FontManager::getSingleton());
    FontManager::FontIterator fi = fontManager.getIterator();

    while (!fi.isAtEnd())
    {
        CEGUI::Font& font(fontManager.get(fi.getCurrentKey()));

        if (d_fontNameOptions.find(font.getName()) == d_fontNameOptions.end())
        {
            d_fontNameOptions[font.getName()] = areEditable;
        }
        ++fi;
    }
}


void FontsSample::retrieveFontFileNames()
{
    System::getSingleton().getResourceProvider()->getResourceGroupFileNames(d_fontFileNameOptions, "*.ttf", Font::getDefaultResourceGroup());
    System::getSingleton().getResourceProvider()->getResourceGroupFileNames(d_fontFileNameOptions, "*.pcf", Font::getDefaultResourceGroup());
    System::getSingleton().getResourceProvider()->getResourceGroupFileNames(d_fontFileNameOptions, "*.otf", Font::getDefaultResourceGroup());
}

void FontsSample::initialiseFontFileNameCombobox()
{
    //Select a font file name if any are present
    if (d_fontFileNameOptions.size() > 0)
    {
        // Add the font file names to the combo box
        for (unsigned int i = 0; i < d_fontFileNameOptions.size(); i++)
        {
            const CEGUI::String& fileName(d_fontFileNameOptions[i]);

            d_fontFileNameSelector->addItem(new StandardItem(fileName, i));
        }

        d_fontFileNameSelector->getDropList()->setIndexSelectionState(static_cast<size_t>(0), true);
        d_fontFileNameSelector->getEditbox()->setText(d_fontFileNameSelector->getItemFromIndex(0)->getText());
    }
}

void FontsSample::initialiseFontCreator()
{
    d_fontFileNameSelector = static_cast<CEGUI::Combobox*>(d_root->getChild("FontsSampleWindow/FontCreator/FontFileCombobox"));
    d_fontNameEditbox = static_cast<CEGUI::Editbox*>(d_root->getChild("FontsSampleWindow/FontCreator/FontNameEditbox"));
    d_fontSizeEditbox = static_cast<CEGUI::Editbox*>(d_root->getChild("FontsSampleWindow/FontCreator/FontSizeEditbox"));
    d_fontAutoScaleCombobox = static_cast<CEGUI::Combobox*>(d_root->getChild("FontsSampleWindow/FontCreator/AutoScaleCombobox"));
    d_fontAntiAliasCheckbox = static_cast<CEGUI::ToggleButton*>(d_root->getChild("FontsSampleWindow/FontCreator/AntiAliasingCheckbox"));
    d_fontCreationButton = static_cast<CEGUI::PushButton*>(d_root->getChild("FontsSampleWindow/FontCreator/CreationButton"));
    d_fontEditButton = static_cast<CEGUI::PushButton*>(d_root->getChild("FontsSampleWindow/FontCreator/EditButton"));

    d_fontFileNameSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, Event::Subscriber(&FontsSample::handleFontFileNameSelectionChanged, this));

    d_fontCreationButton->subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(&FontsSample::handleFontCreationButtonClicked, this));
    d_fontEditButton->subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(&FontsSample::handleFontEditButtonClicked, this));

    initialiseFontFileNameCombobox();
    initialiseAutoScaleCombobox();

}

void FontsSample::initialiseFontSelector()
{
    d_fontSelector = static_cast<ListWidget*>(d_root->getChild("FontsSampleWindow/FontSelector"));
    d_fontSelector->setSortMode(VSM_Ascending);

    //Select a font file name if any are present
    if (d_fontNameOptions.size() > 0)
    {
        std::map<String, bool>::iterator iter = d_fontNameOptions.begin();
        for (; iter != d_fontNameOptions.end(); ++iter)
        {
            std::pair<const String, bool>& currentPair = *iter;
            d_fontSelector->addItem(currentPair.first);
        }
    }
}

void FontsSample::initialiseTextSelector()
{
    d_textSelector = static_cast<ListWidget*>(d_root->getChild("FontsSampleWindow/TextSelector"));
    d_textSelector->subscribeEvent(ListView::EventSelectionChanged, Event::Subscriber(&FontsSample::handleTextSelectionChanged, this));

    for (unsigned int i = 0; i < LangListSize; ++i)
    {
        d_textSelector->addItem(new StandardItem(LangList[i].Language, i));
        d_languageToFontMap[LangList[i].Language] = LangList[i].Font;
    }
}

void FontsSample::changeFontSelectorFontSelection(const CEGUI::String& font)
{
    d_fontSelector->clearSelections();

    unsigned int itemCount = d_fontSelector->getItemCount();
    for (unsigned int i = 0; i < itemCount; ++i)
    {
        StandardItem* item = d_fontSelector->getItemAtIndex(i);
        String itemFontName = item->getText();

        if (itemFontName.compare(font) == 0)
        {
            d_fontSelector->setIndexSelectionState(i, true);
            return;
        }
    }
}

void FontsSample::initialiseLangToTextMap()
{
    for (unsigned int i = 0; i < LangListSize; ++i)
    {
        d_languageToTextMap[LangList[i].Language] = LangList[i].Text;
    }
}

void FontsSample::generateNewFontName()
{
    CEGUI::String fileName = d_fontFileNameSelector->getText();
    CEGUI::String pointSize = d_fontSizeEditbox->getText();

    CEGUI::String fontName = fileName.substr(0, fileName.find_last_of("."));
    fontName += "-" + pointSize;

    d_fontNameEditbox->setText(fontName);
}

void FontsSample::initialiseAutoScaleCombobox()
{
    for (unsigned int i = 0; i < d_autoScaleOptionsArray.size(); ++i)
    {
        CEGUI::String itemText = d_autoScaleOptionsArray.at(i);
        d_fontAutoScaleCombobox->addItem(new StandardItem(itemText, i));
    }
}

int FontsSample::getAutoScaleMode()
{
    CEGUI::String autoScaleString = d_fontAutoScaleCombobox->getSelectedItem()->getText();

    for (unsigned int i = 0; i < d_autoScaleOptionsArray.size(); ++i)
    {
        if (autoScaleString.compare(d_autoScaleOptionsArray.at(i)) == 0)
            return i;
    }

    return 0;
}

bool FontsSample::findFontOption(CEGUI::String fontName)
{
    std::map<String, bool>::iterator iter = d_fontNameOptions.begin();
    for (; iter != d_fontNameOptions.end(); ++iter)
    {
        std::pair<const CEGUI::String, bool>& currentPair = *iter;

        if (currentPair.first.compare(fontName) == 0)
            return currentPair.second;
    }
    return true;
}

void FontsSample::initialiseFontEditorInfoLabel()
{
    d_fontEditorInfoLabel = d_root->getChild("FontsSampleWindow/FontCreator/InfoLabel");

    //Create fadeout animation
    CEGUI::Animation* anim = CEGUI::AnimationManager::getSingleton().createAnimation("ErrorDisplayAndFadeout");
    anim->setDuration(5.f);
    anim->setReplayMode(CEGUI::Animation::RM_Once);

    // this affector changes YRotation and interpolates keyframes with float interpolator
    CEGUI::Affector* affector = anim->createAffector("Alpha", "float");
    affector->createKeyFrame(0.0f, "1.0");
    affector->createKeyFrame(4.0f, "1.0");
    affector->createKeyFrame(5.0f, "0.0", CEGUI::KeyFrame::P_QuadraticDecelerating);

    //anim->defineAutoSubscription(CEGUI::Window::EventTextChanged, "Stop");
    anim->defineAutoSubscription(CEGUI::Window::EventTextChanged, "Start");


    CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
    // after we instantiate the animation, we have to set its target window
    instance->setTargetWindow(d_fontEditorInfoLabel);

    // at this point, you can start this instance and see the results
    instance->start();
}

void FontsSample::checkIfEditButtonShouldBeDisabled(CEGUI::Font &font)
{
    bool isEditable = findFontOption(font.getName());

    if (!isEditable)
    {
        d_fontEditButton->disable();
        d_fontEditButton->setTooltipText("This Sample won't allow editing of\n"
            "fonts that were created outside the Sample or\n"
            "were loaded from .font files");
    }
    else
    {
        d_fontEditButton->enable();
        d_fontEditButton->setTooltipText("");
    }
}

void FontsSample::initialiseSampleFonts()
{
    FontManager& fontManager(FontManager::getSingleton());
    fontManager.createFreeTypeFont("Junicode-14", 14.f, true, "Junicode.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("Klingon-pIqaD-HaSta-24", 24.f, true, "Klingon-pIqaD-HaSta.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("TnuaLibre-12", 12.f, true, "Tnua-Libre.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("RichStyle-22", 22.5f, true, "RichStyle.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("FetteUNZFraktur-20", 20.0f, true, "FetteClassicUNZFraktur.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("GreatVibes-22", 22.f, true, "GreatVibes-Regular.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("FellType-12.5", 12.5f, true, "IMFePIrm29P.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("FutharkAdapted-18", 18.f, true, "Futhark Adapted.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("mizufalp-12", 12.f, true, "mizufalp.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));
}
