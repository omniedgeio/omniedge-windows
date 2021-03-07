# Omniedge-windows

Windows desktop application for Omniedge. (Made by Qt)

# Compiler and debugger

`MSVC 2017 32Bit` in Qt Creator

Please change build directory to root/debug and root/release

# Promoting updates

1. Copy all required files to release/packages/<package-name>/data
2. cd to release folder
3. Re-create repo `repogen.exe --update -p packages -i com.omniedge-inc.omniedge-windows repository`
4. Re-create installer `binarycreator.exe -c config\config.xml -p packages Omniedge.exe`
5. cd to repository folder
6. `git push`

# Contributors

@harri8807 @Tex-tang
