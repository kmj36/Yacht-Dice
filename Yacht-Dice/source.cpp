#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#pragma warning(disable : 4996)
#define BONUS 40

int compare(const void* a, const void* b) // �� ���� �ҽ�
{
	return *(int*)a - *(int*)b;
}

typedef struct high_sections
{
	int arr[6];
	char flag[6];
}high;

typedef struct low_sections
{
	int arr[6];
	char flag[6];
}low;

int print_mainmenu(void);
int print_turn(void);
void play(int, high*, low*, high*, low*);
void dice_and_select(int[],int, high*, low*);
void reset_dice(int []);
void print_Dnm_score(int[], int[], high*, low*);
void selects(int[],high*, low*);
void reset_structer(high*, low*, high*, low*);
void result(int, int);
void rule(void);
void using_music(void);

int main(void) // main
{
	system("title Yacht Dice");
	PlaySound(TEXT("bgm.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
	high *p1_H_s = (high*)malloc(sizeof(high)), *p2_H_s = (high*)malloc(sizeof(high));
	low *p1_L_s = (low*)malloc(sizeof(low)) , *p2_L_s = (low*)malloc(sizeof(low));
	int turn, sel=4;

	reset_structer(p1_H_s, p1_L_s, p2_H_s, p2_L_s);
	while (sel != 1)
	{
		sel = print_mainmenu();
		if (sel == 2)
			rule();
		else if (sel == 3)
			using_music();
		else if (sel == 4)
			return 0;
	}
	system("cls");

	turn = print_turn();
	
	if (turn % 2 == 0)
		printf("[�÷��̾� 1] ���� �����մϴ�.\n");
	else
		printf("[�÷��̾� 2] ���� �����մϴ�.\n");

	PlaySound(NULL, 0, 0);

	Sleep(2000);
	printf(" .");
	Sleep(1000);
	printf(" .");
	Sleep(900);
	printf(" .");
	Sleep(300);
	printf(" .");
	Sleep(200);
	printf(" .");
	Sleep(100);
	system("cls");

	PlaySound(TEXT("bgm2.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);

	play(turn, p1_H_s, p1_L_s, p2_H_s, p2_L_s);

	PlaySound(NULL, 0, 0);
	free(p1_H_s);
	free(p1_L_s);
	free(p2_H_s);
	free(p2_L_s);
	return 0;
}

void using_music(void)
{
	system("mode con cols=48 lines=36");
	printf("[MAIN MENU] Kevin MacLeod - Samba Isobel\n");
	printf("[GAME OST] Kevin MacLeod ~ Apero Hour\n");
	system("pause");
}

void rule(void)
{
	system("mode con cols=80 lines=50");
	int i;
	for (i = 0; i < 48; i++)
	{
		if (i == 0)
			printf("[���� ����]\n");
		else if (i == 2)
			printf("1. �ֻ��� 5���� �����ϴ�.\n");
		else if (i == 4)
			printf("2. �� 3�� ���� �� �ְ� ���ϴ� �ֻ����� 1~5���� �����ϰ� ����� �� �ֽ��ϴ�.\n");
		else if (i == 6)
			printf("[0 �ֻ��� �ٽ� ������  9 ���� ����] �� �����մϴ�.\n");
		else if (i == 7)
			printf("*���� ���� �� �ֻ��� �ٽ� �����Ⱑ �Ұ����մϴ�.*\n");
		else if (i == 9)
			printf("3. �ֻ����� 3�� ���Ȱų�[���� ����]��\n");
		else if (i == 10)
			printf("�������� ��� �ݵ�� ����(������)�� ����ؾ� �մϴ�.\n");
		else if (i == 12)
			printf("4. �� 24���� �����ϸ� ������ ������ ���� �������� ���и� ���մϴ�.\n");
		else if (i == 14)
			printf("[����]\n");
		else if (i == 16)
			printf("[���]\n");
		else if (i == 17)
			printf("Ones: �ֻ��� 1 ���� ����.\n");
		else if (i == 18)
			printf("Twos : �ֻ��� 2 ���� ����.\n");
		else if (i == 19)
			printf("Threes : �ֻ��� 3 ���� ����.\n");
		else if (i == 20)
			printf("Fours : �ֻ��� 4 ���� ����.\n");
		else if (i == 21)
			printf("Fives : �ֻ��� 5 ���� ����.\n");
		else if (i == 22)
			printf("Sixes : �ֻ��� 6 ���� ����.\n");
		else if (i == 23)
			printf("[���ʽ�] : ��� �׸��� ������ 63�� �̻��϶�, +40��.\n");
		else if (i == 25)
			printf("[�ϴ�]\n");
		else if (i == 26)
			printf("Choice : �ֻ��� 5 ���� ����.\n");
		else if (i == 27)
			printf("4 of a Kind : 4���� �ֻ����� �����Ҷ�, ������ �ֻ��� �� 4�� ����.\n");
		else if (i == 28)
			printf("Full House : 3��, 2���� �ֻ����� �����Ҷ�, �ֻ��� 5���� ����.\n");
		else if (i == 29)
			printf("Little Straight : �ֻ��� ���� 1, 2, 3, 4, 5 ���� 30��.\n");
		else if (i == 30)
			printf("Big Straight : �ֻ��� ���� 2, 3, 4, 5, 6 ���� 30��.\n");
		else if (i == 31)
			printf("Yacht : 5���� �ֻ����� ��� ���� ���� 50��.\n");
		else
			printf("                                               \n");
	}
	system("pause");
}

void result(int p1, int p2)
{
	FILE* fp = fopen("lasttime.txt", "w");
	time_t T = time(NULL);
	struct tm tm = *localtime(&T);
	if (p1 > p2)
	{
		system("cls");
		printf("[�÷��̾� 1] �¸�!!!");
	}
	else
	{
		system("cls");
		printf("[�÷��̾� 2] �¸�!!!");
	}
	fprintf(fp, "%d-%d-%d %d:%d:%d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	fclose(fp);
	system("pause");
}

void reset_structer(high* p1_H_s, low* p1_L_s, high* p2_H_s, low* p2_L_s)
{
	int i;
	for (i = 0; i < 6; i++)
	{
		p1_H_s->arr[i] = 0;
		p1_H_s->flag[i] = 'O';
		p1_L_s->arr[i] = 0;
		p1_L_s->flag[i] = 'O';
		p2_H_s->arr[i] = 0;
		p2_H_s->flag[i] = 'O';
		p2_L_s->arr[i] = 0;
		p2_L_s->flag[i] = 'O';
	}
}

void selects(int score[], high * hi, low * lo)
{
	int i, j, sel;
	system("cls");
	printf("��������������������������������������������������\n");
	for (i = 0, j = -6; i < 12; i++, j++)
	{
		if (i == 0)
			printf("1.   Ones  : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 1)
			printf("2.   Twos  : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 2)
			printf("3.   Threes : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 3)
			printf("4.   Fours  : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 4)
			printf("5.   Fives  : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 5)
			printf("6.   Sixes  : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 6)
			printf("7.   Choice  : %d [%c]\n", score[i], lo->flag[j]);
		else if (i == 7)
			printf("8.   4 of a Kind  : %d [%c]\n", score[i], lo->flag[j]);
		else if (i == 8)
			printf("9.   Full House  : %d [%c]\n", score[i], lo->flag[j]);
		else if (i == 9)
			printf("10.  L. Straight : %d [%c]\n", score[i], lo->flag[j]);
		else if (i == 10)
			printf("11.  B. Straight : %d [%c]\n", score[i], lo->flag[j]);
		else // (i == 11)
			printf("12.     Yacht    : %d [%c]\n", score[i], lo->flag[j]);
	}
	printf("��������������������������������������������������\n");
	while (1) //���� 1.�÷��� �Ǻ� hi, lo 2. �÷��װ� O �̾����, 3. 1 ~ 12 �̾�� ��
	{
		printf("[���� ����]: ");
		scanf("%d", &sel);
		if (sel < 1 || sel > 12)
		{
			printf("������ ������ϴ�. �ٽ� �����Ͽ��ּ���.\n");
			continue;
		}
		sel -= 1;
		if (sel < 6)
		{
			if (hi->flag[sel] == 'X')
			{
				printf("�ߺ��� �� �����ϴ�. �ٽ� �����Ͽ��ּ���.\n");
				continue;
			}
			hi->arr[sel] = score[sel];
			hi->flag[sel] = 'X';
			return;
		}
		else
		{
			if (lo->flag[sel-6] == 'X')
			{
				printf("�ߺ��� �� �����ϴ�. �ٽ� �����Ͽ��ּ���.\n");
				continue;
			}
			lo->arr[sel - 6] = score[sel];
			lo->flag[sel - 6] = 'X';
			return;
		}
	}
}

void reset_dice(int dice[])
{
	int i;
	for (i = 0; i < 5; i++)
		dice[i] = 0;
}

void print_Dnm_score(int score[], int temp[], high * hi, low * lo)
{
	int cpy[5], i, j;
	for (i = 0; i < 5; i++)
		cpy[i] = temp[i];
	for (i = 0; i < 12; i++) // �ʱ�ȭ
		score[i] = 0;
	qsort(cpy, sizeof(cpy) / sizeof(int), sizeof(int), compare);
	// ���� ����


	for(j=0;j<6;j++) // ��� ���� ���� (0 �̻� ����)
		for (i = 0; i < 5; i++)
			if(cpy[i] == j+1)
				score[j] += cpy[i];
	

	for (i = 0; i < 5; i++) // ���̽� (0 �̻� ����)
		score[6] += cpy[i];


	for (i = 0; i < 2; i++) // 4 ���� �� ī�ε� (0 ���� �ʿ�)
	{
		if ((cpy[i] == cpy[i + 1] && cpy[i + 1] == cpy[i + 2] && cpy[i + 2] == cpy[i + 3]) && (cpy [i] != 0 && cpy[i+1] != 0 && cpy[i + 2] != 0 && cpy[i + 3] != 0))
		{
			for (j = 0; j < 4; j++)
				score[7] += cpy[i + j];
			break;
		}
	}


	// Ǯ�Ͽ콺 (0 ���� �ʿ�)
	if ((cpy[0] == cpy[1] && (cpy[2] == cpy[3] && cpy[3] == cpy[4]) || (cpy[0] == cpy[1] && cpy[1] == cpy[2]) && cpy[3] == cpy[4]) && (cpy [0] != 0 && cpy[1] != 0 && cpy[2] != 0 && cpy[3] != 0 && cpy[4] != 0))
		for (i = 0; i < 5; i++)
			score[8] += cpy[i];
			
			
	//��Ʋ ��Ʈ����Ʈ (0 �̻� ����)
	if (cpy[0] == 1 && cpy[1] == 2 && cpy[2] == 3 && cpy[3] == 4 && cpy[4] == 5)
		score[9] = 30;
	
	
	//�� ��Ʈ����Ʈ (0 �̻� ����)
	if (cpy[0] == 2 && cpy[1] == 3 && cpy[2] == 4 && cpy[3] == 5 && cpy[4] == 6)
		score[10] = 30;

	//��Ʈ
	if (cpy[0] == cpy[1] && cpy[1] == cpy[2] && cpy[2] == cpy[3] && cpy[3] == cpy[4] && (cpy[0] !=0 && cpy[1] != 0 && cpy[2] != 0 && cpy[3] != 0 && cpy[4] != 0))
		score[11] = 50;

	// ���� ������ ���
	printf("��������������������������������������������������\n");
	for (i = 0, j=-6; i < 12; i++, j++)
	{
		if (i == 0)
			printf("  Ones  : %d [%c]\n", score[i], hi->flag[i]);
		else if(i == 1)
			printf("  Twos  : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 2)
			printf("  Threes : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 3)
			printf("  Fours  : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 4)
			printf("  Fives  : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 5)
			printf("  Sixes  : %d [%c]\n", score[i], hi->flag[i]);
		else if (i == 6)
			printf("  Choice  : %d [%c]\n", score[i], lo->flag[j]);
		else if (i == 7)
			printf("  4 of a Kind  : %d [%c]\n", score[i], lo->flag[j]);
		else if (i == 8)
			printf("  Full House  : %d [%c]\n", score[i], lo->flag[j]);
		else if (i == 9)
			printf("  L. Straight : %d [%c]\n", score[i], lo->flag[j]);
		else if (i == 10)
			printf("  B. Straight : %d [%c]\n", score[i], lo->flag[j]);
		else
			printf("     Yacht    : %d [%c]\n", score[i], lo->flag[j]);
	}
	printf("��������������������������������������������������\n");
}

void dice_and_select(int temp[],int last, high* hi, low* lo) // �ֻ��� �� ����(�Լ� �̿�) ��� ���. �� ����
{
	int dice[5] = { 0 }, i, sel, tmp;
	int score[12];
	for(;last >= 0;last--)
	{
		for (i = 0; i < 5; i++)
			dice[i] = rand() % 6 + 1;
		for (i = 0; i < 5; i++)
		{
			if(temp[i] != 0)
				dice[i] = 0;
		}
		while (1)
		{
			system("cls");
			print_Dnm_score(score, temp, hi, lo);
			printf("�ֻ��� ���\n\n");
			printf("[�ֻ���]:");
			for (i = 0; i < 5; i++)
				printf(" %d", dice[i]);
			printf("\n         ������\n");
			printf("[ ���� ]:");
			for (i = 0; i < 5; i++)
				printf(" %d", temp[i]);
			printf("\n[0 = �ٽ� ������, 9 = ���� ����] �ֻ��� %d�� ����\n", last);
			printf("[�ֻ��� ����]: ");
			scanf("%d", &sel);
			if (sel == 9 || last == 0)
			{
				for (i = 0; i < 5; i++)
					if (temp[i] == 0)
					{
						tmp = temp[i];
						temp[i] = dice[i];
						dice[i] = tmp;
					}
				system("cls");
				print_Dnm_score(score, temp, hi, lo);
				selects(score, hi, lo);
				return;
			}
			else if (sel > 5 || sel < 1)
				break;
			else
			{
				tmp = temp[sel - 1];
				temp[sel - 1] = dice[sel - 1];
				dice[sel - 1] = tmp;
			}
		}
	}

}

int print_turn(void) // �� ���ϴ� ���� ����
{
	int res, i, ans;
	for (i = 0; i < 36; i++)
	{
		if (i == 4)
		{
			printf("               [Yacht Dice game]               \n");
		}
		else if (i == 6)
		{
			printf("                  [�����Ͻÿ�]                 \n");
		}
		else if (i == 8)
		{
			printf("                    [1. ����]                  \n");
		}
		else if (i == 10)
		{
			printf("                    [2. ����]                  \n");
		}
		else if (i == 12)
		{
			printf("                     [3. ��]                   \n");
		}
		else
		{
			printf("                                               \n");
		}
	}
	while (1)
	{
		srand((unsigned int)time(NULL));
		ans = rand() % 3 + 1;
		printf("[�Է�]: ");
		scanf("%d", &res);
		system("cls");
		switch (res)
		{
		case 1:
			if (ans == 1)
			{
				printf("[�Է�]: ����\n");
				printf("ans [����]: �����ϴ�.\n");
			}
			else if (ans == 2)
			{
				printf("[�Է�]: ����\n");
				printf("ans [����]: �����ϴ�.\n");
				return 1;
			}
			else if (ans == 3)
			{
				printf("[�Է�]: ����\n");
				printf("ans [��]: �̰���ϴ�.\n");
				return 0;
			}
			break;
		case 2:
			if (ans == 1)
			{
				printf("[�Է�]: ����\n");
				printf("ans [����]: �̰���ϴ�.\n");
				return 0;
			}
			else if (ans == 2)
			{
				printf("[�Է�]: ����\n");
				printf("ans [����]: �����ϴ�.\n");
			}
			else if (ans == 3)
			{
				printf("[�Է�]: ����\n");
				printf("ans [��]: �����ϴ�.\n");
				return 1;
			}
			break;
		case 3:
			if (ans == 1)
			{
				printf("[�Է�]: ��\n");
				printf("ans [����]: �����ϴ�.\n");
				return 1;
			}
			else if (ans == 2)
			{
				printf("[�Է�]: ��\n");
				printf("ans [����]: �̰���ϴ�.\n");
				return 0;
			}
			else if (ans == 3)
			{
				printf("[�Է�]: ��\n");
				printf("ans [��]: �����ϴ�.\n");
			}
			break;
		}
	}
	return 0;
}

int print_mainmenu(void) // ���� �޴� ���
{
	int i, sel;
	char* timpe = (char*)malloc(sizeof(char) * 50);
	FILE* fp = fopen("lasttime.txt", "r");
	fscanf(fp,"%[^\n]s", timpe);
	fclose(fp);
	system("mode con cols=48 lines=36");
	for (i = 0; i < 36; i++)
	{
		if (i == 4)
		{
			printf("               [Yacht Dice game]               \n");
		}
		else if (i == 6)
		{
			printf("[���������� �÷��� �� �ð�: %s]\n", timpe);
			free(timpe);
		}
		else if (i == 12)
		{
			printf("                [1. ���� ����]                 \n");
		}
		else if (i == 14)
		{
			printf("                [2. ����  �� ]                 \n");
		}
		else if (i == 16)
		{
			printf("                [3. ��� ����]                 \n");
		}
		else if (i == 18)
		{
			printf("                [4. ���� ����]                 \n");
		}
		else
		{
			printf("                                               \n");
		}
	}
	printf("[����]: ");
	scanf("%d", &sel);
	return sel;
}

void play(int turn, high * p1_H_s, low * p1_L_s, high * p2_H_s, low * p2_L_s) // �Ͽ� ���� ���ھ� ���� ���
{
	system("mode con cols=128 lines=36");
	int last=2, i, pan, p1_subtotal, p2_subtotal, p1_total, p2_total, conti=1;
	int p1_temp[5] = { 0 }, p2_temp[5] = {0};
	while (conti <= 24)
	{
		if (turn % 2 == 0) // �÷��̾� 1
		{
			system("cls");
			for (pan = 0; pan < 32; pan++)
			{
				if (pan == 0)
				{
					printf("����������������������������������������������������������������������������������������������������\n");
				}
				else if (pan == 1)
				{
					printf("��    [Turn]   ��   [�÷��̾� 1]  ��   [�÷��̾� 2] ��\n");
				}
				else if (pan == 2)
				{
					printf("��   %2d / 24   ��     your turn   ��                ��\n", conti);
				}
				else if (pan == 3 || pan == 5 || pan == 7 || pan == 9 || pan == 11 || pan == 13 || pan == 15 || pan == 17 || pan == 19 || pan == 21 || pan == 23 || pan == 25 || pan == 27 || pan == 29)
				{
					printf("����������������������������������������������������������������������������������������������������\n");
				}
				else if (pan == 4)
				{
					printf("��    Ones     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[0], p1_H_s->flag[0], p2_H_s->arr[0], p2_H_s->flag[0]);
				}
				else if (pan == 6)
				{
					printf("��    Twos     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[1], p1_H_s->flag[1], p2_H_s->arr[1], p2_H_s->flag[1]);
				}
				else if (pan == 8)
				{
					printf("��   Threes    ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[2], p1_H_s->flag[2], p2_H_s->arr[2], p2_H_s->flag[2]);
				}
				else if (pan == 10)
				{
					printf("��   Fours     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[3], p1_H_s->flag[3], p2_H_s->arr[3], p2_H_s->flag[3]);
				}
				else if (pan == 12)
				{
					printf("��   Fives     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[4], p1_H_s->flag[4], p2_H_s->arr[4], p2_H_s->flag[4]);
				}
				else if (pan == 14)
				{
					printf("��   Sixes     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[5], p1_H_s->flag[5], p2_H_s->arr[5], p2_H_s->flag[5]);
				}
				else if (pan == 16)
				{
					for (p1_subtotal = 0, p2_subtotal = 0, i = 0; i < 6; i++)
					{
						if (p1_H_s->flag[i] == 'X')
							p1_subtotal += p1_H_s->arr[i];
						if (p2_H_s->flag[i] == 'X')
							p2_subtotal += p2_H_s->arr[i];
					}
					if (p1_subtotal >= 63 && p2_subtotal >= 63)
					{
						printf("��    Bonus    ��        +40       ��       +40       ��\n");
					}
					else
					{
						if (p1_subtotal >= 63)
						{
							printf("��    Bonus    ��        +40       ��                 ��\n");
							p1_subtotal += BONUS;
						}
						else if (p2_subtotal >= 63)
						{
							printf("��    Bonus    ��                  ��       +40       ��\n");
							p2_subtotal += BONUS;
						}
					}
					printf("��   Subtotal  ��       %2d/63     ��      %2d/63     ��\n", p1_subtotal, p2_subtotal);
				}
				else if (pan == 18)
				{
					printf("��   Choice    ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[0], p1_L_s->flag[0], p2_L_s->arr[0], p2_L_s->flag[0]);
				}
				else if (pan == 20)
				{
					printf("�� 4 of a Kind ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[1], p1_L_s->flag[1], p2_L_s->arr[1], p2_L_s->flag[1]);
				}
				else if (pan == 22)
				{
					printf("�� Full House  ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[2], p1_L_s->flag[2], p2_L_s->arr[2], p2_L_s->flag[2]);
				}
				else if (pan == 24)
				{
					printf("�� L. Straight ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[3], p1_L_s->flag[3], p2_L_s->arr[3], p2_L_s->flag[3]);
				}
				else if (pan == 26)
				{
					printf("�� B.Straight  ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[4], p1_L_s->flag[4], p2_L_s->arr[4], p2_L_s->flag[4]);
				}
				else if (pan == 28)
				{

					printf("��    Yacht    ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[5], p1_L_s->flag[5], p2_L_s->arr[5], p2_L_s->flag[5]);
				}
				else if (pan == 30)
				{
					for (p1_total = 0, p2_total = 0, i = 0; i < 6; i++)
					{
						if (p1_L_s->flag[i] == 'X')
							p1_total += p1_L_s->arr[i];
						if (p2_L_s->flag[i] == 'X')
							p2_total += p2_L_s->arr[i];
					}
					p1_total += p1_subtotal;
					p2_total += p2_subtotal;
					printf("��    total    ��        %3d      ��       %3d      ��\n", p1_total, p2_total);
				}
				else
				{
					printf("����������������������������������������������������������������������������������������������������\n");
				}
			}
			printf("[�÷��̾� 1�� ��]\n");
			printf("�ֻ��� �����⸦ ");
			system("pause"); // "�ֻ��� �����⸦ ����Ͻ÷��� �ƹ� Ű�� �����ʽÿ� . . ."
			dice_and_select(p1_temp, last, p1_H_s, p1_L_s);
			reset_dice(p1_temp);
			conti += 1;
		}
		else // �÷��̾� 2 [����ȭ�� ���� ���� �����]
		{
			system("cls");
			for (pan = 0; pan < 32; pan++) // ���� ���ھ� �۾� �����
			{
				if (pan == 0)
				{
					printf("����������������������������������������������������������������������������������������������������\n");
				}
				else if (pan == 1)
				{
					printf("��    [Turn]   ��   [�÷��̾� 1]  ��   [�÷��̾� 2] ��\n");
				}
				else if (pan == 2)
				{
					printf("��   %2d / 24   ��                 ��     your turn  ��\n", conti);
				}
				else if (pan == 3 || pan == 5 || pan == 7 || pan == 9 || pan == 11 || pan == 13 || pan == 15 || pan == 17 || pan == 19 || pan == 21 || pan == 23 || pan == 25 || pan == 27 || pan == 29)
				{
					printf("����������������������������������������������������������������������������������������������������\n");
				}
				else if (pan == 4)
				{
					printf("��    Ones     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[0], p1_H_s->flag[0], p2_H_s->arr[0], p2_H_s->flag[0]);
				}
				else if (pan == 6)
				{
					printf("��    Twos     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[1], p1_H_s->flag[1], p2_H_s->arr[1], p2_H_s->flag[1]);
				}
				else if (pan == 8)
				{
					printf("��   Threes    ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[2], p1_H_s->flag[2], p2_H_s->arr[2], p2_H_s->flag[2]);
				}
				else if (pan == 10)
				{
					printf("��   Fours     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[3], p1_H_s->flag[3], p2_H_s->arr[3], p2_H_s->flag[3]);
				}
				else if (pan == 12)
				{
					printf("��   Fives     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[4], p1_H_s->flag[4], p2_H_s->arr[4], p2_H_s->flag[4]);
				}
				else if (pan == 14)
				{
					printf("��   Sixes     ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_H_s->arr[5], p1_H_s->flag[5], p2_H_s->arr[5], p2_H_s->flag[5]);
				}
				else if (pan == 16)
				{
					for (p1_subtotal = 0, p2_subtotal = 0, i = 0; i < 6; i++)
					{
						if (p1_H_s->flag[i] == 'X')
							p1_subtotal += p1_H_s->arr[i];
						if (p2_H_s->flag[i] == 'X')
							p2_subtotal += p2_H_s->arr[i];
					}
					if (p1_subtotal >= 63 && p2_subtotal >= 63)
					{
						printf("��    Bonus    ��        +40       ��       +40       ��\n");
					}
					else
					{
						if (p1_subtotal >= 63)
						{
							printf("��    Bonus    ��        +40       ��                 ��\n");
							p1_subtotal += BONUS;
						}
						else if (p2_subtotal >= 63)
						{
							printf("��    Bonus    ��                  ��       +40       ��\n");
							p2_subtotal += BONUS;
						}
					}
					printf("��   Subtotal  ��       %2d/63     ��      %2d/63     ��\n", p1_subtotal, p2_subtotal);
				}
				else if (pan == 18)
				{
					printf("��   Choice    ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[0], p1_L_s->flag[0], p2_L_s->arr[0], p2_L_s->flag[0]);
				}
				else if (pan == 20)
				{
					printf("�� 4 of a Kind ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[1], p1_L_s->flag[1], p2_L_s->arr[1], p2_L_s->flag[1]);
				}
				else if (pan == 22)
				{
					printf("�� Full House  ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[2], p1_L_s->flag[2], p2_L_s->arr[2], p2_L_s->flag[2]);
				}
				else if (pan == 24)
				{
					printf("�� L. Straight ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[3], p1_L_s->flag[3], p2_L_s->arr[3], p2_L_s->flag[3]);
				}
				else if (pan == 26)
				{
					printf("�� B.Straight  ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[4], p1_L_s->flag[4], p2_L_s->arr[4], p2_L_s->flag[4]);
				}
				else if (pan == 28)
				{

					printf("��    Yacht    ��       %2d [%c]    ��      %2d [%c]    ��\n", p1_L_s->arr[5], p1_L_s->flag[5], p2_L_s->arr[5], p2_L_s->flag[5]);
				}
				else if (pan == 30)
				{
					for (p1_total = 0, p2_total = 0, i = 0; i < 6; i++)
					{
						if (p1_L_s->flag[i] == 'X')
							p1_total += p1_L_s->arr[i];
						if (p2_L_s->flag[i] == 'X')
							p2_total += p2_L_s->arr[i];
					}
					p1_total += p1_subtotal;
					p2_total += p2_subtotal;
					printf("��    total    ��        %3d      ��       %3d      ��\n", p1_total, p2_total);
				}
				else
				{
					printf("����������������������������������������������������������������������������������������������������\n");
				}
			}
			printf("[�÷��̾� 2�� ��]\n");
			printf("�ֻ��� �����⸦ ");
			system("pause"); // "�ֻ��� �����⸦ ����Ͻ÷��� �ƹ� Ű�� �����ʽÿ� . . ."
			dice_and_select(p2_temp, last, p2_H_s, p2_L_s);
			reset_dice(p2_temp);
			conti += 1;
		}
		turn += 1;
	}
	result(p1_total, p2_total);
	return;
}