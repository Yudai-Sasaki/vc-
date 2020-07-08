#include"Pacman.h"

//�w�肵�����W�ɂ���L�����N�^�[�̔ԍ���Ԃ�(�L�����N�^�[�����݂��Ȃ�������-1��Ԃ�)
int getCharacterIndex(int x, int y, Character characters[]) {
	for (int i = 0; i < CHARACTER_NUM; i++) {
		if ((characters[i].x == x) && (characters[i].y == y)) {
			return i;
		}
	}
	return -1;
}

//�}�b�v�S�̂ɑ��݂���a�̐���Ԃ�
int getFeedNum(int cells[HEIGHT][WIDTH]) {
	int feedNum = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			switch (cells[y][x]) {
			case CELL_FEED:
			case CELL_POWER:
				feedNum++;
				break;
			}
		}
	}
	return feedNum;
}

//gameOver�̂Ƃ��̏���
void gameOver() {
	for (int i = 0; i < 5; i++) {
		flash();
	}
	erase();
	int  x, y, w, h;
	getmaxyx(stdscr, h, w);
	y = h / 2;
	x = (w - 12) / 2;
	move(y, x);
	attrset(COLOR_PAIR(2));
	printw("GAME OVER...");
	beep();
	refresh();
	getch();
	exit(0);
}

//gameClear�̂Ƃ��̏���
void gameClear() {
	erase();
	int  x, y, w, h;
	getmaxyx(stdscr, h, w);
	y = h / 2;
	x = (w - 9) / 2;
	move(y, x);
	attrset(COLOR_PAIR(1));
	printw("CLEAR!!!");
	beep();
	refresh();
	getch();
	exit(0);
}

