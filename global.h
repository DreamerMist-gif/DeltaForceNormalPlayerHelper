#pragma once
#pragma warning(disable:4996)

#include <windows.h>
#include <iostream>
#include <process.h>
#include <string>
#include "graphics.h" // Keep quotes as this will be a local file in your repo

void MBX(const char* str);
void MBX(std::string str);

void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);

LPWSTR pCharToLPWSTR(char* old);

void PressKey(UINT Key);
