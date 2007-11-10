--
-- DirectX9GUIRenderer premake script
--

cegui_dynamic("DirectX9GUIRenderer")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

-- We need to remove edit-and-continue in VS2003 (7.1) or older versions because 
-- of the __LINE__ expansion macro bug (see http://support.microsoft.com/kb/199057/en)
if target=="vs2002" or target=="vs2003" then
	package.buildflags = { "no-edit-and-continue" }
end

include(pkgdir)

library("dxerr9")
library("d3dx9d")

dependency("CEGUIBase")

define("DIRECTX9_GUIRENDERER_EXPORTS")
