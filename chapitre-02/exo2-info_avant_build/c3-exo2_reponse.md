============================ Jenga Workspace: JENGA ============================

Location: C:\Users\Brice Baudouin\OneDrive\Bureau\JENGA\JENGA
Entry file: C:\Users\Brice Baudouin\OneDrive\Bureau\JENGA\JENGA\JENGA.jenga
Configurations: Debug, Release
Platforms: Windows
Target OSes: Windows, Android
Target Architectures: x86_64, arm64


Projects
------------------------------------------------------------
Name        Kind          Language   Test   External
====================================================
JengaTest   WindowedApp   C++        No     Yes


Available Toolchains
------------------------------------------------------------
Name                Family   Target OS   Arch     Env  
=======================================================
host-clang          clang    Windows     x86_64   msvc
host-gcc            gcc      Windows     x86_64   mingw
clang-mingw         clang    Windows     x86_64   mingw
mingw               gcc      Windows     x86_64   mingw
clang-cross-linux   clang    Linux       x86_64   gnu


Daemon
------------------------------------------------------------
Status: Not running


Remarque: dans .jenga on précise la version du c++ utilisée qui est C++17 la localisation du projet qui est dans le fichier source tandis que jenga info présice les toolchains disponible, test pour dire s'il s'agit des test unitaire(non) external, ainsi que les daemon. chose que le .jenga ne renseigne pas.
