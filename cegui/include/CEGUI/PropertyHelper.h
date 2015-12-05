/***********************************************************************
	created:	17th August 2015
	author:		Lukas Meindl (reworked from code by Martin Preisler who reworked his code from code by Paul D Turner)
	
	purpose:	Interface to the PropertyHelper class
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
#ifndef _CEGUIPropertyHelper_h_
#define _CEGUIPropertyHelper_h_

#include "CEGUI/String.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/USize.h"
#include "CEGUI/UVector.h"
#include "CEGUI/Colour.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/UDim.h"
#include "CEGUI/Rectf.h"
#include "CEGUI/URect.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstdio>

namespace CEGUI
{
/*!
\brief
	Helper class used to convert various data types to and from the format expected in Property strings

\par
    Usage:

    float value = PropertyHelper<float>::fromString("0.1");
    String value = PropertyHelper<float>::toString(0.1f);
*/
template<typename T>
class PropertyHelper;

// this redirects PropertyHelper<const T> to PropertyHelper<T>
template<typename T>
class PropertyHelper<const T>
{
public:
    typedef typename PropertyHelper<T>::return_type return_type;
    typedef typename PropertyHelper<T>::safe_method_return_type safe_method_return_type;
    typedef typename PropertyHelper<T>::pass_type pass_type;
    typedef typename PropertyHelper<T>::string_return_type string_return_type;

    static inline const String& getDataTypeName()
    {
        return PropertyHelper<T>::getDataTypeName();
    }

    static inline return_type fromString(const String& str)
    {
        return PropertyHelper<T>::fromString(str);
    }

    static inline String toString(pass_type val)
    {
        return PropertyHelper<T>::toString(val);
    }
   
};


// this redirects PropertyHelper<const T&> to PropertyHelper<T>
template<typename T>
class PropertyHelper<const T&>
{
public:
    typedef typename PropertyHelper<T>::return_type return_type;
    typedef typename PropertyHelper<T>::safe_method_return_type safe_method_return_type;
    typedef typename PropertyHelper<T>::pass_type pass_type;
    typedef typename PropertyHelper<T>::string_return_type string_return_type;

    static inline const String& getDataTypeName()
    {
        return PropertyHelper<T>::getDataTypeName();
    }

    static inline return_type fromString(const String& str)
    {
        return PropertyHelper<T>::fromString(str);
    }

    static inline String toString(pass_type val)
    {
        return PropertyHelper<T>::toString(val);
    }
};

// this redirects PropertyHelper<const T*> to PropertyHelper<T*>
template<typename T>
class PropertyHelper<const T*>
{
public:
    typedef typename PropertyHelper<T*>::return_type return_type;
    typedef typename PropertyHelper<T*>::safe_method_return_type safe_method_return_type;
    typedef typename PropertyHelper<T*>::pass_type pass_type;
    typedef typename PropertyHelper<T*>::string_return_type string_return_type;

    static inline const String& getDataTypeName()
    {
        return PropertyHelper<T>::getDataTypeName();
    }

    static inline return_type fromString(const String& str)
    {
        return PropertyHelper<T*>::fromString(str);
    }

    static inline String toString(pass_type val)
    {
        return PropertyHelper<T*>::toString(val);
    }
};



template<>
class CEGUIEXPORT PropertyHelper<String>
{
public:
    typedef const String& return_type;
    typedef String safe_method_return_type;
    typedef const String& pass_type;
    typedef const String& string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<float>
{
public:
    typedef float return_type;
    typedef return_type safe_method_return_type;
    typedef const float pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<double>
{
public:
    typedef double return_type;
    typedef return_type safe_method_return_type;
    typedef const double pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::int16_t>
{
public:
    typedef std::int16_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::int16_t pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::int32_t>
{
public:
    typedef std::int32_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::int32_t pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::int64_t>
{
public:
    typedef std::int64_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::int64_t pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::uint32_t>
{
public:
    typedef std::uint32_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::uint32_t pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<std::uint64_t>
{
public:
    typedef std::uint64_t return_type;
    typedef return_type safe_method_return_type;
    typedef const std::uint64_t pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};


#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UNICODE

template<>
class CEGUIEXPORT PropertyHelper<String::value_type>
{
public:
    typedef String::value_type return_type;
    typedef return_type safe_method_return_type;
    typedef const String::value_type pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

#endif

template<> 
class CEGUIEXPORT PropertyHelper<bool>
{
public:
    typedef bool return_type;
    typedef return_type safe_method_return_type;
    typedef const bool pass_type;
    typedef const String& string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String True;
    static const CEGUI::String False;
};



template<> 
class CEGUIEXPORT PropertyHelper<AspectMode>
{
public:
    typedef AspectMode return_type;
    typedef return_type safe_method_return_type;
    typedef AspectMode pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);

    //! Definitions of the possible values represented as Strings
    static const CEGUI::String Shrink;
    static const CEGUI::String Expand;
    static const CEGUI::String Ignore;
};

template<>
class CEGUIEXPORT PropertyHelper<glm::vec2>
{
public:
    typedef glm::vec2 return_type;
    typedef return_type safe_method_return_type;
    typedef const glm::vec2& pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<glm::vec3>
{
public:
    typedef glm::vec3 return_type;
    typedef return_type safe_method_return_type;
    typedef const glm::vec3& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<glm::quat>
{
public:
    typedef glm::quat return_type;
    typedef return_type safe_method_return_type;
    typedef const glm::quat& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<Image*>
{
public:
    typedef const Image* return_type;
    typedef return_type safe_method_return_type;
    typedef const Image* const pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<Colour>
{
public:
    typedef Colour return_type;
    typedef return_type safe_method_return_type;
    typedef const Colour& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<ColourRect>
{
public:
    typedef ColourRect return_type;
    typedef return_type safe_method_return_type;
    typedef const ColourRect& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<UDim>
{
public:
    typedef UDim return_type;
    typedef return_type safe_method_return_type;
    typedef const UDim& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<UVector2>
{
public:
    typedef UVector2 return_type;
    typedef return_type safe_method_return_type;
    typedef const UVector2& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<USize>
{
public:
    typedef USize return_type;
    typedef return_type safe_method_return_type;
    typedef const USize& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<URect>
{
public:
    typedef URect return_type;
    typedef return_type safe_method_return_type;
    typedef const URect& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<Sizef>
{
public:
    typedef Sizef return_type;
    typedef return_type safe_method_return_type;
    typedef const Sizef& pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<Rectf>
{
public:
    typedef Rectf return_type;
    typedef return_type safe_method_return_type;
    typedef const Rectf& pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};


template<>
class CEGUIEXPORT PropertyHelper<UBox>
{
public:
    typedef UBox return_type;
    typedef return_type safe_method_return_type;
    typedef const UBox& pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};


template<>
class CEGUIEXPORT PropertyHelper<Font*>
{
public:
    typedef const Font* return_type;
    typedef return_type safe_method_return_type;
    typedef const Font* const pass_type;
    typedef String string_return_type;
    
    static const String& getDataTypeName();
    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};


// Explicit instantiation declarations
extern template CEGUIEXPORT class PropertyHelper<String>;
extern template CEGUIEXPORT class PropertyHelper<float>;
extern template CEGUIEXPORT class PropertyHelper<double>;
extern template CEGUIEXPORT class PropertyHelper<std::int16_t>;
extern template CEGUIEXPORT class PropertyHelper<std::int32_t>;
extern template CEGUIEXPORT class PropertyHelper<std::int64_t>;
extern template CEGUIEXPORT class PropertyHelper<std::uint32_t>;
extern template CEGUIEXPORT class PropertyHelper<std::uint64_t>;
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UNICODE
extern template CEGUIEXPORT class PropertyHelper<String::value_type>;
#endif
extern template CEGUIEXPORT class PropertyHelper<bool>;
extern template CEGUIEXPORT class PropertyHelper<AspectMode>;
extern template CEGUIEXPORT class PropertyHelper<glm::vec2>;
extern template CEGUIEXPORT class PropertyHelper<glm::vec3>;
extern template CEGUIEXPORT class PropertyHelper<glm::quat>;
extern template CEGUIEXPORT class PropertyHelper<Image*>;
extern template CEGUIEXPORT class PropertyHelper<Colour>;
extern template CEGUIEXPORT class PropertyHelper<ColourRect>;
extern template CEGUIEXPORT class PropertyHelper<UDim>;
extern template CEGUIEXPORT class PropertyHelper<UVector2>;
extern template CEGUIEXPORT class PropertyHelper<Rectf>;
extern template CEGUIEXPORT class PropertyHelper<Sizef>;
extern template CEGUIEXPORT class PropertyHelper<URect>;
extern template CEGUIEXPORT class PropertyHelper<USize>;
extern template CEGUIEXPORT class PropertyHelper<UBox>;
extern template CEGUIEXPORT class PropertyHelper<Font*>;


}

#endif
