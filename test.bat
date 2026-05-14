@echo off
g++ -DLOCAL -std=c++20 -Wall -Wextra -Wshadow -Wconversion -Wno-cpp testing.cpp -o testing.exe
if errorlevel 1 goto end

testing.exe

:end
pause