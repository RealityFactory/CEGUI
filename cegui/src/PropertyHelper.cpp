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

#include <sstream>

namespace CEGUI
{

//! Definitions of static constants
const CEGUI::String PropertyHelper<bool>::True("true");
const CEGUI::String PropertyHelper<bool>::False("false");

const CEGUI::String PropertyHelper<AspectMode>::Shrink("Shrink");
const CEGUI::String PropertyHelper<AspectMode>::Expand("Expand");
const CEGUI::String PropertyHelper<AspectMode>::Ignore("Ignore");

//! Helper function for throwing errors
static void throwParsingException(const String& typeName, const String& parsedstring)
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
        throwParsingException(getDataTypeName(), str);
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
        throwParsingException(getDataTypeName(), str);
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
        throwParsingException(getDataTypeName(), str);

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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> ud;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return ud;
}

PropertyHelper<UDim>::string_return_type PropertyHelper<UDim>::toString(
    PropertyHelper<UDim>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> uv;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return uv;
}

PropertyHelper<UVector2>::string_return_type PropertyHelper<UVector2>::toString(
    PropertyHelper<UVector2>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> uv;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return uv;
}

PropertyHelper<USize>::string_return_type PropertyHelper<USize>::toString(
    PropertyHelper<USize>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> ur;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return ur;
}

PropertyHelper<URect>::string_return_type PropertyHelper<URect>::toString(
    PropertyHelper<URect>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> ret;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return ret;
}

PropertyHelper<UBox>::string_return_type PropertyHelper<UBox>::toString(
    PropertyHelper<UBox>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
}

const String& PropertyHelper<ColourRect>::getDataTypeName()
{
    static const String type("ColourRect");

    return type;
}

PropertyHelper<ColourRect>::return_type
PropertyHelper<ColourRect>::fromString(const String& str)
{
    ColourRect val(Colour(0xFF000000));

    if (str.empty())
         return val;

    std::stringstream& sstream = getPreparedStream(str);

    if (str.length() == 8)
    {
        CEGUI::Colour colourForEntireRect(0xFF000000);

        sstream >> colourForEntireRect;
        if (sstream.fail())
            throwParsingException(getDataTypeName(), str);

        val = ColourRect(colourForEntireRect);

        return val;
    }
    else
    {
        sstream >> val;
        if (sstream.fail())
            throwParsingException(getDataTypeName(), str);

        return val;
    }

    return val;
}

PropertyHelper<ColourRect>::string_return_type PropertyHelper<ColourRect>::toString(
    PropertyHelper<ColourRect>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();

    if(val.isMonochromatic())
        sstream << val.d_top_left;
    else
        sstream << val;

    return String(sstream.str());
}

const String& PropertyHelper<Colour>::getDataTypeName()
{
    static const String type("Colour");

    return type;
}

PropertyHelper<Colour>::return_type
PropertyHelper<Colour>::fromString(const String& str)
{
    Colour val(0xFF000000);

    if (str.empty())
        return val;

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> val;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);
    

    return Colour(val);
}

PropertyHelper<Colour>::string_return_type PropertyHelper<Colour>::toString(
    PropertyHelper<Colour>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> val;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return val;
}

PropertyHelper<Rectf>::string_return_type PropertyHelper<Rectf>::toString(
    PropertyHelper<Rectf>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> val;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return val;
}

PropertyHelper<Sizef>::string_return_type PropertyHelper<Sizef>::toString(
    PropertyHelper<Sizef>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> val;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return val;
}


PropertyHelper<double>::string_return_type PropertyHelper<double>::toString(
    PropertyHelper<double>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> val;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return val;
}


PropertyHelper<int>::string_return_type PropertyHelper<int>::toString(
    PropertyHelper<int>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
}

const String& PropertyHelper<std::uint32_t>::getDataTypeName()
{
    static const String type("std::uint32_t");

    return type;
}

PropertyHelper<std::uint32_t>::return_type
PropertyHelper<std::uint32_t>::fromString(const String& str)
{
    std::uint32_t val = 0;

    if (str.empty())
        return val;

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> val;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return val;
}


PropertyHelper<std::uint32_t>::string_return_type PropertyHelper<std::uint32_t>::toString(
    PropertyHelper<std::uint32_t>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
}

const String& PropertyHelper<std::uint64_t>::getDataTypeName()
{
    static const String type("std::uint64_t");

    return type;
}

PropertyHelper<std::uint64_t>::return_type
PropertyHelper<std::uint64_t>::fromString(const String& str)
{
    std::uint64_t val = 0;

    if (str.empty())
        return val;

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> val;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return val;
}


PropertyHelper<std::uint64_t>::string_return_type PropertyHelper<std::uint64_t>::toString(
    PropertyHelper<std::uint64_t>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << val;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> MandatoryString(" x :") >> val.x >> MandatoryString(" y :") >> val.y;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return val;
}

PropertyHelper<glm::vec2>::string_return_type PropertyHelper<glm::vec2>::toString(
    PropertyHelper<glm::vec2>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << "x:" << val.x << " y:" << val.y;

    return String(sstream.str());
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

    std::stringstream& sstream = getPreparedStream(str);
    sstream >> MandatoryString(" x :") >> val.x >> MandatoryString(" y :") >> val.y >> MandatoryString(" z :") >> val.z;
    if (sstream.fail())
        throwParsingException(getDataTypeName(), str);

    return val;
}

PropertyHelper<glm::vec3>::string_return_type PropertyHelper<glm::vec3>::toString(
    PropertyHelper<glm::vec3>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << "x:" << val.x << " y:" << val.y << " z:" << val.z;

    return String(sstream.str());
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
        std::stringstream& sstream = getPreparedStream(str);
        sstream >> MandatoryString(" w :") >> val.w >> MandatoryString(" x :") >> val.x >> MandatoryString(" y :") >> val.y >> MandatoryString(" z :") >> val.z;
        if (sstream.fail())
            throwParsingException(getDataTypeName(), str);
        return val;
    }
    else
    {
        float x, y, z;
        // CEGUI takes degrees because it's easier to work with
        std::stringstream& sstream = getPreparedStream(str);
        sstream >> MandatoryString(" x :") >> x >> MandatoryString(" y :") >> y >> MandatoryString(" z :") >> z;
        if (sstream.fail())
            throwParsingException(getDataTypeName(), str);

        // glm::radians converts from degrees to radians
        return glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z)));
    }
}

PropertyHelper<glm::quat>::string_return_type PropertyHelper<glm::quat>::toString(
    PropertyHelper<glm::quat>::pass_type val)
{
    std::stringstream& sstream = getPreparedStream();
    sstream << "w:" << val.w << " x:" << val.x << " y:" << val.y << " z:" << val.z;

    return String(sstream.str());
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
template class PropertyHelper<std::uint32_t>;
template class PropertyHelper<std::uint64_t>;
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UNICODE
template class PropertyHelper<String::value_type>;
#endif
template class PropertyHelper<bool>;
template class PropertyHelper<AspectMode>;
template class PropertyHelper<USize>;
template class PropertyHelper<Sizef>;
template class PropertyHelper<glm::vec2>;
template class PropertyHelper<glm::vec3>;
template class PropertyHelper<glm::quat>;
template class PropertyHelper<Image*>;
template class PropertyHelper<Colour>;
template class PropertyHelper<ColourRect>;
template class PropertyHelper<UDim>;
template class PropertyHelper<UVector2>;
template class PropertyHelper<URect>;
template class PropertyHelper<Rectf>;
template class PropertyHelper<UBox>;
template class PropertyHelper<Font*>;

}
