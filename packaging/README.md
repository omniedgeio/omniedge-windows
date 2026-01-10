Packaging script (pack_win.ps1) creates a Windows ZIP bundle containing the built executable, translations, Qt DLLs, and plugins. It places the ZIP at packaging/output/omniedge-windows-setup.zip. The GitHub Actions workflow runs this script after building and uploads the resulting ZIP as an artifact for distribution. Local developers can run the script to produce a ZIP for manual distribution.

Usage:
- Ensure you have a Windows-compatible Qt 6.x toolchain installed or use CI.
- Build the project with qmake OmniEdge.pro and nmake.
- Run packaging/pack_win.ps1 to produce packaging/output/omniedge-windows-setup.zip.
- Distribute the ZIP to testers.

Note:
- This ZIP bundles the app executable, translations, Qt DLLs, and necessary plugins. It is a zip-based distribution, not an installer.
- For production distribution, consider adding an NSIS/Inno installer in a future patch to sign and create an installer.
