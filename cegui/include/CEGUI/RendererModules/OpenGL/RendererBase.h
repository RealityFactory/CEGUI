/***********************************************************************
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
#ifndef _CEGUIRendererBase_h_
#define _CEGUIRendererBase_h_

#include "../../Base.h"
#include "../../Renderer.h"
#include "../../Size.h"
#include "../../Vector.h"
#include "../../Rect.h"
#include "../../TextureTarget.h"
#include "../../RefCounted.h"
#include "CEGUI/RendererModules/OpenGL/GL.h"

#include "glm/glm.hpp"

#include <vector>
#include <unordered_map>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class OpenGLTexture;
class OpenGLGeometryBufferBase;
class RenderMaterial;

//! Common base class used for other OpenGL (desktop or ES) based renderer modules.
class OPENGL_GUIRENDERER_API OpenGLRendererBase : public Renderer
{
public:
    // implement Renderer interface
    virtual RenderTarget& getDefaultRenderTarget();
    virtual GeometryBuffer& createGeometryBufferTextured(CEGUI::RefCounted<RenderMaterial> renderMaterial);
    virtual GeometryBuffer& createGeometryBufferColoured(CEGUI::RefCounted<RenderMaterial> renderMaterial);
    virtual TextureTarget* createTextureTarget(bool addStencilBuffer);
    virtual void destroyTextureTarget(TextureTarget* target);
    virtual void destroyAllTextureTargets();
    virtual Texture& createTexture(const String& name);
    virtual Texture& createTexture(const String& name,
                           const String& filename,
                           const String& resourceGroup);
    virtual Texture& createTexture(const String& name, const Sizef& size);
    virtual void destroyTexture(Texture& texture);
    virtual void destroyTexture(const String& name);
    virtual void destroyAllTextures();
    virtual Texture& getTexture(const String& name) const;
    virtual bool isTextureDefined(const String& name) const;
    virtual void setDisplaySize(const Sizef& sz);
    virtual const Sizef& getDisplaySize() const;
    virtual const glm::vec2& getDisplayDPI() const;
    virtual uint getMaxTextureSize() const;
    virtual const String& getIdentifierString() const;
    virtual bool isTexCoordSystemFlipped() const;

    /*!
    \brief
        Create a texture that uses an existing OpenGL texture with the specified
        size.  Note that it is your responsibility to ensure that the OpenGL
        texture is valid and that the specified size is accurate.

    \param sz
        Size object that describes the pixel size of the OpenGL texture
        identified by \a tex.

    \param name
        String holding the name for the new texture.  Texture names must be
        unique within the Renderer.

    \return
        Texture object that wraps the OpenGL texture \a tex, and whose size is
        specified to be \a sz.

    \exceptions
        - AlreadyExistsException - thrown if a Texture object named \a name
          already exists within the system.
    */
    Texture& createTexture(const String& name, GLuint tex, const Sizef& sz);

    /*!
    \brief
        Tells the renderer to initialise some extra states beyond what it
        directly needs itself for CEGUI.

        This option is useful in cases where you've made changes to the default
        OpenGL state and do not want to save/restore those between CEGUI
        rendering calls.  Note that this option will not deal with every
        possible state or extension - if you want a state added here, make a
        request and we'll consider it ;)
    */
    void enableExtraStateSettings(bool setting);

    /*!
    \brief
        Grabs all the loaded textures from Texture RAM and stores them in a
        local data buffer.  This function invalidates all textures, and
        restoreTextures must be called before any CEGUI rendering is done for
        predictable results.
    */
    void grabTextures();

    /*!
    \brief
        Restores all the loaded textures from the local data buffers previously
        created by 'grabTextures'
    */
    void restoreTextures();

    /*!
    \brief
        Helper to return a valid texture size according to reported OpenGL
        capabilities.

    \param sz
        Size object containing input size.

    \return
        Size object containing - possibly different - output size.
    */
    virtual Sizef getAdjustedTextureSize(const Sizef& sz) = 0;

    /*!
    \brief
        Utility function that will return \a f if it's a power of two, or the
        next power of two up from \a f if it's not.
    */
    static float getNextPOTSize(const float f);

    //! set the render states for the specified BlendMode.
    virtual void setupRenderingBlendMode(const BlendMode mode,
                                         const bool force = false) = 0;

    /*!
    \brief
        Helper to get the viewport.

    \return
        The viewport.
    */
    const CEGUI::Rectf& getActiveViewPort();

protected:
    OpenGLRendererBase();

    /*!
    \brief
        Constructor.

    \param display_size
        Size object describing the initial display resolution.
    */
    OpenGLRendererBase(const Sizef& display_size);
    
    OpenGLRendererBase(bool set_glew_experimental);

    /*!
    \brief
        Constructor.

    \param display_size
        Size object describing the initial display resolution.
    \param set_glew_experimental
        If true, set "glewExperimental = GL_TRUE" before calling "glewInit".
    */
    OpenGLRendererBase(const Sizef& display_size, bool set_glew_experimental);
    
    void init (bool init_glew=false, bool set_glew_experimental=false);

    //! Destructor!
    virtual ~OpenGLRendererBase();

    //! helper to safely log the creation of a named texture
    static void logTextureCreation(const String& name);
    //! helper to safely log the destruction of a named texture
    static void logTextureDestruction(const String& name);

    //! helper to set (rough) max texture size.
    void initialiseMaxTextureSize();

    //! helper to set display size with current viewport size.
    void initialiseDisplaySizeWithViewportSize();

    //! return some appropriate OpenGLGeometryBufferBase subclass instance.
    virtual OpenGLGeometryBufferBase* createGeometryBuffer_impl(RefCounted<RenderMaterial> renderMaterial) = 0;

    //! return some appropriate TextureTarget subclass instance.
    virtual TextureTarget* createTextureTarget_impl(bool addStencilBuffer) = 0;

    //! return some appropriate Texture subclass instance.
    virtual OpenGLTexture* createTexture_impl(const String& name) = 0;

    //! String holding the renderer identification text.
    static String d_rendererID;
    //! What the renderer considers to be the current display size.
    Sizef d_displaySize;
    //! What the renderer considers to be the current display DPI resolution.
    glm::vec2 d_displayDPI;
    //! The default RenderTarget
    RenderTarget* d_defaultTarget;
    //! container type used to hold TextureTargets we create.
    typedef std::vector<TextureTarget*> TextureTargetList;
    //! Container used to track texture targets.
    TextureTargetList d_textureTargets;
    //! container type used to hold Textures we create.
    typedef std::unordered_map<String, OpenGLTexture*> TextureMap;
    //! Container used to track textures.
    TextureMap d_textures;
    //! What the renderer thinks the max texture size is.
    uint d_maxTextureSize;
    //! option of whether to initialise extra states that may not be at default
    bool d_initExtraStates;
    //! What blend mode we think is active.
    BlendMode d_activeBlendMode;
};

/**
    This class allows us to implement a factory template for creating and
    destroying any type of TextureTarget.  The code that detects
    the computer's abilities will generate an appropriate factory for a
    TextureTarget based on what the host system can provide - or use the
    default 'null' factory if no suitable TextureTargets are available.
*/
class OGLTextureTargetFactory
{
public:
    OGLTextureTargetFactory() {}
    virtual ~OGLTextureTargetFactory() {}
    virtual TextureTarget* create(OpenGLRendererBase&) const
        { return 0; }
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif

