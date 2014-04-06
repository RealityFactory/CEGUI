/***********************************************************************
    filename:   ShaderParameterBindings.h
    created:    18th July 2013
    author:     Lukas Meindl
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
#ifndef _CEGUIShaderParameterBindings_h_
#define _CEGUIShaderParameterBindings_h_

#include "CEGUI/Base.h"

#include "glm/glm.hpp"

#include <map>
#include <string>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


namespace CEGUI
{
class Texture;

//----------------------------------------------------------------------------//
/*!
\brief
    Enumerated type that contains the valid types a shader parameter can have
*/
enum ShaderParamType
{
    //! A regular integer type
    SPT_INT,
    //! A regular float type
    SPT_FLOAT,
    //! A pointer to a CEGUI Texture
    SPT_TEXTURE,
    //! A pointer to a 4x4 Matrix
    SPT_MATRIX_4X4,

    //! Total number of shader parameter types
    SPT_COUNT
};
//----------------------------------------------------------------------------//

/*!
\brief
    The class is the abstract interface used for all shader parameters that
    are added to the RenderMaterials.
*/
class ShaderParameter : public AllocatedObject<ShaderParameter>
{
public:
    ShaderParameter()
    {}

    virtual ~ShaderParameter()
    {}

    ShaderParameter(const ShaderParameter& other)
    {}

    /*!
    \brief
        Returns the type of the shader parameter.

    \return
        The type of the shader parameter.
    */
    virtual ShaderParamType getType() const = 0;

    /*!
    \brief
        Returns a copy of this ShaderParameter on the heap and returns the
        pointer to it. The caller has to take care of memory deallocation.

    \return
        A pointer to the copy of this ShaderParameter.
    */
    virtual ShaderParameter* clone() const = 0;

    /*!
    \brief
        Checks if the ShaderParameters are equal, depending on their type
        and parameter.

    \return
        True if the ShaderParameters are equal.
    */
    virtual bool equal(const ShaderParameter* other_parameter) const = 0;

    /*!
    \brief
        Copies the parameter value from the passed ShaderParameter to this
        one if the classes are of the same type.
    \param other_parameter
        The ShaderParameter from which the parameter value should be taken over.
    */
    virtual void takeOverParameterValue(const ShaderParameter* other_parameter) = 0;
};

/*!
\brief
    The class implements the functionality of the
    ShaderParameter interface for float parameters.
*/
class ShaderParameterFloat : public ShaderParameter
{
public:
    ShaderParameterFloat(float parameterValue)
    : d_parameterValue(parameterValue)
    {}

    //! Implementation of the shader_parameter interface
    virtual ShaderParamType getType() const
    { return SPT_FLOAT; }
    virtual ShaderParameterFloat* clone() const
    { return new ShaderParameterFloat(*this); }
    bool equal(const ShaderParameter* other_parameter) const;
    void takeOverParameterValue(const ShaderParameter* other_parameter);

    //! The float parameter value
    float d_parameterValue;
};

/*!
\brief
    The class implements the functionality of the
    ShaderParameter interface for int parameters.
*/
class ShaderParameterInt : public ShaderParameter
{
public:
    ShaderParameterInt(int parameterValue)
    : d_parameterValue(parameterValue)
    {}

    //! Implementation of the shader_parameter interface
    virtual ShaderParamType getType() const
    { return SPT_INT; }
    virtual ShaderParameterInt* clone() const
    { return new ShaderParameterInt(*this); }
    bool equal(const ShaderParameter* other_parameter) const;
    void takeOverParameterValue(const ShaderParameter* other_parameter);

    //! The float parameter value
    int d_parameterValue;
};

/*!
\brief
    The class implements the functionality of the
    ShaderParameter interface for Texture parameters.
*/
class ShaderParameterTexture : public ShaderParameter
{
public:
    ShaderParameterTexture(const CEGUI::Texture* parameterValue)
        : d_parameterValue(parameterValue)
    {}

    //! Implementation of the shader_parameter interface
    virtual ShaderParamType getType() const
    { return SPT_TEXTURE; }
    virtual ShaderParameterTexture* clone() const
    { return new ShaderParameterTexture(*this); }
    bool equal(const ShaderParameter* other_parameter) const;
    void takeOverParameterValue(const ShaderParameter* other_parameter);

    //! The float parameter value
    const CEGUI::Texture* d_parameterValue;
};

/*!
\brief
    The class implements the functionality of the
    ShaderParameter interface for 4x4 matrix parameters.
*/
class ShaderParameterMatrix : public ShaderParameter
{
public:
    ShaderParameterMatrix(const glm::mat4& parameterValue)
        : d_parameterValue(parameterValue)
    {}

    //! Implementation of the shader_parameter interface
    virtual ShaderParamType getType() const
    { return SPT_MATRIX_4X4; }
    virtual ShaderParameterMatrix* clone() const
    { return new ShaderParameterMatrix(*this); }
    bool equal(const ShaderParameter* other_parameter) const;
    void takeOverParameterValue(const ShaderParameter* other_parameter);

    //! The float parameter value
    glm::mat4 d_parameterValue;
};

/*!
\brief

*/
class CEGUIEXPORT ShaderParameterBindings : public AllocatedObject<ShaderParameterBindings>
{
public:
    typedef std::map<std::string, ShaderParameter*> ShaderParameterBindingsMap;

    ShaderParameterBindings();
    ~ShaderParameterBindings();


    /*!
    \brief
        Adds a matrix shader parameter to the parameter bindings

    \param parameter_name
        The name of the parameter as used by the shader

    \param matrix
        The pointer to the matrix
    */
    void setParameter(const std::string& parameter_name, const glm::mat4& matrix);

    /*!
    \brief
        Adds a texture shader parameter to the parameter bindings

    \param parameter_name
        The name of the parameter as used by the shader

    \param texture
        The pointer to the CEGUI::Texture
    */
    void setParameter(const std::string& parameter_name, const CEGUI::Texture* texture);



    /*!
    \brief
        Returns a pointer to the shader_parameter with the specified parameter name

    \param parameter_name
        The name of the parameter as used by the shader

    \return
        The pointer to the required shader_parameter. Will return 0 if a parameter
        with the specified name was not set.
    */
    ShaderParameter* getParameter(const std::string& parameter_name);

    /*!
    \brief
        Sets the shader_parameter in the map to 0, which means that the shader parameter
        will remain unchanged during rendering

    \param parameter_name
        The name of the parameter as used by the shader
    */
    void removeParameter(const std::string& parameter_name);

    const ShaderParameterBindingsMap& getShaderParameterBindings() const;

protected:
    /*!
    \brief
        Adds a new shader parameter to the parameter bindings. If an old one exists it will be
        deleted and replaced by the new one.

    \param parameter_name
        The name of the parameter as used by the shader

    \param shader_parameter
        The pointer to the shader_parameter
    */
    void setNewParameter(const std::string& parameter_name, ShaderParameter* shader_parameter);

    //! Map of the names of the shader parameter and the respective shader parameter value
    ShaderParameterBindingsMap d_shaderParameterBindings;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif


#endif
