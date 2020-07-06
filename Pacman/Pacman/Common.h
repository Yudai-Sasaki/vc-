#pragma once

#include<curses.h>
#include<stdlib.h>
#include<string.h>


#define WIDTH 32
#define HEIGHT 19
#define BUFFSIZE 1024
#define CHARBUFF 124

//マップデータの種類
enum {
	CELL_NULL,
	CELL_WALL,
	CELL_FEED,
	CELL_POWER,
	CELL_NUM
};

//キャラクターの種類
enum {
	CHARACTER_PACMAN,
	CHARACTER_ENEMY_RED,
	CHARACTER_ENEMY_CYAN,
	CHARACTER_ENEMY_MAGENTA,
	CHARACTER_ENEMY_GREEN,
	CHARACTER_NUM
};

//キャラクターを定義する構造体
struct Character {
	int x;
	int y;
	int direction;
};


