#pragma once

#include <Windows.h>
#include"Common.h"

//プロトタイプ宣言
int getCharacterIndex(int x, int y, Character characters[]);
int getFeedNum(int cells[HEIGHT][WIDTH]);
void gameOver();
void gameClear();
