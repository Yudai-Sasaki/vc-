#pragma once

#include <Windows.h>
#include"Common.h"

//�v���g�^�C�v�錾
int getCharacterIndex(int x, int y, Character characters[]);
int getFeedNum(int cells[HEIGHT][WIDTH]);
void gameOver();
void gameClear();
