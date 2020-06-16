#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <conio.h>
#define BOARD 15



void getLine(int who, int x, int y, int dir, int* Line);
void getHalfLine(int who, int x, int y, int dx, int dy, int* sum);
void isWall(int who, int opponent, int* Line, int* type);
void renderAqizi(int X, int Y);
void AIRound();
void playerRound();
void getScoresList();
int evaluatePointDeScore(int who, int x, int y);
void evaluate2Opti(int x, int y, int comboOfPoint, int* totalScore);
void isWall(int who, int opponent, int* Line, int* type);
int evaluate1First(int who, int opponent, int* Line, int* comboOfPoint);
void AISelectPosition(int* idealX, int* idealY);
void isWin(int who, int x, int y);
void renderEndingInfo(int who);

//ȫ�ֱ���-------------------------------------------------------
//������ǰ�������̸���λ�õĵ÷� ����aiѡ����߷ֵ�λ��ȥ��
int scoreList[BOARD][BOARD] = { 0 };

int board[BOARD][BOARD] = { 0 };
enum { YOU = 1, YOUXIA, XIA, ZUOXIA };

//ʤ����־  1����ʤ�� 2����ʤ��
int winFlag = 0;

//λ���ϵĳ�ʼ��
int positionScore[BOARD][BOARD] = {
	{-15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15},
	{-15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -15},
	{-15, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, -15},
	{-15, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, -15},
	{-15, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, -15},
	{-15, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, -15},
	{-15, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, -15},
	{-15, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, -15},
	{-15, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, -15},
	{-15, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, -15},
	{-15, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, -15},
	{-15, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, -15},
	{-15, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, -15},
	{-15, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -15},
	{-15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15, -15} };
//---------------------------------------------------------------


void renderTheBoard() {
	setlinecolor(RED);
	setfillcolor(RGB(255, 205, 150));

	//�������̵�ɫ
	solidrectangle(-20, -20, 720, 720);

	//���Ʊ߿�
	for (int i = 1; i < 3; i++) {
		rectangle(-i, -i, 700 + i, 700 + i);
	}
	//��������
	for (int x = 0, y = 0, i = 0; i < 15; i++) {
		line(x, y, x, y + 700);
		x += 50;
	}
	//���ƺ���
	for (int x = 0, y = 0, i = 0; i < 15; i++) {
		line(x, y, x + 700, y);
		y += 50;
	}
	//���ĵ���
	setfillcolor(BLACK);
	fillcircle(350, 350, 5);

}

void renderTheBase() {
	setfillcolor(RGB(250, 240, 230));
	solidrectangle(-50, -50, 1100, 750);

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	settextstyle(30, 0, _T("����"));
	TCHAR s1[] = _T("���ִ������");
	TCHAR s2[] = _T("����ִ�׺���");
	outtextxy(800, 50, s1);
	outtextxy(800, 90, s2);
}

void getTheWindow() {
	initgraph(1150, 800);
	//�����µ�ԭ��
	setorigin(50, 50);
	renderTheBase();
	renderTheBoard();
}

//��ʼ����
void battleStart() {
	int nowRound = 1;
	while (nowRound) {
		//printf("--%d\n", nowRound);

		switch (nowRound % 2) {
		case 0://AI��
			AIRound();
			nowRound++;
			break;
		case 1://�����
			playerRound();
			nowRound++;
			break;
		}

		//��ʤ-----------
		if (winFlag != 0) {
			renderEndingInfo(winFlag);
			return;
		}
	}
}

void renderEndingInfo(int who) {
	settextcolor(RGB(255, 106, 106));
	settextstyle(40, 0, _T("����"));
	TCHAR win1[] = _T("��һ�ʤ��");
	TCHAR win2[] = _T("���Ի�ʤ��");
	switch (who) {
	case 1:
		outtextxy(800, 300, win1);
		break;
	case 2:
		outtextxy(800, 300, win2);
		break;
	}
}

//�������
void playerRound() {
	MOUSEMSG mouse;
	FlushMouseMsgBuffer();

	//�������Ӱ����� ����50����Ӧ�����±�
	//ע�� �����豸����ԭ�㲻�ɸ��� �������Ҫ-50
	int X = 0, Y = 0;
	while (true) {
		while (MouseHit()) {
			mouse = GetMouseMsg();
			if (mouse.uMsg == WM_LBUTTONDOWN) {
				//�˴���ʵ������ȡ�������� ����Ϊԭ��任�������
				int mouseX = mouse.x - 50;
				int mouseY = mouse.y - 50;
				//�ж�����Ƿ���������
				if ((mouseX >= 0 && mouseX <= 700) && (mouseY >= 0 && mouseY <= 700)) {
					//�ж�����Ƿ��������ж�������
					if ((mouseX % 50 > 10 && mouseX % 50 < 40) || (mouseY % 50 > 10 && mouseY % 50 < 40)) {
						break; //��겻���ж�������
					}
					//��ȷ��λ��ĳһ��������
					else {
						if (mouseX % 50 <= 10) {
							X = 50 * (mouseX / 50);
						}
						else if (mouseX % 50 >= 40) {
							X = 50 * ((mouseX / 50) + 1);
						}

						if (mouseY % 50 <= 10) {
							Y = 50 * (mouseY / 50);
						}
						else if (mouseY % 50 >= 40) {
							Y = 50 * ((mouseY / 50) + 1);
						}


						//����������

						if (board[Y / 50][X / 50] == 0) {
							board[Y / 50][X / 50] = 1; //������ҵĺ���
							renderAqizi(X, Y); //���Ƴ��Ǹ�λ���ϵ�����
							isWin(1, X / 50, Y / 50);
							return;
						}
						//��ʱ�Խ���cpuʹ��
						Sleep(20);
					}
				}
			}
		}
	}
}

//ʤ���ж�
void isWin(int who, int x, int y) {

	int theLine[11] = { 0 };
	for (int dir = 1; dir <= 4; dir++) {
		getLine(who, x, y, dir, theLine);

		int count = 1;  //��ΪLine[5]�ض���һ�� ����count��1��ʼ
		for (int i = 6; i <= 10; i++) {
			if (theLine[i] == who) {
				count++;
			}
			else {
				break;
			}
		}
		for (int i = 4; i >= 0; i--) {
			if (theLine[i] == who) {
				count++;
			}
			else {
				break;
			}
		}
		if (count >= 5) {
			winFlag = who;
			break;
		}
	}
}

//------------------------------------------------------AI����
//AI����
void AIRound() {
	int idealX = 0, idealY = 0;
	getScoresList();
	AISelectPosition(&idealX, &idealY);
	board[idealY][idealX] = 2; //����AI�İ���
	renderAqizi((idealX) * 50, (idealY) * 50); //���Ƴ��Ǹ�λ���ϵ�����
	isWin(2, idealX, idealY);
}



void AISelectPosition(int* idealX, int* idealY) {
	int maxScore = -99999;
	for (int i = 0; i < BOARD; i++)
	{
		for (int j = 0; j < BOARD; j++) {
			if (board[i][j] == 0) {
				if (scoreList[i][j] > maxScore) {
					maxScore = scoreList[i][j];
					*idealX = j;
					*idealY = i;
				}
			}
		}
	}
}



//------------------�����жϲ���----------------------------------------

void getHalfLine(int who, int x, int y, int dx, int dy, int* sum) {
	int isPositive = 1;
	if (dy == -1 || (dy == 0 && dx == -1)) {
		isPositive = 0;
	}
	for (int i = 0; i < 5; i++) {
		x += dx;
		y += dy;
		//Խ�紦��
		if (x < 0 || x > BOARD - 1 || y < 0 || y > BOARD - 1) {
			sum[i] = 3 - who;  //��Ϊ���ֵ���ɫ
		}
		else {
			sum[i] = board[y][x];
		}
	}
	//�ж��Ƿ�����
	if (isPositive == 0) {
		int sum2[5];
		for (int i = 0; i < 5; i++) {
			sum2[i] = sum[i];
		}
		for (int i = 0; i < 5; i++) {
			sum[i] = sum2[4 - i];
		}
	}
}

// who:1��2��
//�������ܣ�����һ������ �� ���� ����һ�������ȥ �����鴦��Ϊ��һ�������ϵ�����
void getLine(int who, int x, int y, int dir, int* Line) {
	int negtive[5], positive[5];
	int dx, dy;
	switch (dir) {
	case YOU:
		dx = 1;
		dy = 0;
		break;
	case YOUXIA:
		dx = 1;
		dy = 1;
		break;
	case XIA:
		dx = 0;
		dy = 1;
		break;
	case ZUOXIA:
		dx = -1;
		dy = 1;
		break;
	}
	getHalfLine(who, x, y, dx * (-1), dy * (-1), negtive);
	getHalfLine(who, x, y, dx, dy, positive);

	for (int i = 0; i < 5; i++) {
		Line[i] = negtive[i];
	}
	Line[5] = who;
	for (int i = 0; i < 5; i++) {
		Line[i + 6] = positive[i];
	}
}
////////////////////////////////////////////////////////////////////////////////////
enum {
	FIVE = 50000,
	ALIVE4 = 10000,
	ALIVE3 = 2000,
	ALIVE2 = 80,
	RUSH4 = 1600,
	SLEEP3 = 150,
	SLEEP2 = 40,
	DEAD1 = -1,
	SLEEP1 = 5,
	NONE = 0,
	DEAD2 = -2,
	DEAD4 = -10,
	DEAD3 = -5,

};



int evaluatePointDeScore(int who, int x, int y) {
	int totalScore = 0;
	int score = 0;
	//��ÿ����� combo ,��ʮλ��λǧλ����������ʾ�����ĵĸ��� �����Ż���������
	//����0200��ʾ˫��
	int comboOfPoint = 0;
	int Line[11];
	// 4������ֱ��� �ٽ�������
	for (int dir = 1; dir <= 4; dir++) {
		getLine(who, x, y, dir, Line);  //�õ�Line��
		score = evaluate1First(who, 3 - who, Line, &comboOfPoint);
		totalScore += score;
	}
	evaluate2Opti(x, y, comboOfPoint, &totalScore);  //���ܵ÷ֽ��в���
	return totalScore;
}

//�ж�ĳһLine�ϵ����β�������ֺ���-----------------------
int evaluate1First(int who, int opponent, int* Line, int* comboOfPoint) {
	int score = 0;
	int type[2];                        //[0]��count [1]��flag
	isWall(who, opponent, Line, type);  //�õ�type
	switch (type[0]) {
	case 1:  // 1������
		switch (type[1]) {
		case -2:  // DEAD
			score = DEAD1;

			break;
		case 0:
			score = SLEEP1;
			break;
		case 2:  // ALIVE
			score = NONE;
			break;
		}
		break;
	case 2:  // 2������
		switch (type[1]) {
		case -2:  // DEAD
			score = DEAD2;
			break;
		case 0:
			score = SLEEP2;
			break;
		case 2:  // ALIVE
			score = ALIVE2;
			*comboOfPoint += 35;
			break;
		}
		break;
	case 3:  // 3������
		switch (type[1]) {
		case -2:  // DEAD
			score = DEAD3;
			break;
		case 0:
			score = SLEEP3;
			*comboOfPoint += 50;
			break;
		case 2:  // ALIVE
			score = ALIVE3;
			*comboOfPoint += 130;
			break;
		}
		break;
	case 4:  // 4������
		switch (type[1]) {
		case -2:  // DEAD
			score = DEAD4;
			break;
		case 0:
			score = RUSH4;
			*comboOfPoint += 1200;
			break;
		case 2:  // ALIVE
			score = ALIVE4;
			break;
		}
		break;
	case 5:  // FIVE
		score = FIVE;
		break;

		//����Ϊ��Ӧ�����������
	case 6:
		score = FIVE;
		break;
	case 7:
		score = FIVE;
		break;
	case 8:
		score = FIVE;
		break;
	}
	return score;
}

//�ж������������������ж��ٸ�
//������ɫ���Ӽ�ֹͣ���� ���-1
//�����߶�����ɫֹͣ ��flag���Ϊ-2
//������λҲֹͣ ���+1
//��ô���flagֵֻ��Ϊ-2��0��2 ��Ӧ ������/�壬��
//���ڽ�һ���ж�
void isWall(int who, int opponent, int* Line, int* type) {
	int count = 1;  //��ΪLine[5]�ض���һ�� ����count��1��ʼ
	int flag = 0;
	for (int i = 6; i <= 10; i++) {
		if (Line[i] == who) {
			count++;
		}
		else if (Line[i] == opponent) {
			flag -= 1;
			break;
		}
		else {
			flag += 1;
			break;
		}
	}
	for (int i = 4; i >= 0; i--) {
		if (Line[i] == who) {
			count++;
		}
		else if (Line[i] == opponent) {
			flag -= 1;
			break;
		}
		else {
			flag += 1;
			break;
		}
	}
	type[0] = count;
	type[1] = flag;
}  //������ ��ʵ�жϵ���λ�� ����Ӧ���ټ����ж���1�����ӵ����

//��ֵ�Ż�����
void evaluate2Opti(int x, int y, int comboOfPoint, int* totalScore) {
	// ���ݴ��������е�λ�ø���λ�÷�
	*totalScore += positionScore[y][x];
	// ��˫�� ˫���� ��ô�������Ĳ�������
	if (comboOfPoint > 2000) {
		*totalScore = (*totalScore) * 2;
	}
	else if (comboOfPoint < 1000 && comboOfPoint > 200) {
		*totalScore = (*totalScore) * 2.5;
	}
	else if (comboOfPoint < 100 && comboOfPoint > 60) {
		*totalScore = (*totalScore) * 1.5;
	}
}


//-------------------------------ĳһ��������ж� ��� �Ż��÷ֲ���  ����----------------------------------------

//Ϊ��ǰ�������̸������µ�λ�ô�� ����aiѡ����߷ֵ�λ��ȥ��
void getScoresList() {
	for (int i = 0; i < BOARD; i++) {
		for (int j = 0; j < BOARD; j++) {
			if (board[i][j] == 0) {
				//������+���ط� �����ۺϵ÷�
				scoreList[i][j] = evaluatePointDeScore(2, j, i) + evaluatePointDeScore(1, j, i) * 1;
			}
		}
	}
}

//--------------------------------------------------------------------------

//���ֻ��ƺ���-----------------------------------------

//�������� �뾶20  ����x,y  0�� 2���� 1����
void renderAqizi(int X, int Y) {
	int type = board[Y / 50][X / 50];
	setlinecolor(BLACK);
	switch (type) {
	case 0:
		break;
	case 2:
		setfillcolor(WHITE);
		break;
	case 1:
		setfillcolor(BLACK);
		break;
	}
	fillcircle(X, Y, 20);
}


int main(void) {
	getTheWindow();

	battleStart();

	_getch();
	closegraph();
	return 0;
}