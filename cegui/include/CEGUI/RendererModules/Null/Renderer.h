/***********************************************************************
    created:    Fri Jan 15 2010
    author:     Eugene Marcotte
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
#ifndef _CEGUINullRenderer_h_
#define _CEGUINullRenderer_h_

#include "../../Renderer.h"
#include "../../Sizef.h"

#include <vector>
#include <unordered_map>

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef CEGUINULLRENDERER_EXPORTS
#       define NULL_GUIRENDERER_API __declspec(dllexport)
#   else
#       define NULL_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define NULL_GUIRENDERER_API
#endif

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
class NullGeometryBuffer;
class NullTexture;
class NullShaderWrapper;

//! CEGUI::Renderer implementation for no particular engine
class NULL_GUIRENDERER_API NullRenderer : public Renderer
{
public:
    /*!
    \brief
        Convenience function that creates all the necessary objects
        then initialises the CEGUI system with them.

        This will create and initialise the following objects for you:
        - CEGUI::NullRenderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \param abi
        This must be set to CEGUI_VERSION_ABI

    \return
        Reference to the CEGUI::NullRenderer object that was created.

    */
    static NullRenderer& bootstrapSystem(const int abi = CEGUI_VERSION_ABI);
    
    /*!
    \brief
        Convenience function to cleanup the CEGUI system and related objects
        that were created by calling the bootstrapSystem function.

        This function will destroy the following objects for you:
        - CEGUI::System
        - CEGUI::DefaultResourceProvider
        - CEGUI::NullRenderer

    \note
        If you did not initialise CEGUI by calling the bootstrapSystem function,
        you should \e not call this, but rather delete any objects you created
        manually.
    */
    static void destroySystem();

    /*!
    \brief
        Create an NullRenderer object 
    */
    static NullRenderer& create(const int abi = CEGUI_VERSION_ABI);

    //! destroy an NullRenderer object.
    static void destroy(NullRenderer& renderer);

    // implement CEGUI::Renderer interface
    virtual RenderTarget& getDefaultRenderTarget();
    virtual RefCounted<RenderMaterial> createRenderMaterial(const DefaultShaderType shaderType) const;
    virtual GeometryBuffer& createGeometryBufferTextured(RefCounted<RenderMaterial> renderMaterial);
    virtual GeometryBuffer& createGeometryBufferColoured(RefCounted<RenderMaterial> renderMaterial);
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
    virtual void beginRendering();
    virtual void endRendering();
    virtual void setDisplaySize(const Sizef& sz);
    virtual const Sizef& getDisplaySize() const;
    virtual const glm::vec2& getDisplayDPI() const;
    virtual uint getMaxTextureSize() const;
    virtual const String& getIdentifierString() const;
    virtual bool isTexCoordSystemFlipped() const;

protected:
    //! default constructor.
    NullRenderer();
	//! common construction things.
	void constructor_impl();
    //! destructor.
    virtual ~NullRenderer();

    //! helper to throw exception if name is already used.
    void throwIfNameExists(const String& name) const;
    //! helper to safely log the creation of a named texture
    static void logTextureCreation(const String& name);
    //! helper to safely log the destruction of a named texture
    static void logTextureDestruction(const String& name);

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
    //! container type used to hold GeometryBuffers we create.
    typedef std::vector<NullGeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::unordered_map<String, NullTexture*> TextureMap;
    //! Container used to track textures.
    TextureMap d_textures;
    //! What the renderer thinks the max texture size is.
    uint d_maxTextureSize;

    //! Shaderwrapper for textured & coloured vertices
    NullShaderWrapper* d_shaderWrapperTextured;
    //! Shaderwrapper for coloured vertices
    NullShaderWrapper* d_shaderWrapperSolid;
};


} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUINullRenderer_h_
