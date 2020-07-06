#include<conio.h>
#include<stdio.h>
#include<locale.h>
#include<time.h>
#include"Common.h"
#include"Setting.h"
#include"Pacman.h"


Character characters[CHARACTER_NUM]; //パックマンと敵4体を定義する構造体
int cells[HEIGHT][WIDTH];			 //マップのそれぞれの座標に何があるのかという情報を保持

int main() {

	//各マップデータの記号
	char cellChar[CELL_NUM][3] = {
		"　",//CELL_NULL
		"■",//CELL_WALL
		"・",//CELL_FEED
		"▲" //CELL_POWER
	};

	//敵が動く方向ベクトル
	int directions[4][2]{
		{0,-1}, //上
		{-1,0}, //左
		{0,1},  //下
		{1,0},  //右
	};

	// 端末制御の開始
	if (initscr() == NULL) {
		return 1;
	}

	// 色の準備
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);

	//キーの有効化
	keypad(stdscr, TRUE);

	//マップデータ取得
	char mapData[HEIGHT][WIDTH];
	const char* fileName = NULL;
	selectMapData(fileName, mapData);	

	//マップデータの読み込み
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			char tmp = mapData[y][x];
			int cell = CELL_NULL;	//デフォルトで空白セルを設定
			switch (tmp) {
			case '#':cell = CELL_WALL;	break;
			case '%':cell = CELL_FEED;	break;
			case '!':cell = CELL_POWER;	break;
			default: {	
				//パックマンまたは敵のときにcharacterIndexにキャラ番号を代入
				int characterIndex = -1;
				switch (tmp) {
				case'p': characterIndex = CHARACTER_PACMAN;	      break;
				case'r': characterIndex = CHARACTER_ENEMY_RED;	  break;
				case'c': characterIndex = CHARACTER_ENEMY_CYAN;   break;
				case'm': characterIndex = CHARACTER_ENEMY_MAGENTA;break;
				case'g': characterIndex = CHARACTER_ENEMY_GREEN;  break;
				}
				//各キャラの構造体に座標を設定
				if (characterIndex >= 0) {	
					characters[characterIndex].x = x;
					characters[characterIndex].y = y;
				}
			}
			break;
			}
			//各座標のセル情報を書き込む
			cells[y][x] = cell;
		}
	}

	//餌の最大値
	int maxFeedNum = getFeedNum(cells);

	//無敵時間
	int invincibleTime = 0;

	while (1) {
		srand((unsigned int)time(NULL));

	//マップの表示
		erase();
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				int characterIndex = getCharacterIndex(x, y, characters);
				//キャラクターのとき
				if (characterIndex >= 0) {
					//パックマンの描画
					if ((characterIndex == CHARACTER_PACMAN)) {
						attrset(COLOR_PAIR(CHARACTER_PACMAN + 1));
						printw("Ω");
					}
					//敵がいじけ状態のとき◎になる
					else if ((characterIndex > CHARACTER_PACMAN) && (invincibleTime > 0)) {
						if (invincibleTime > 10) {
							//全ての敵が白色に
							attrset(COLOR_PAIR(0));
							printw("◎");
						}
						else {
							//いじけ状態が終わりかけると敵の色が戻る
							attrset(COLOR_PAIR(characterIndex + 1));
							printw("◎");
						}
					}
					//いじけ状態以外のとき
					else {
						attrset(COLOR_PAIR(characterIndex + 1));
						printw("●");
					}
				}
				//壁のとき
				else if (cells[y][x] == CELL_WALL) {
					attrset(COLOR_PAIR(6));
					printw(cellChar[cells[y][x]]);
				}
				//餌のとき
				else if (cells[y][x] == CELL_FEED) {
					attrset(COLOR_PAIR(1));
					printw(cellChar[cells[y][x]]);
				}
				//パワー餌or空白セルのとき
				else {
					attrset(COLOR_PAIR(0));
					printw(cellChar[cells[y][x]]);
				}
			}
			printw("\n");
			refresh();
		}


	//パックマンの操作
		int x = characters[CHARACTER_PACMAN].x;
		int y = characters[CHARACTER_PACMAN].y;

		switch (getch()) {
		case KEY_UP:y--;   break;
		case KEY_DOWN:y++; break;
		case KEY_LEFT:x--; break;
		case KEY_RIGHT:x++;break;
		}

		//ワープを実現
		x = (WIDTH + x) % WIDTH;
		y = (HEIGHT + y) % HEIGHT;
		int characterIndex = getCharacterIndex(x, y, characters);
		
		//GameOver判定
		if (characterIndex > CHARACTER_PACMAN) {
			//無敵状態のときに敵にぶつかると敵を初期位置へ
			if (invincibleTime > 0) {
				characters[characterIndex].x = 15;
				characters[characterIndex].y = 6;
			}
			else {
				gameOver();
			}
		}

		//移動したい位置が壁のときは進まない
		if (cells[y][x] == CELL_WALL) {

		}
		//餌を食べる処理
		else {
			//パワー餌判定
			if (cells[y][x] == CELL_POWER) {
				invincibleTime += 45;
			}
			//通った場所を空白セルにする
			cells[y][x] = CELL_NULL;
			
			//Clear判定
			int feedNum = getFeedNum(cells);	//現在マップ上にある餌の数を取得
			int characterIndex = -1;
			if (feedNum == 0) {
				gameClear();
			}

			//敵の出現処理
			else if (feedNum == maxFeedNum * 4 / 5) {
				characterIndex = CHARACTER_ENEMY_MAGENTA;
			}
			else if (feedNum == maxFeedNum * 3 / 5) {
				characterIndex = CHARACTER_ENEMY_GREEN;
			}
			else if (feedNum == maxFeedNum * 2 / 5) {
				characterIndex = CHARACTER_ENEMY_CYAN;
			}
			if (characterIndex >= 0) {
				characters[characterIndex].x = 15;
				characters[characterIndex].y = 6;
			}
			characters[CHARACTER_PACMAN].x = x;
			characters[CHARACTER_PACMAN].y = y;
		}

	//敵の移動
		for (int i = CHARACTER_PACMAN + 1; i < CHARACTER_NUM; i++) {
			//現在進んでいる方向に進んだときの座標取得
			int x = characters[i].x + directions[characters[i].direction][0];
			int y = characters[i].y + directions[characters[i].direction][1];
			//ワープを実現
			x = (WIDTH + x) % WIDTH;
			y = (HEIGHT + y) % HEIGHT;

			//曲がれる道があるときは方向を設定しなおす
			if ((cells[y][x] != CELL_WALL) && (characterIndex < CHARACTER_PACMAN)) {
				//現在の方向が上下のときは左右が曲がれるかを調べる
				if (characters[i].direction == 0 || characters[i].direction == 2) {
					if ((cells[characters[i].y][characters[i].x - 1] != CELL_WALL) ||
						(cells[characters[i].y][characters[i].x + 1] != CELL_WALL)) {
						int oppositeDirection = (characters[i].direction + 2) % 4; //進行方向と逆の方向ベクトル
						characters[i].direction = rand() % 4;	//方向をランダムに生成
						//生成した方向が条件を満たすまで方向を設定しなおす
						while (cells[characters[i].y + directions[characters[i].direction][1]]	//壁のある方に進んでいないか
							[characters[i].x + directions[characters[i].direction][0]] == CELL_WALL ||
							(oppositeDirection == characters[i].direction)) {	//後ろに戻っていないか
							characters[i].direction = rand() % 4;
						}
						//新しい方向に移動
						characters[i].x = (WIDTH + characters[i].x + directions[characters[i].direction][0]) % WIDTH;
						characters[i].y = (HEIGHT + characters[i].y + directions[characters[i].direction][1]) % HEIGHT;
					}
					else {
						//曲がれない場合は直進
						characters[i].x = x;
						characters[i].y = y;
					}
				}
				//現在の方向が左右のときは上下が曲がれるかを調べる
				else {
					if ((cells[characters[i].y - 1][characters[i].x] != CELL_WALL) ||
						(cells[characters[i].y + 1][characters[i].x] != CELL_WALL)) {
						int oppositeDirection = (characters[i].direction + 2) % 4; //進行方向と逆の方向ベクトル
						characters[i].direction = rand() % 4;	//方向をランダムに生成
						//生成した方向が条件を満たすまで方向を設定しなおす
						while (cells[characters[i].y + directions[characters[i].direction][1]]//壁のある方に進んでいないか
							[characters[i].x + directions[characters[i].direction][0]] == CELL_WALL ||
							(oppositeDirection == characters[i].direction)) {	   //後ろに戻っていないか
							characters[i].direction = rand() % 4;
						}
						//新しい方向に移動
						characters[i].x = (WIDTH + characters[i].x + directions[characters[i].direction][0]) % WIDTH;
						characters[i].y = (HEIGHT + characters[i].y + directions[characters[i].direction][1]) % HEIGHT;
					}
					else {
						//曲がれない場合は直進
						characters[i].x = x;
						characters[i].y = y;
					}
				}
			}

			//壁にぶつかったとき方向を変える
			else if (cells[y][x] == CELL_WALL) {
				int previousDirection = characters[i].direction;           //進行方向の方向ベクトル
				int oppositeDirection = (characters[i].direction + 2) % 4; //進行方向と逆の方向ベクトル
				//条件を満たすまで方向を設定しなおす
				while ((previousDirection == characters[i].direction) ||   //進行方向に進もうとしてその場で立ち止まっていないか
					(oppositeDirection == characters[i].direction) ||      //後ろに戻っていないか
					(cells[characters[i].y + directions[characters[i].direction][1]]//壁のある方に進んでいないか
						[characters[i].x + directions[characters[i].direction][0]] == CELL_WALL)) {
					characters[i].direction = rand() % 4;
				}
				//新しい方向に移動
				characters[i].x = (WIDTH + characters[i].x + directions[characters[i].direction][0]) % WIDTH;
				characters[i].y = (HEIGHT + characters[i].y + directions[characters[i].direction][1]) % HEIGHT;
			}

			//味方にぶつかったとき方向を変える
			else if (characterIndex > CHARACTER_PACMAN) {
				int previousDirection = characters[i].direction;			//進行方向の方向ベクトル
				while ((previousDirection == characters[i].direction) ||	//進行方向に進もうとしてその場で立ち止まらないようにする
					(cells[characters[i].y + directions[characters[i].direction][1]]//壁のある方に進まないようにする
						[characters[i].x + directions[characters[i].direction][0]] == CELL_WALL)) {
					characters[i].direction = rand() % 4;
				}
				//新しい方向に移動
				characters[i].x = (WIDTH + characters[i].x + directions[characters[i].direction][0]) % WIDTH;
				characters[i].y = (HEIGHT + characters[i].y + directions[characters[i].direction][1]) % HEIGHT;
			}

			//何もない場合は直進
			else {
				characters[i].x = x;
				characters[i].y = y;
			}

			//GameOver判定
			int characterIndex = getCharacterIndex(characters[i].x, characters[i].y, characters);
			if (characterIndex == CHARACTER_PACMAN) {
				//パックマンが無敵状態のときにぶつかると初期位置へ
				if (invincibleTime > 0) {
					characters[i].x = 15;
					characters[i].y = 6;
				}
				else {
					gameOver();
				}
			}
		}

		//無敵時間中は１動作するごとに時間が１減る
		if (invincibleTime > 0) {
			invincibleTime--;
		}
	}

	return 0;
}

