# Omniedge-windows

Windows desktop application for Omniedge. 

>Bring the intranet on the internet

Main repo: https://github.com/omniedgeio/omniedge

![OmniEdge-Windows](https://user-images.githubusercontent.com/93888/171177275-6ec8e7f6-3ea3-4163-8e1a-ef33c5ed0b9f.png)


# Compiler and debugger

`MSVC 2017 32Bit` in `Qt 5.12.12` Creator

# Promoting updates

1. Copy all required files to release/packages/<package-name>/data
2. cd to release folder
3. Re-create repo `repogen.exe --update -p packages -i com.omniedge-inc.omniedge-windows repository`
4. Re-create installer `binarycreator.exe -c config\config.xml -p packages Omniedge.exe`
5. cd to repository folder
6. `git push`

# Contributors

@harri8807 @Tex-tang
