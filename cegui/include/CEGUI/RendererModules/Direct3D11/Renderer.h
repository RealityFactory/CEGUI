/***********************************************************************
    filename:   Renderer.h
    created:    Sun, 6th April 2014
    author:     Lukas E Meindl
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
#ifndef _CEGUIDirect3D11Renderer_h_
#define _CEGUIDirect3D11Renderer_h_

#include "../../Renderer.h"
#include "../../Size.h"
#include "../../Vector.h"

#include <glm/glm.hpp>

#include <vector>
#include <map>

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef CEGUIDIRECT3D11RENDERER_EXPORTS
#       define D3D11_GUIRENDERER_API __declspec(dllexport)
#   else
#       define D3D11_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define D3D11_GUIRENDERER_API
#endif

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// D3D forward refs
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;

#include <d3d11.h>

// Start of CEGUI namespace section
namespace CEGUI
{
class Direct3D11GeometryBuffer;
class Direct3D11Texture;
class Direct3D11ShaderWrapper;


//! Renderer implementation using Direct3D 10.
class D3D11_GUIRENDERER_API Direct3D11Renderer : public Renderer
{
public:
    /*!
    \brief
        Convenience function that creates the required objects to initialise the
        CEGUI system.

        This will create and initialise the following objects for you:
        - CEGUI::Direct3D11Renderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \param device
        Pointer to the ID3D11Device interface that is to be used for CEGUI
        rendering operations.

    \param context
        Pointer to the ID3D11DeviceContext interface that is to be used for
        CEGUI rendering operations.

    \param abi
        This must be set to CEGUI_VERSION_ABI

    \return
        Reference to the CEGUI::Direct3D11Renderer object that was created.
    */
    static Direct3D11Renderer& bootstrapSystem(ID3D11Device* device,
                                               ID3D11DeviceContext* context,
                                               const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Convenience function to cleanup the CEGUI system and related objects
        that were created by calling the bootstrapSystem function.

        This function will destroy the following objects for you:
        - CEGUI::System
        - CEGUI::DefaultResourceProvider
        - CEGUI::Direct3D11Renderer

    \note
        If you did not initialise CEGUI by calling the bootstrapSystem function,
        you should \e not call this, but rather delete any objects you created
        manually.
    */
    static void destroySystem();

    /*!
    \brief
        Create an Direct3D11Renderer object.
    */
    static Direct3D11Renderer& create(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
                                      const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Destroy an Direct3D11Renderer object.

    \param renderer
        The Direct3D11Renderer object to be destroyed.
    */
    static void destroy(Direct3D11Renderer& renderer);

	//Returns Direct3D device
	ID3D11Device* getDirect3DDevice(); 
    //Returns Direct3D context
	ID3D11DeviceContext* getDirect3DDeviceContext(); 

    /*!
    \brief
        Helper to set the view projection matrix.

    \param viewProjectionMatrix
        The view projection matrix.
    */
    virtual void setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix);    
    /*!
    \brief
        Helper to return view projection matrix.

    \return
        The view projection matrix.
    */
    const glm::mat4& Direct3D11Renderer::getViewProjectionMatrix();

    /*!
    \brief
        Binds the corresponding D3D11 blend mode.
    */
    void bindBlendMode(BlendMode d_blendMode);

    /*!
    \brief
        Binds the D3D11 RasterizerState corresponding to the scissorEnabled boolean.
    */
    void bindRasterizerState(bool scissorEnabled);

    // Implement interface from Renderer
    RenderTarget& getDefaultRenderTarget();
    RefCounted<RenderMaterial> createRenderMaterial(const DefaultShaderType shaderType) const;
    GeometryBuffer& createGeometryBufferColoured(CEGUI::RefCounted<RenderMaterial> renderMaterial);
    GeometryBuffer& createGeometryBufferTextured(CEGUI::RefCounted<RenderMaterial> renderMaterial);
    void destroyGeometryBuffer(const GeometryBuffer& buffer);
    void destroyAllGeometryBuffers();
    TextureTarget* createTextureTarget();
    void destroyTextureTarget(TextureTarget* target);
    void destroyAllTextureTargets();
    Texture& createTexture(const String& name);
    Texture& createTexture(const String& name,
                           const String& filename,
                           const String& resourceGroup);
    Texture& createTexture(const String& name, const Sizef& size);
    void destroyTexture(Texture& texture);
    void destroyTexture(const String& name);
    void destroyAllTextures();
    Texture& getTexture(const String& name) const;
    bool isTextureDefined(const String& name) const;
    void beginRendering();
    void endRendering();
    void setDisplaySize(const Sizef& sz);
    const Sizef& getDisplaySize() const;
    const Vector2f& getDisplayDPI() const;
    uint getMaxTextureSize() const;
    const String& getIdentifierString() const;
protected:
    //! constructor
    Direct3D11Renderer(ID3D11Device* device,ID3D11DeviceContext* context);

    //! destructor.
    ~Direct3D11Renderer();

    //! Initialises the D3D states
    void initialiseSamplerStates();
    void initialiseDepthStencilState();
    void initialiseRasterizerStates();
    void initialiseBlendStates();

    //! Initialises the ShaderManager and the required D3D11 shaders
    void initialiseShaders();
    //! Initialises the D3D11 ShaderWrapper for textured objects
    void initialiseStandardTexturedShaderWrapper();
    //! Initialises the D3D11 ShaderWrapper for coloured objects
    void initialiseStandardColouredShaderWrapper();
    //! Wrapper of the OpenGL shader we will use for textured geometry
    Direct3D11ShaderWrapper* d_shaderWrapperTextured;
    //! Wrapper of the OpenGL shader we will use for solid geometry
    Direct3D11ShaderWrapper* d_shaderWrapperSolid;

    //! return size of the D3D device viewport.
    Sizef getViewportSize();

    //! helper to throw exception if name is already used.
    void throwIfNameExists(const String& name) const;
    //! helper to safely log the creation of a named texture
    static void logTextureCreation(const String& name);
    //! helper to safely log the destruction of a named texture
    static void logTextureDestruction(const String& name);
    //! String holding the renderer identification text.
    static String d_rendererID;
	//! The D3D device context we're using to create various resources with.
	ID3D11Device* d_device;
	//! The D3D device context we're using to render
	ID3D11DeviceContext* d_deviceContext;
    //! BlendState for regular blending in CEGUI
    ID3D11BlendState* d_blendStateNormal;
    //! BlendState for premultiplied blending in CEGUI
    ID3D11BlendState* d_blendStatePreMultiplied;
    //! The currently set BlendState
    ID3D11BlendState* d_currentBlendState;
    //! The rasterizer state for CEGUI with scissor enabled
    ID3D11RasterizerState* d_rasterizerStateScissorEnabled;
    //! The rasterizer state for CEGUI with scissor enabled
    ID3D11RasterizerState* d_rasterizerStateScissorDisabled;
    //! The current rasterizer state
    ID3D11RasterizerState* d_currentRasterizerState;
    //! The default depth/stencil state
    ID3D11DepthStencilState* d_depthStencilStateDefault;

    //! What the renderer considers to be the current display size.
    Sizef d_displaySize;
    //! What the renderer considers to be the current display DPI resolution.
    Vector2f d_displayDPI;
    //! The default RenderTarget
    RenderTarget* d_defaultTarget;
    //! container type used to hold TextureTargets we create.
    typedef std::vector<TextureTarget*> TextureTargetList;
    //! Container used to track texture targets.
    TextureTargetList d_textureTargets;
    //! container type used to hold GeometryBuffers we create.
    typedef std::vector<Direct3D11GeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::map<String, Direct3D11Texture*, StringFastLessCompare
                     CEGUI_MAP_ALLOC(String, Direct3D11Texture*)> TextureMap;
    //! Container used to track textures.
    TextureMap d_textures;

    //! Variable containing the sampler state for CEGUI textures
    ID3D11SamplerState* d_samplerState;

    //! View projection matrix
    glm::mat4 d_viewProjectionMatrix;
};


} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIDirect3D11Renderer_h_
