/***********************************************************************
    filename:   GeometryBufferBase.cpp
    created:    Tue Apr 30 2013
    authors:    Paul D Turner <paul@cegui.org.uk>
                Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "CEGUI/RendererModules/OpenGL/GeometryBufferBase.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/RendererModules/OpenGL/GlmPimpl.h"
#include "CEGUI/ShaderParameterBindings.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLGeometryBufferBase::OpenGLGeometryBufferBase(OpenGLRendererBase& owner, CEGUI::RefCounted<RenderMaterial> renderMaterial) :
    GeometryBuffer(renderMaterial),
    d_owner(&owner),
    d_activeTexture(0),
    d_clipRect(0, 0, 0, 0),
    d_clippingActive(true),
    d_translation(0, 0, 0),
    d_rotation(Quaternion::IDENTITY),
    d_pivot(0, 0, 0),
    d_effect(0),
    d_matrix(new mat4Pimpl()),
    d_matrixValid(false),
    d_vertexCount(0)
{
}

//----------------------------------------------------------------------------//
OpenGLGeometryBufferBase::~OpenGLGeometryBufferBase()
{
    delete d_matrix;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setTranslation(const Vector3f& v)
{
    d_translation = v;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setRotation(const Quaternion& r)
{
    d_rotation = r;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setPivot(const Vector3f& p)
{
    d_pivot = Vector3f(p.d_x, p.d_y, p.d_z);
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setClippingRegion(const Rectf& region)
{
    d_clipRect.top(ceguimax(0.0f, region.top()));
    d_clipRect.left(ceguimax(0.0f, region.left()));
    d_clipRect.bottom(ceguimax(0.0f, region.bottom()));
    d_clipRect.right(ceguimax(0.0f, region.right()));
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setTexture(Texture* texture)
{
    if(d_activeTexture != texture)
    {
        d_activeTexture = texture;
        CEGUI::ShaderParameterBindings* shaderParameterBindings = (*d_renderMaterial).getShaderParamBindings();
        shaderParameterBindings->setParameter("texture0", d_activeTexture);
    }
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::reset()
{
    d_vertexData.clear();
    d_activeTexture = 0;
    d_clippingActive = true;
}

//----------------------------------------------------------------------------//
Texture* OpenGLGeometryBufferBase::getTexture() const
{
    return d_activeTexture;
}

//----------------------------------------------------------------------------//
uint OpenGLGeometryBufferBase::getVertexCount() const
{
    return d_vertexData.size();
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* OpenGLGeometryBufferBase::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
const mat4Pimpl* OpenGLGeometryBufferBase::getMatrix() const
{
    if (!d_matrixValid)
        updateMatrix();

    return d_matrix;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::updateMatrix() const
{
    glm::mat4& modelMatrix = d_matrix->d_matrix;
    modelMatrix = glm::mat4(1.f);

    const glm::vec3 final_trans(d_translation.d_x + d_pivot.d_x,
                                d_translation.d_y + d_pivot.d_y,
                                d_translation.d_z + d_pivot.d_z);

    modelMatrix = glm::translate(modelMatrix, final_trans);

    glm::quat rotationQuat = glm::quat(d_rotation.d_w, d_rotation.d_x, d_rotation.d_y, d_rotation.d_z);
    glm::mat4 rotation_matrix = glm::mat4_cast(rotationQuat);

    modelMatrix = modelMatrix * rotation_matrix;

    glm::vec3 transl = glm::vec3(-d_pivot.d_x, -d_pivot.d_y, -d_pivot.d_z);
    glm::mat4 translMatrix = glm::translate(glm::mat4(1.f), transl);
    modelMatrix =  modelMatrix * translMatrix;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setClippingActive(const bool active)
{
    d_clippingActive = active;
}

//----------------------------------------------------------------------------//
bool OpenGLGeometryBufferBase::isClippingActive() const
{
    return d_clippingActive;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::appendGeometry(const std::vector<float>& vertex_data)
{
    d_vertexData.insert(d_vertexData.end(), vertex_data.begin(), vertex_data.end());
    // Update size of geometry buffer
    d_vertexCount = d_vertexData.size() / getVertexAttributeElementCount();
}


//----------------------------------------------------------------------------//

}

