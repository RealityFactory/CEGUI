--
-- CEGUI premake configuration script
--

-- comment this to disable debug suffixes for dynamic module dlls
-- if you want to use another suffix, just change the string :)
-- all the debug cegui libraries are built with this suffix
DEBUG_DLL_SUFFIX = "_d"

-- SDK / dependency paths
-- { base, include_suffix, library_suffix }
-- base can be absolute or relative to the root cegui_mk2 dir
IRRLICHT_PATHS = { "irrlicht-1.4", "include", "lib/Win32-visualstudio" }
OGRE_PATHS = { "C:/OgreSDK", "include", "lib" }
OIS_PATHS = { "C:/OgreSDK", "include/OIS", "lib" }

--- Irrlicht SDK Version
--- 12 is 1.2 (or before?).  13 is 1.3 or 1.3.x, and 14 is 1.4 (and above?)
CEGUI_IRR_SDK_VERSION = 14

--- OIS API version to be used in the Ogre samples base app.
--- true: use older numKeyboards / numMice
--- false: use getNumberOfDevices
CEGUI_OLD_OIS_API = true

--- Lua version
--- 51 is 5.1 (and above?) From 0.7.0 onwards, lua 5.0 is no longer supported.
CEGUI_LUA_VER = 51

-------------
-- Renderers
-- this controls which renderer modules are built
OPENGL_RENDERER = true
DIRECT3D9_RENDERER = true
DIRECT3D10_RENDERER = false
IRRLICHT_RENDERER = false
OGRE_RENDERER = false

----------------
-- Image Codecs
-- this controls which image codecs are built
TGA_IMAGE_CODEC = true
SILLY_IMAGE_CODEC = true
DEVIL_IMAGE_CODEC = true
FREEIMAGE_IMAGE_CODEC = true
CORONA_IMAGE_CODEC = true

-- this setting selects the default image codec module
-- can be either "tga", "silly", "devil", "freeimage" or "corona"
-- SILLY was written for CEGUI
DEFAULT_IMAGE_CODEC = "silly"

---------------
-- Window Renderers
-- controls window renderers built
FALAGARD_WR = true

-- default WR
-- available: falagard
DEFAULT_WINDOW_RENDERER = "falagard"




---------------
-- XML parsers
-- this controls which xml parser modules are built
EXPAT_PARSER = true
XERCES_PARSER = false
TINYXML_PARSER = false
LIBXML_PARSER = false

-- this selects the default XML parser module
-- can be either "expat", "xerces", "tinyxml" or "libxml"
DEFAULT_XML_PARSER = "expat"


-------
-- Lua
-- this controls whether CEGUILua is enabled
LUA_SCRIPT_MODULE = true

-- disable this for a smaller and faster, but less safe Lua module
-- only affects Release builds. Debug and ReleaseWithSymbols always
-- enable this
LUA_SCRIPT_MODULE_SAFE = false

-- enable this to build the bundled tolua++ as a static library
TOLUA_STATIC = false


-----------
-- Samples
-- remember you have to edit CEGUISamplesConfig.h as well this just controls
-- dependencies etc. if the renderer is disabled this has no effect
SAMPLES_GL = true
SAMPLES_DX9 = true
SAMPLES_DX10 = false
SAMPLES_IRRLICHT = false
SAMPLES_OGRE = false

-- this setting controls if the samples should be included in the same
-- solution as the core libraries. If this setting is disabled you can
-- still generate a seperate solution for the samples
--
-- due to a missing feature in premake enabling this will cause the
-- output files to be placed in cegui_mk2/bin and not cegui_mk2/Samples/bin
--
SAMPLES_INCLUDED = false
