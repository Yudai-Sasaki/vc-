#include"Setting.h"

void getGurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(CHARBUFF, currentDirectory);
}

bool readChar(const char* section, const char* keyword, const char* defaultValue, char* returnValue, const char* filePath) {
	if (GetPrivateProfileString(section, keyword, defaultValue, returnValue, CHARBUFF, filePath) != 0) {
		return true;
	}
	else {
		fprintf(stdout, "%s doesn't contain [%s] %s\n", filePath, section, keyword);
		return false;
	}
}

//�R�̃}�b�v�f�[�^���烉���_���łP���擾����
void selectMapData(const char* fileName, char mapData[HEIGHT][WIDTH]) {
	char currentDirectory[CHARBUFF];
	getGurrentDirectory(currentDirectory);
	char section[CHARBUFF];
	char keyWord[CHARBUFF];
	char settingFile[CHARBUFF];
	char name[CHARBUFF];
	sprintf_s(settingFile, "%s\\Setting.ini", currentDirectory);
	srand((unsigned int)time(NULL));
	sprintf_s(section, "mapData");

	//�����_���Ń}�b�v�f�[�^����I��
	int mapNum = rand() % 3 + 1;
	if (mapNum == 1) {
		sprintf_s(keyWord, "fileName1");
	}
	else if (mapNum == 2) {
		sprintf_s(keyWord, "fileName2");
	}
	else {
		sprintf_s(keyWord, "fileName3");
	}

	//�}�b�v�f�[�^�����擾
	if (readChar(section, keyWord, "none", name, settingFile)) {
		fileName = name;
		//�}�b�v�f�[�^�t�@�C���̓ǂݍ���
		getMapData(fileName, mapData);
	}
	else {
		fprintf(stdout, "%s doesn't contain [%s] %s\n", settingFile, section, keyWord);
	}
}

//�}�b�v�f�[�^�t�@�C����ǂݍ��݁A���̓��e��mapData�Ɋi�[
void getMapData(const char* fileName, char mapData[HEIGHT][WIDTH]) {
	FILE* fp;
	char s[BUFFSIZE];

	errno_t error;
	error = fopen_s(&fp, fileName, "r");

	if (fp == NULL) {
		return;
	}
	if (error != 0) {
		fprintf_s(stderr, "failed to open");
	}
	else {
		int line = 0; //mapData�̗�
		int row = 0;  //mapData�̍s
		char* p1;
		char* ctx;
		while (fgets(s, BUFFSIZE, fp) != NULL) {
			p1 = strtok_s(s, ",", &ctx);
			mapData[row][line] = *p1;
			line++;
			while (p1) {
				p1 = strtok_s(NULL, ",", &ctx);
				if (line < WIDTH) {
					mapData[row][line] = *p1;
					line++;
				}
			}
			row++;
			line = 0;
		}
		fclose(fp);
	}
}

