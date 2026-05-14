@echo off
g++ -DLOCAL -std=c++20 -Wall -Wextra -Wshadow -Wconversion -Wno-cpp main.cpp -o main.exe
if errorlevel 1 goto end

main.exe

:end
pause