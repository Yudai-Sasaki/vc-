#pragma once

#include<Windows.h>
#include<time.h>
#include"Common.h"

//�v���g�^�C�v�錾
void getGurrentDirectory(char* currentDirectory);
bool readChar(const char* section, const char* keyword, const char* defaultValue, char* returnValue, const char* filePath);
void selectMapData(const char* fileName, char mapData[HEIGHT][WIDTH]);
void getMapData(const char* fileName, char mapData[HEIGHT][WIDTH]);
