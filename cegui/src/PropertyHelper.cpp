/***********************************************************************
    created:    17th August 2015
    author:     Lukas Meindl (based on code by Paul D Turner)

    purpose:    Implementation of PropertyHelper methods
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/Image.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Font.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/StreamHelper.h"
#include "CEGUI/SharedStringStream.h"

#include <cstdio>
#include <sstream>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

namespace CEGUI
{

//! Definitions of static constants
const CEGUI::String PropertyHelper<bool>::True("true");
const CEGUI::String PropertyHelper<bool>::False("false");

const CEGUI::String PropertyHelper<AspectMode>::Shrink("Shrink");
const CEGUI::String PropertyHelper<AspectMode>::Expand("Expand");
const CEGUI::String PropertyHelper<AspectMode>::Ignore("Ignore");



//! Helper function for throwing errors
static void logParsingError(const String& typeName, const String& parsedstring)
{
    throw InvalidRequestException("PropertyHelper::fromString could not parse the type " + typeName + " from the string: \"" + parsedstring + "\"");
}

//! Helper function for getting the stringstream
static std::stringstream& getPreparedStream(const String& str)
{
    std::stringstream& sstream = CEGUI::SharedStringstream::s_sharedStreamInstance.d_sharedStream;
    sstream.str(str.c_str());
    sstream.clear();
    return sstream;
}

//! Helper function for getting the stringstream
static std::stringstream& getPreparedStream()
{
    std::stringstream& sstream = CEGUI::SharedStringstream::s_sharedStreamInstance.d_sharedStream;
    sstream.str(std::string());
    sstream.clear();
    return sstream;
}

const String& PropertyHelper<bool>::getDataTypeName()
{
    static const String type("bool");

    return type;
}

PropertyHelper<bool>::return_type PropertyHelper<bool>::fromString(const String& str)
{
    return (str == True || str == "True");
}

PropertyHelper<bool>::string_return_type PropertyHelper<bool>::toString(pass_type val)
{
    return val ? True : False;
}

const String& PropertyHelper<AspectMode>::getDataTypeName()
{
    static const String type("AspectMode");

    return type;
}

PropertyHelper<AspectMode>::string_return_type PropertyHelper<AspectMode>::toString(
    PropertyHelper<AspectMode>::pass_type val)
{
    if (val == AM_IGNORE)
    {
        return Ignore;
    }
    else if (val == AM_SHRINK)
    {
        return Shrink;
    }
    else if (val == AM_EXPAND)
    {
        return Expand;
    }
    else
    {
        assert(false && "Invalid aspect mode");
        return Ignore;
    }
}

PropertyHelper<AspectMode>::return_type PropertyHelper<AspectMode>::fromString(const String& str)
{
    if (str == Shrink)
    {
        return AM_SHRINK;
    }
    else if (str == Expand)
    {
        return AM_EXPAND;
    }
    else
    {
        return AM_IGNORE;
    }
}

const String& PropertyHelper<Image*>::getDataTypeName()
{
    static const String type("Image");

    return type;
}

PropertyHelper<Image*>::return_type
PropertyHelper<Image*>::fromString(const String& str)
{
    // handle empty string case
    if (str.empty())
        return 0;

    PropertyHelper<Image*>::return_type image;

    try
    {
        image = &ImageManager::getSingleton().get(str);
    }
    catch (UnknownObjectException&)
    {
        image = 0;
    }

    return image;
}

PropertyHelper<Image*>::string_return_type PropertyHelper<Image*>::toString(
    PropertyHelper<Image*>::pass_type val)
{
    return val ? val->getName() : String("");
}

const String& PropertyHelper<Font*>::getDataTypeName()
{
    static const String type("Font");

    return type;
}

PropertyHelper<Font*>::return_type
PropertyHelper<Font*>::fromString(const String& str)
{
    // handle empty string case
    if (str.empty())
        return 0;

    PropertyHelper<Font*>::return_type image;

    try
    {
        image = &FontManager::getSingleton().get(str);
    }
    catch (UnknownObjectException&)
    {
        image = 0;
    }

    return image;
}

PropertyHelper<Font*>::string_return_type PropertyHelper<Font*>::toString(
    PropertyHelper<Font*>::pass_type val)
{
    return val ? val->getName() : String("");
}

const String& PropertyHelper<float>::getDataTypeName()
{
    static const String type("float");

    return type;
}

PropertyHelper<float>::return_type
PropertyHelper<float>::fromString(const String& str)
{
    float val = 0.0f;

    if (str.empty())
        return val;

    std::stringstream& sstream = getPreparedStream(str);

    sstream >> val;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}

PropertyHelper<float>::string_return_type PropertyHelper<float>::toString(
    PropertyHelper<float>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
}

const String& PropertyHelper<UDim>::getDataTypeName()
{
    static const String type("UDim");

    return type;
}

PropertyHelper<UDim>::return_type
PropertyHelper<UDim>::fromString(const String& str)
{
    UDim ud(0.0f, 0.0f);

    if (str.empty())
        return ud;

    // Format is: " { %g , %g } " but we are lenient regarding the format, so this is also allowed: " %g %g "
    std::stringstream sstream(str.c_str());
    sstream >> optionalChar<'{'> >> ud.d_scale >> optionalChar<','> >> ud.d_offset;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return ud;
}

PropertyHelper<UDim>::string_return_type PropertyHelper<UDim>::toString(
    PropertyHelper<UDim>::pass_type val)
{
    char buff[128];
    snprintf(buff, sizeof(buff), "{%g,%g}", val.d_scale, val.d_offset);

    return String(buff);
}

const String& PropertyHelper<UVector2>::getDataTypeName()
{
    static const String type("UVector2");

    return type;
}

PropertyHelper<UVector2>::return_type
PropertyHelper<UVector2>::fromString(const String& str)
{
    UVector2 uv(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f));

    if (str.empty())
        return uv;

    // Format is: " { { %g , %g } , { %g , %g } } " but we are lenient regarding the format, so this is also allowed: " { %g %g } { %g %g } "
    std::stringstream sstream(str.c_str());
    sstream >> optionalChar<'{'> >> mandatoryChar<'{'> >> uv.d_x.d_scale >> optionalChar<','> >> uv.d_x.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> mandatoryChar<'{'> >> uv.d_y.d_scale >> optionalChar<','> >> uv.d_y.d_offset;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return uv;
}

PropertyHelper<UVector2>::string_return_type PropertyHelper<UVector2>::toString(
    PropertyHelper<UVector2>::pass_type val)
{
    char buff[256];
    snprintf(buff, sizeof(buff), "{{%g,%g},{%g,%g}}",
        val.d_x.d_scale, val.d_x.d_offset, val.d_y.d_scale, val.d_y.d_offset);

    return String(buff);
}

const String& PropertyHelper<USize>::getDataTypeName()
{
    static const String type("USize");

    return type;
}

PropertyHelper<USize>::return_type
PropertyHelper<USize>::fromString(const String& str)
{
    USize uv(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f));

    if (str.empty())
        return uv;

    // Format is: " { { %g , %g } , { %g , %g } } " but we are lenient regarding the format, so this is also allowed: " { %g %g } { %g %g } "
    std::stringstream sstream(str.c_str());
    sstream >> optionalChar<'{'> >> mandatoryChar<'{'> >> uv.d_width.d_scale >> optionalChar<','> >> uv.d_width.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> mandatoryChar<'{'> >> uv.d_height.d_scale >> optionalChar<','> >> uv.d_height.d_offset;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return uv;
}

PropertyHelper<USize>::string_return_type PropertyHelper<USize>::toString(
    PropertyHelper<USize>::pass_type val)
{
    char buff[256];
    snprintf(buff, sizeof(buff), "{{%g,%g},{%g,%g}}",
        val.d_width.d_scale, val.d_width.d_offset, val.d_height.d_scale, val.d_height.d_offset);

    return String(buff);
}

const String& PropertyHelper<URect>::getDataTypeName()
{
    static const String type("URect");

    return type;
}

PropertyHelper<URect>::return_type
PropertyHelper<URect>::fromString(const String& str)
{
    URect ur(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)), UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));

    if (str.empty())
        return ur;

    // Format is:  { { %g , %g } , { %g , %g } , { %g , %g } , { %g , %g } }" 
    // but we are lenient regarding the format, so this is also allowed: " { %g %g } { %g %g } { %g %g } { %g %g }"
    std::stringstream sstream(str.c_str());
    sstream >> optionalChar<'{'> >> mandatoryChar<'{'> >> ur.d_min.d_x.d_scale >> optionalChar<','> >> ur.d_min.d_x.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> mandatoryChar<'{'> >> ur.d_min.d_y.d_scale >> optionalChar<','> >> ur.d_min.d_y.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> mandatoryChar<'{'> >> ur.d_max.d_x.d_scale >> optionalChar<','> >> ur.d_max.d_x.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> mandatoryChar<'{'> >> ur.d_max.d_y.d_scale >> optionalChar<','> >> ur.d_max.d_y.d_offset;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return ur;
}

PropertyHelper<URect>::string_return_type PropertyHelper<URect>::toString(
    PropertyHelper<URect>::pass_type val)
{
    char buff[512];
    snprintf(buff, sizeof(buff), "{{%g,%g},{%g,%g},{%g,%g},{%g,%g}}",
        val.d_min.d_x.d_scale, val.d_min.d_x.d_offset,
        val.d_min.d_y.d_scale, val.d_min.d_y.d_offset,
        val.d_max.d_x.d_scale, val.d_max.d_x.d_offset,
        val.d_max.d_y.d_scale, val.d_max.d_y.d_offset);

    return String(buff);
}

const String& PropertyHelper<UBox>::getDataTypeName()
{
    static const String type("UBox");

    return type;
}

PropertyHelper<UBox>::return_type
PropertyHelper<UBox>::fromString(const String& str)
{
    UBox ret(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f), UDim(0.0f, 0.0f), UDim(0.0f, 0.0f));

    if (str.empty())
        return ret;

    // Format is:  { top: { %g , %g } , left: { %g , %g } , bottom: { %g , %g } , right: { %g , %g } }",
    // but we are lenient regarding the format, so this is also allowed: " top : { %g %g } left : { %g %g } bottom : { %g %g } right : { %g %g } "
    std::stringstream sstream(str.c_str());
    sstream >> optionalChar<'{'> >> MandatoryString(" top : {") >> ret.d_top.d_scale >> optionalChar<','> >> ret.d_top.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> MandatoryString(" left : {") >> ret.d_left.d_scale >> optionalChar<','> >> ret.d_left.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> MandatoryString(" bottom : {") >> ret.d_bottom.d_scale >> optionalChar<','> >> ret.d_bottom.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> MandatoryString(" right : {") >> ret.d_right.d_scale >> optionalChar<','> >> ret.d_right.d_offset;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return ret;
}

PropertyHelper<UBox>::string_return_type PropertyHelper<UBox>::toString(
    PropertyHelper<UBox>::pass_type val)
{
    char buff[512];
    snprintf(buff, sizeof(buff), "{top:{%g,%g},left:{%g,%g},bottom:{%g,%g},right:{%g,%g}}",
        val.d_top.d_scale, val.d_top.d_offset,
        val.d_left.d_scale, val.d_left.d_offset,
        val.d_bottom.d_scale, val.d_bottom.d_offset,
        val.d_right.d_scale, val.d_right.d_offset);

    return String(buff);
}

const String& PropertyHelper<ColourRect>::getDataTypeName()
{
    static const String type("ColourRect");

    return type;
}

PropertyHelper<ColourRect>::return_type
PropertyHelper<ColourRect>::fromString(const String& str)
{
    if (str.empty())
        return ColourRect(Colour(0xFF000000));
    else if (str.length() == 8)
    {
        argb_t all = 0xFF000000;

        std::stringstream sstream;
        sstream.clear();
        sstream << std::hex << str.c_str();
        sstream >> all;
        if (sstream.fail())
            logParsingError(getDataTypeName(), str);
        sstream << std::dec;

        return ColourRect(all);
    }
    else
    {
        argb_t topLeft = 0xFF000000, topRight = 0xFF000000, bottomLeft = 0xFF000000, bottomRight = 0xFF000000;

        std::stringstream originalStrStream(str.c_str());
        std::stringstream sstream;
        sstream << std::hex;
        std::string hexadecimalString;

        // Match and remove the preceding string and all trailing whitespaces
        originalStrStream >> MandatoryString(" tl : ");
        if (originalStrStream.fail())
            logParsingError(getDataTypeName(), str);

        std::getline(originalStrStream, hexadecimalString, ' ');
        sstream << hexadecimalString;
        sstream >> topLeft;
        if (sstream.fail())
            logParsingError(getDataTypeName(), str);

        originalStrStream >> MandatoryString(" tr : ");
        if (originalStrStream.fail())
            logParsingError(getDataTypeName(), str);

        std::getline(originalStrStream, hexadecimalString, ' ');
        sstream.str(std::string());
        sstream.clear();
        sstream << hexadecimalString;
        sstream >> topRight;
        if (sstream.fail())
            logParsingError(getDataTypeName(), str);

        originalStrStream >> MandatoryString(" bl : ");
        if (originalStrStream.fail())
            logParsingError(getDataTypeName(), str);

        std::getline(originalStrStream, hexadecimalString, ' ');
        sstream.str(std::string());
        sstream.clear();
        sstream << hexadecimalString;
        sstream >> bottomLeft;
        if (sstream.fail())
            logParsingError(getDataTypeName(), str);

        originalStrStream >> MandatoryString(" br : ");
        if (originalStrStream.fail())
            logParsingError(getDataTypeName(), str);

        std::getline(originalStrStream, hexadecimalString, ' ');
        sstream.str(std::string());
        sstream.clear();
        sstream << hexadecimalString;
        sstream >> bottomRight;
        if (sstream.fail())
            logParsingError(getDataTypeName(), str);

        sstream << std::dec;

        return ColourRect(topLeft, topRight, bottomLeft, bottomRight);
    }
}

PropertyHelper<ColourRect>::string_return_type PropertyHelper<ColourRect>::toString(
    PropertyHelper<ColourRect>::pass_type val)
{
    char buff[64];
    sprintf(buff, "tl:%.8X tr:%.8X bl:%.8X br:%.8X", val.d_top_left.getARGB(), val.d_top_right.getARGB(), val.d_bottom_left.getARGB(), val.d_bottom_right.getARGB());

    return String(buff);
}

const String& PropertyHelper<Colour>::getDataTypeName()
{
    static const String type("Colour");

    return type;
}

PropertyHelper<Colour>::return_type
PropertyHelper<Colour>::fromString(const String& str)
{
    argb_t val = 0xFF000000;

    if (str.empty())
        return Colour(val);

    std::stringstream sstream;
    sstream.clear();
    sstream << std::hex << str.c_str();
    sstream >> val;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);
    sstream << std::dec;

    return Colour(val);
}

PropertyHelper<Colour>::string_return_type PropertyHelper<Colour>::toString(
    PropertyHelper<Colour>::pass_type val)
{
    char buff[16];
    sprintf(buff, "%.8X", val.getARGB());

    return String(buff);
}

const String& PropertyHelper<Rectf>::getDataTypeName()
{
    static const String type("Rectf");

    return type;
}

PropertyHelper<Rectf>::return_type
PropertyHelper<Rectf>::fromString(const String& str)
{
    Rectf val(0.0f, 0.0f, 0.0f, 0.0f);

    if (str.empty())
        return val;

    std::stringstream sstream(str.c_str());
    sstream >> MandatoryString(" l :") >> val.d_min.d_x >> MandatoryString(" t :") >> val.d_min.d_y >> MandatoryString(" r :") >> val.d_max.d_x >> MandatoryString(" b :") >> val.d_max.d_y;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}

PropertyHelper<Rectf>::string_return_type PropertyHelper<Rectf>::toString(
    PropertyHelper<Rectf>::pass_type val)
{
    char buff[256];
    snprintf(buff, sizeof(buff), "l:%g t:%g r:%g b:%g",
        val.d_min.d_x, val.d_min.d_y, val.d_max.d_x, val.d_max.d_y);

    return String(buff);
}

const String& PropertyHelper<Sizef>::getDataTypeName()
{
    static const String type("Sizef");

    return type;
}

PropertyHelper<Sizef>::return_type
PropertyHelper<Sizef>::fromString(const String& str)
{
    Sizef val(0.0f, 0.0f);

    if (str.empty())
        return val;

    std::stringstream sstream(str.c_str());
    sstream >> MandatoryString(" w :") >> val.d_width >> MandatoryString(" h :") >> val.d_height;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}

PropertyHelper<Sizef>::string_return_type PropertyHelper<Sizef>::toString(
    PropertyHelper<Sizef>::pass_type val)
{
    char buff[128];
    snprintf(buff, sizeof(buff), "w:%g h:%g", val.d_width, val.d_height);

    return String(buff);
}

const String& PropertyHelper<double>::getDataTypeName()
{
    static const String type("double");

    return type;
}

PropertyHelper<double>::return_type
PropertyHelper<double>::fromString(const String& str)
{
    double val = 0.0;

    if (str.empty())
        return val;

    std::stringstream sstream(str.c_str());
    sstream >> val;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}


PropertyHelper<double>::string_return_type PropertyHelper<double>::toString(
    PropertyHelper<double>::pass_type val)
{
    char buff[64];
    snprintf(buff, sizeof(buff), "%g", val);

    return String(buff);
}


const String& PropertyHelper<int>::getDataTypeName()
{
    static const String type("int");

    return type;
}

PropertyHelper<int>::return_type
PropertyHelper<int>::fromString(const String& str)
{
    int val = 0;

    if (str.empty())
        return val;

    std::stringstream sstream(str.c_str());
    sstream >> val;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}


PropertyHelper<int>::string_return_type PropertyHelper<int>::toString(
    PropertyHelper<int>::pass_type val)
{
    char buff[64];
    snprintf(buff, sizeof(buff), "%d", val);

    return String(buff);
}


const String& PropertyHelper<uint64>::getDataTypeName()
{
    static const String type("uint64");

    return type;
}

PropertyHelper<uint64>::return_type
PropertyHelper<uint64>::fromString(const String& str)
{
    uint64 val = 0;

    if (str.empty())
        return val;

    std::stringstream sstream(str.c_str());
    sstream >> val;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}


PropertyHelper<uint64>::string_return_type PropertyHelper<uint64>::toString(
    PropertyHelper<uint64>::pass_type val)
{
    char buff[64];
    snprintf(buff, sizeof(buff), "%llu", val);

    return String(buff);
}


const String& PropertyHelper<unsigned long>::getDataTypeName()
{
    static const String type("unsigned long");

    return type;
}

PropertyHelper<unsigned long>::return_type
PropertyHelper<unsigned long>::fromString(const String& str)
{
    unsigned long val = 0;

    if (str.empty())
        return val;

    std::stringstream sstream(str.c_str());
    sstream >> val;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}

PropertyHelper<unsigned long>::string_return_type PropertyHelper<unsigned long>::toString(
    PropertyHelper<unsigned long>::pass_type val)
{
    char buff[64];
    snprintf(buff, sizeof(buff), "%lu", val);

    return String(buff);
}

const String& PropertyHelper<glm::vec2>::getDataTypeName()
{
    static const String type("vec2");

    return type;
}

PropertyHelper<glm::vec2>::return_type
PropertyHelper<glm::vec2>::fromString(const String& str)
{
    glm::vec2 val(0, 0);

    if (str.empty())
        return val;

    std::stringstream sstream(str.c_str());
    sstream >> MandatoryString(" x :") >> val.x >> MandatoryString(" y :") >> val.y;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}

PropertyHelper<glm::vec2>::string_return_type PropertyHelper<glm::vec2>::toString(
    PropertyHelper<glm::vec2>::pass_type val)
{
    char buff[128];
    snprintf(buff, sizeof(buff), "x:%g y:%g", val.x, val.y);

    return String(buff);
}

const String& PropertyHelper<glm::vec3>::getDataTypeName()
{
    static const String type("vec3");

    return type;
}

PropertyHelper<glm::vec3>::return_type
PropertyHelper<glm::vec3>::fromString(const String& str)
{
    glm::vec3 val(0, 0, 0);

    if (str.empty())
        return val;

    std::stringstream sstream(str.c_str());
    sstream >> MandatoryString(" x :") >> val.x >> MandatoryString(" y :") >> val.y >> MandatoryString(" z :") >> val.z;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}

PropertyHelper<glm::vec3>::string_return_type PropertyHelper<glm::vec3>::toString(
    PropertyHelper<glm::vec3>::pass_type val)
{
    char buff[128];
    snprintf(buff, sizeof(buff), "x:%g y:%g z:%g", val.x, val.y, val.z);

    return String(buff);
}

const String& PropertyHelper<glm::quat>::getDataTypeName()
{
    static const String type("quat");

    return type;
}

PropertyHelper<glm::quat>::return_type
PropertyHelper<glm::quat>::fromString(const String& str)
{
    glm::quat val(1, 0, 0, 0);

    if (str.empty())
        return val;
    else if (strchr(str.c_str(), 'w') || strchr(str.c_str(), 'W'))
    {
        std::stringstream sstream(str.c_str());
        sstream >> MandatoryString(" x :") >> val.x >> MandatoryString(" y :") >> val.y >> MandatoryString(" z :") >> val.z >> MandatoryString(" w :") >> val.w;
        if (sstream.fail())
            logParsingError(getDataTypeName(), str);
        return val;
    }
    else
    {
        float x, y, z;
        // CEGUI takes degrees because it's easier to work with
        std::stringstream sstream(str.c_str());
        sstream >> MandatoryString(" x :") >> x >> MandatoryString(" y :") >> y >> MandatoryString(" z :") >> z;
        if (sstream.fail())
            logParsingError(getDataTypeName(), str);

        // glm::radians converts from degrees to radians
        return glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z)));
    }
}

PropertyHelper<glm::quat>::string_return_type PropertyHelper<glm::quat>::toString(
    PropertyHelper<glm::quat>::pass_type val)
{
    char buff[128];
    snprintf(buff, sizeof(buff), "w:%g x:%g y:%g z:%g", val.w, val.x, val.y, val.z);

    return String(buff);
}


const String& PropertyHelper<uint>::getDataTypeName()
{
    static const String type("uint");

    return type;
}

PropertyHelper<uint>::return_type
PropertyHelper<uint>::fromString(const String& str)
{
    uint val = 0;

    if (str.empty())
        return val;

    std::stringstream sstream(str.c_str());
    sstream >> val;
    if (sstream.fail())
        logParsingError(getDataTypeName(), str);

    return val;
}


PropertyHelper<uint>::string_return_type PropertyHelper<uint>::toString(
    PropertyHelper<uint>::pass_type val)
{
    char buff[64];
    snprintf(buff, sizeof(buff), "%u", val);

    return String(buff);
}

#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UNICODE

const String& PropertyHelper<String::value_type>::getDataTypeName()
{
    static const String type("char");

    return type;
}

PropertyHelper<String::value_type>::return_type
PropertyHelper<String::value_type>::fromString(const String& str)
{
    return str[0];
}

PropertyHelper<String::value_type>::string_return_type PropertyHelper<String::value_type>::toString(
    PropertyHelper<String::value_type>::pass_type val)
{
    return String("") + val;
}

#endif

const String& PropertyHelper<String>::getDataTypeName()
{
    static const String type("String");

    return type;
}

PropertyHelper<String>::return_type PropertyHelper<String>::fromString(const String& str)
{
    return str;
}

PropertyHelper<String>::string_return_type PropertyHelper<String>::toString(
    PropertyHelper<String>::pass_type val)
{
    return val;
}

// Explicit instantiation definitions
template class PropertyHelper<String>;
template class PropertyHelper<float>;
template class PropertyHelper<double>;
template class PropertyHelper<int>;
template class PropertyHelper<uint>;
template class PropertyHelper<uint64>;
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UNICODE
template class PropertyHelper<String::value_type>;
#endif
template class PropertyHelper<unsigned long>;
template class PropertyHelper<bool>;
template class PropertyHelper<AspectMode>;
template class PropertyHelper<Sizef>;
template class PropertyHelper<glm::vec2>;
template class PropertyHelper<glm::vec3>;
template class PropertyHelper<glm::quat>;
template class PropertyHelper<Rectf>;
template class PropertyHelper<Image*>;
template class PropertyHelper<Colour>;
template class PropertyHelper<ColourRect>;
template class PropertyHelper<UDim>;
template class PropertyHelper<UVector2>;
template class PropertyHelper<URect>;
template class PropertyHelper<USize>;
template class PropertyHelper<UBox>;
template class PropertyHelper<Font*>;

}
