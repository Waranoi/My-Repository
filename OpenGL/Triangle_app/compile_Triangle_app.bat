@echo off
echo Compiling to Application\Triangle_app.exe...
call ..\Window\MinGW\set_distro_paths.bat
g++ Triangle_app.cpp ..\Window\glad\src\glad.c -I..\Window\glfw-3.2.1.bin.WIN64\include -I..\Window\glad\include -L..\Window\glfw-3.2.1.bin.WIN64\lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32 -o Application\Triangle_app.exe
echo Finished
pause