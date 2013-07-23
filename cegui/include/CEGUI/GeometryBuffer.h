/***********************************************************************
    filename:   CEGUIGeometryBuffer.h
    created:    Thu Jan 8 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIGeometryBuffer_h_
#define _CEGUIGeometryBuffer_h_

#include "CEGUI/Base.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/Rect.h"
#include "CEGUI/RefCounted.h"
#include "CEGUI/RenderMaterial.h"

#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
    class RenderMaterial;

//----------------------------------------------------------------------------//

/*!
\brief
    Enumerated type that contains the valid options to specify a vertex attribute
    of a vertex used in CEGUI
*/
enum VertexAttributeType
{
    //! Position 0 attribute
    VAT_POSITION0,
    //! Colour 0 attribute
    VAT_COLOUR0,
    //! Texture coordinate 0 attribute
    VAT_TEXCOORD0
};


/*!
\brief
    Abstract class defining the interface for objects that buffer geometry for
    later rendering.
*/
class CEGUIEXPORT GeometryBuffer :
    public AllocatedObject<GeometryBuffer>
{
public:
    virtual ~GeometryBuffer();

    /*!
    \brief
        Draw the geometry buffered within this GeometryBuffer object.
    */
    virtual void draw() const = 0;

    /*!
    \brief
        Set the translation to be applied to the geometry in the buffer when it
        is subsequently rendered.

    \param v
        Vector3 describing the three axis translation vector to be used.
    */
    virtual void setTranslation(const Vector3f& v) = 0;

    /*!
    \brief
        Set the rotations to be applied to the geometry in the buffer when it is
        subsequently rendered.

    \param r
        Quaternion describing the rotation to be used.
    */
    virtual void setRotation(const Quaternion& r) = 0;

    /*!
    \brief
        Set the pivot point to be used when applying the rotations.

    \param p
        Vector3 describing the location of the pivot point to be used when
        applying the rotation to the geometry.
    */
    virtual void setPivot(const Vector3f& p) = 0;

    /*!
    \brief
        Set the clipping region to be used when rendering this buffer.
    */
    virtual void setClippingRegion(const Rectf& region) = 0;

    /*!
    \brief
        Append the geometry data to the existing data

    \param vertex_data
        Pointer to an array of floats containing the geometry data that 
        should be added to the GeometryBuffer.

    \param array_size
        The number of elements in the array.
    */
    virtual void appendGeometry(const float* const vertex_data, uint array_size);

    
    /*!
    \brief
        Append the geometry data to the existing data

    \param vertex_data
        Vector of floats containing the geometry data that should be added to the
        GeometryBuffer.
    */
    virtual void appendGeometry(const std::vector<float>& vertex_data) = 0;

    /*!
    \brief
        Append a single vertex to the buffer.

    \param vertex
        Vertex object describing the vertex to be added to the GeometryBuffer.
    */
    virtual void appendVertex(const TexturedColouredVertex& vertex);

        /*!
    \brief
        Append a single vertex to the buffer.

    \param vertex
        Vertex object describing the vertex to be added to the GeometryBuffer.
    */
    virtual void appendVertex(const ColouredVertex& vertex);

    /*!
    \brief
        Append a number of vertices, with texture coordinate and colour attributes,
        from an array to the GeometryBuffer.

    \param vbuff
        Pointer to an array of Vertex objects that describe the vertices that
        are to be added to the GeometryBuffer.

    \param vertex_count
        The number of Vertex objects from the array \a vbuff that are to be
        added to the GeometryBuffer.
    */
    virtual void appendGeometry(const TexturedColouredVertex* const vbuff, uint vertex_count);

    /*!
    \brief
        Set the active texture to be used with all subsequently added vertices.

    \param texture
        Pointer to a Texture object that shall be used for subsequently added
        vertices.  This may be 0, in which case texturing will be disabled for
        subsequently added vertices.
    */
    virtual void setActiveTexture(Texture* texture) = 0;

    /*!
    \brief
        Clear all buffered data and reset the GeometryBuffer to the default
        state.
    */
    virtual void reset() = 0;

    /*!
    \brief
        Return a pointer to the currently active Texture object.  This may
        return 0 if no texture is set.

    \return
        Pointer the Texture object that is currently active, or 0 if texturing
        is not being used.
    */
    virtual Texture* getActiveTexture() const = 0;

    /*!
    \brief
        Return the total number of vertices currently held by this
        GeometryBuffer object.

    \return
        The number of vertices that have been appended to this GeometryBuffer.
    */
    virtual uint getVertexCount() const = 0;

    
    /*!
    \brief
        Return the number of floats used by the attributes contained in the
        current vertex layout.

    \return
        The number of floats used by the attributes contained in the current
        vertex layout.
    */
    int getVertexAttributeElementCount() const;

    /*!
    \brief
        Return the number of batches of geometry that this GeometryBuffer has
        split the vertices into.

    \note
        How batching is done will be largely implementation specific, although
        it would be reasonable to expect that you will have <em>at least</em>
        one batch of geometry per texture switch.

    \return
        The number of batches of geometry held by the GeometryBuffer.
    */
    virtual uint getBatchCount() const = 0;

    /*!
    \brief
        Set the RenderEffect to be used by this GeometryBuffer.

    \param effect
        Pointer to the RenderEffect to be used during renderng of the
        GeometryBuffer.  May be 0 to remove a previously added RenderEffect.

    \note
        When adding a RenderEffect, the GeometryBuffer <em>does not</em> take
        ownership of, nor make a copy of, the passed RenderEffect - this means
        you need to be careful not to delete the RenderEffect if it might still
        be in use!
    */
    virtual void setRenderEffect(RenderEffect* effect) = 0;

    /*!
    \brief
        Return the RenderEffect object that is assigned to this GeometryBuffer
        or 0 if none.
    */
    virtual RenderEffect* getRenderEffect() = 0;

    /*!
    \brief
        Set the blend mode option to use when rendering this GeometryBuffer.

    \note
        The blend mode setting is not a 'state' setting, but is used for \e all
        geometry added to the buffer regardless of when the blend mode is set.

    \param mode
        One of the BlendMode enumerated values indicating the blending mode to
        be used.
    */
    virtual void setBlendMode(const BlendMode mode);

    /*!
    \brief
        Return the blend mode that is set to be used for this GeometryBuffer.

    \return
        One of the BlendMode enumerated values indicating the blending mode
        that will be used when rendering all geometry added to this
        GeometryBuffer object.
    */
    virtual BlendMode getBlendMode() const;

    /*!
    \brief
        Set whether clipping will be active for subsequently added vertices.

    \param active
        - true if vertices added after this call should be clipped to the
          clipping region defined for this GeometryBuffer.
        - false if vertices added after this call should not be clipped
          (other than to the edges of rendering target.
    */
    virtual void setClippingActive(const bool active) = 0;

    /*
    \brief
        Return whether clipping will be used for the current batch
        of vertices being defined.

    \return
        - true if vertices subsequently added to the GeometryBuffer will
          be clipped to the clipping region defined for this GeometryBuffer.
        - false if vertices subsequently added will not be clipped (other than
          to the edges of the rendering target).
    */
    virtual bool isClippingActive() const = 0;

    /*
    \brief
        Resets the vertex attributes that were set for the vertices of this
        GeometryBuffer.
    */
    void resetVertexAttributes();

    /*
    \brief
        Resets the vertex attributes that were set for the vertices of this
        GeometryBuffer.

    \param attribute
        The attribute that should be appended to the list of vertex attributes
        describing the vertices of this GeometryBuffer.
    */
    void appendVertexAttribute(VertexAttributeType attribute);

        /*
    \brief
        The update function that is to be called when all the vertex attributes
        are set.
    */
    virtual void finaliseVertexAttributes() = 0;

protected:
    //! Constructor.
    GeometryBuffer(RefCounted<RenderMaterial> renderMaterial);

    //! The BlendMode to use when rendering this GeometryBuffer.
    BlendMode d_blendMode;

    //! Reference to the RenderMaterial used for this GeometryBuffer
    RefCounted<RenderMaterial> d_renderMaterial;
    
    //! A vector of the attributes of the vertices of this GeometryBuffer. The order
    // in which they were added to the vector is used to define the alignment of the
    // vertex data.
    std::vector<VertexAttributeType> d_vertexAttributes;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
