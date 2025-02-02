# statically-linked
- libfmt - Static libs built from source [*[commit](https://github.com/fmtlib/fmt/tree/873670ba3f9e7bc77ec2c1c94b04f1f8bef77e9f)*]
- Lua [*5.4.2 [binaries](https://sourceforge.net/projects/luabinaries/) with includes*]
- FreeType [*[binaries](https://github.com/ubawurinna/freetype-windows-binaries/releases/tag/v2.13.3) with includes*]

Download the above dependencies and place them in this folder.
Visual Studio is configured for:
- "fmt.lib"
- "freetype.lib"
- "lua54.lib"

Make sure your static libs' names match the list above to avoid errors.
