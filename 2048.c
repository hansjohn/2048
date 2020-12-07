#define BASE_NUM 2   //����
#define MAX_NUM 2048 //��ʤ����
#define BOARDSIZE 4
#define MAXWITHDRAW 7
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
int board[BOARDSIZE][BOARDSIZE] = { 0 };
int score;
typedef struct stack        //ѭ����ջ
{
	int list[MAXWITHDRAW][BOARDSIZE][BOARDSIZE];
	int top;
	int base;
	int score[MAXWITHDRAW];
}Stack;

Stack stack;
void initstack()
{
	memset(stack.list, 0, sizeof(stack.list));
	stack.top = 0;
	stack.base = 0;
	memset(stack.score, 0, sizeof(stack.score));
}

void gotoxy(int x, int y)//����ƶ���(x,y)λ�ã��ƶ�����0,0��ʱ������������һ��һ����
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void hide_cursor() //����CMD���ڹ��
{
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = FALSE;
	cci.dwSize = sizeof(cci);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cci);
}
void cleanBoard() {
	int x, y;
	for (x = 0; x < BOARDSIZE; x++)
	{
		for (y = 0; y < BOARDSIZE; y++)
		{
			board[x][y] = 0;
		}
	}
}
void addRandom()
{
	srand((unsigned)time(NULL));
	int tmp = rand() % (BOARDSIZE * BOARDSIZE);
	while (*(*board + tmp) != 0)   //�������ط��Ѿ���ֵ�˵Ļ�,�ͼ���
	{
		tmp = rand() % (BOARDSIZE * BOARDSIZE);
	}
	int tmpnum = rand() % 2;
	if (tmpnum == 1)
	{
		*(*board + tmp) = BASE_NUM * BASE_NUM;
	}
	else
	{
		*(*board + tmp) = BASE_NUM;
	}
}
void initBoard()
{
	cleanBoard();
	addRandom();
	addRandom();
	score = 0;
}
void drawheadline()
{
	printf("��");
	for (int i = 0; i < BOARDSIZE - 1; i++)
	{
		printf("����������������");
	}
	printf("����������������");
	printf("\n");
}
void drawlastline()
{
	printf("��");
	for (int i = 0; i < BOARDSIZE - 1; i++)
	{
		printf("����������������");
	}
	printf("����������������");
	printf("\n");
}
void drawmidline()
{
	printf("��");
	for (int i = 0; i < BOARDSIZE - 1; i++)
	{
		printf("����������������");
	}
	printf("����������������");
	printf("\n");
}
void drawBoard()
{

	gotoxy(0, 0);
	hide_cursor();
	printf("      ***** Game 2048 ***** \n");
	drawheadline();
	for (int row = 0; row < BOARDSIZE; row++)
	{
		printf("��");
		for (int i = 0; i < BOARDSIZE; i++)
		{
			printf("       ��");
		}
		printf("\n");
		for (int col = 0; col < BOARDSIZE; col++)
		{
			if (board[row][col] != 0)
			{
				printf("�� %4d  ", board[row][col]);
			}
			else
			{
				printf("��       ");
			}
		}
		printf("��\n");
		printf("��");
		for (int i = 0; i < BOARDSIZE; i++)
		{
			printf("       ��");
		}
		printf("\n");
		if (row < BOARDSIZE - 1)
		{
			drawmidline();
		}
	}
	drawlastline();
	printf("\n\tscore:%5d\n\n", score);
	printf("[w] UP [a] LEFT [s] DOWN [d] right\n\n");
	printf("[z] WITHDRAW [q] QUIT\n");
}
char getInput()
{
	char ch = getch();
	while (ch != 'a' && ch != 'A' && ch != 's' && ch != 'S' && ch != 'w' && ch != 'W' && ch != 'd' && ch != 'D' && ch != 'q' && ch != 'Q' && ch != 'z' && ch != 'Z')
	{
		ch = getch();
	}
	return ch;
}


void showResult(int mode)
{
	if (mode == 2)
	{
		printf("YOU WIN\n");
	}
	else if (mode == 1)
	{
		printf("GAME OVER\n");
	}
}

/// <summary>
/// �����ƶ�
/// </summary>
/// 


int slideArray_oneTime(int* board, int lenth)//����0��ʱ�����˵������Ҫ���ƶ���,slideArry����ֱ���˳�
{
	int start = lenth, right = lenth, left = lenth;
	while (right >= 0 && board[right] == 0)//���ҵ���,����һ����Ϊ0�ĵط�
	{
		left--;
		right--;
	}
	if (right < 0)//���ж���0
	{
		return 0;
	}
	else if (right == 0)//������Ƿ�0
	{
		board[start] = board[right];
		board[right] = 0;
		return 1;
	}
	else
	{
		left--;
		while (left >= 0 && board[left] == 0)
		{
			left--;
		}
		if (left < 0)//right��߶���0���������rigt�Ѿ��������
		{
			board[start] = board[right];
			if (right != start)       //right�����׽ڵ�
				board[right] = 0;
			return right != start;
		}
		else
		{
			int tmpl = board[left];   //��ֹleft��right��������ʼ��
			int tmpr = board[right];
			board[right] = 0;
			board[left] = 0;
			if (tmpr != tmpl)
			{
				board[start] = tmpr;
				board[start - 1] = tmpl;
				return (right - left - 1) || (start != right);   //����0˵��left��right����
			}
			else
			{
				board[start] = 2 * tmpr;
				score += 2 * tmpr;
				return 1;
			}
		}
	}
}
int slideArray(int* board, int lenth)
{
	int success = 0;
	for (int len = lenth - 1; len > 0; len--)//���ƶ����߽�,�߽�����Ϊlen,������С
	{
		success += slideArray_oneTime(board, len);
	}
	return success;

}

int  moveRight()
{
	int x;
	int success = 0;
	for (x = 0; x < BOARDSIZE; x++)
	{
		success += slideArray(board[x], BOARDSIZE);
	}
	return success;
}

void rotateBoard()
{
	int i, j, n = BOARDSIZE;
	int tmp;
	for (i = 0; i < n / 2; i++)
	{
		for (j = i; j < n - i - 1; j++)
		{
			tmp = board[i][j];
			board[i][j] = board[j][n - i - 1];
			board[j][n - i - 1] = board[n - i - 1][n - j - 1];
			board[n - i - 1][n - j - 1] = board[n - j - 1][i];
			board[n - j - 1][i] = tmp;
		}
	}
}
int moveLeft()
{
	rotateBoard();
	rotateBoard();
	int success = moveRight();
	rotateBoard();
	rotateBoard();
	return success;
}
int moveUp()
{
	rotateBoard();
	rotateBoard();
	rotateBoard();
	int success = moveRight();
	rotateBoard();
	return success;
}
int moveDown()
{
	rotateBoard();
	int success = moveRight();
	rotateBoard();
	rotateBoard();
	rotateBoard();
	return success;
}


int moveByDirect(char c)
{
	int success = 0;
	if (c == 'a' || c == 'A')
	{
		success = moveLeft();
	}
	else if (c == 's' || c == 'S')
	{
		success = moveDown();
	}
	else if (c == 'd' || c == 'D')
	{
		success = moveRight();
	}
	else if (c == 'w' || c == 'W')
	{
		success = moveUp();
	}
	return success;
}
/// <summary>
/// �ж�ʤ������
/// </summary>
/// <returns>
/// 
/// </returns>

int hasEmpty() {
	int x, y;
	for (x = 0; x < BOARDSIZE; x++)
		for (y = 0; y < BOARDSIZE; y++)
			if (board[x][y] == 0)
				return 1;
	return 0;
}
int findPair()
{
	for (int x = 0; x < BOARDSIZE; x++)
	{
		for (int y = 0; y < BOARDSIZE; y++)
		{
			if (y + 1 < BOARDSIZE && board[x][y] == board[x][y + 1])
				return 1;
			if (x + 1 < BOARDSIZE && board[x][y] == board[x + 1][y])
				return 1;
		}
	}
	return 0;
}
int hasWon()
{
	for (int x = 0; x < BOARDSIZE; x++)
	{
		for (int y = 0; y < BOARDSIZE; y++)
		{
			if (board[x][y] == MAX_NUM)
				return 1;

		}
	}
	return 0;
}
int gameEnded()
{
	if (hasWon()) return 2;
	if (hasEmpty()) return 0;
	if (findPair()) return 0;
	return 1;
}

/// <summary>
/// 
///����
/// </summary>
/// <param name="���Ӻ�"></param>
/// <param name="argv"></param>
/// <returns></returns>


void savetoarry()
{
	stack.top = (stack.top + 1) % MAXWITHDRAW;
	if (stack.top == stack.base)
	{
		stack.base = (stack.base + 1) % MAXWITHDRAW;
	}
	for (int x = 0; x < BOARDSIZE; x++)
		for (int y = 0; y < BOARDSIZE; y++)
			stack.list[stack.top][x][y] = board[x][y];
	stack.score[stack.top] = score;
}
void savetofile()
{
	FILE* fp;
	if ((fp = fopen("gamelog.txt", "wb")) != NULL)
	{

		fwrite(&stack, sizeof(stack), 1, fp);

		fclose(fp);
	}

}
void savegame()
{
	savetoarry();
	savetofile();

}
void withdraw()
{
	if (stack.top == stack.base)
	{
		if (stack.top != 0)
		{
			for (int x = 0; x < BOARDSIZE; x++)
				for (int y = 0; y < BOARDSIZE; y++)
					board[x][y] = stack.list[stack.top][x][y];
			score = stack.score[stack.top];
			stack.top = stack.base = 0;
		}
		else
		{
			return;
		}
	}
	else
	{
		if (!memcmp(stack.list[stack.top], board, sizeof(board)))//���ʱ,˵��ջ�е�һ��Ԫ���ǵ�ǰ��ʾ��,Ҫ��ջ
		{
			if (stack.top == 0)
			{
				stack.top = MAXWITHDRAW - 1;
			}
			else
			{
				stack.top--;
			}
		}
		if (stack.top == 0)
		{

			for (int x = 0; x < BOARDSIZE; x++)
				for (int y = 0; y < BOARDSIZE; y++)
					board[x][y] = stack.list[stack.top][x][y];
			score = stack.score[stack.top];
			stack.top = MAXWITHDRAW - 1;

		}
		else
		{

			for (int x = 0; x < BOARDSIZE; x++)
				for (int y = 0; y < BOARDSIZE; y++)
					board[x][y] = stack.list[stack.top][x][y];
			score = stack.score[stack.top];
			stack.top--;
		}
	}
	drawBoard();

}
void drawmenu()
{
	printf("******************* Game 2048 *******************\n");
	printf("**                                             **\n");
	printf("**               press [r] to start            **\n");
	printf("**                                             **\n");
	printf("**   press [c] to continue with the last game  **\n");
	printf("**                                             **\n");
	printf("**               press [q] to quit             **\n");
	printf("**                                             **\n");
	printf("*************************************************\n");


}
char getmenuchoice()
{
	char ch = getch();
	while (ch != 'R' && ch != 'r' && ch != 'c' && ch != 'C' && ch != 'q' && ch != 'Q')
	{
		ch = getch();
	}
	return ch;
}
void loadgame()
{
	FILE* fr = NULL;
	if ((fr = fopen("gamelog.txt", "rb+")) != NULL)
	{
		fread(&stack, sizeof(stack), 1, fr);
		for (int x = 0; x < BOARDSIZE; x++)
		{
			for (int y = 0; y < BOARDSIZE; y++)
			{
				board[x][y] = stack.list[stack.top][x][y];
			}
		}
		score = stack.score[stack.top];
	}

}
int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));

	while (1)//���˵�
	{
		system("cls");
		drawmenu();
		char menuchoice = getmenuchoice();
		if (menuchoice == 'c' || menuchoice == 'C')
		{
			loadgame();
			system("cls");
			drawBoard();
		}
		else if (menuchoice == 'r' || menuchoice == 'R')
		{
			system("cls");
			initBoard();
			drawBoard();
			initstack();
			savegame();
		}
		else if (menuchoice == 'q' || menuchoice == 'Q')
		{
			break;
		}
		while (1)
		{

			char c = getInput();
			if (c == 'q' || c == 'Q')
			{
				savegame();
				break;
			}
			else if (c == 'z' || c == 'Z')
			{
				withdraw();
				continue;
			}

			if (moveByDirect(c))
			{
				addRandom();
				drawBoard();
				savegame();
			}
			int mode;
			if (mode = gameEnded())
			{
				showResult(mode);
				break;
			}
		}
	}
	return 0;
}