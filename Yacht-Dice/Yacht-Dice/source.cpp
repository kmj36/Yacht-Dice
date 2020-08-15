#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#pragma warning(disable : 4996)
// i,j,k ... 등은 다 인덱스 변수임.
#define BONUS 40 // 보조 합계가 63 이상일 경우 + BONUS

int compare(const void* a, const void* b) // 비교 오픈 소스 이용함
{
	return *(int*)a - *(int*)b; // 역참조 후 a-b 후 반환
}

typedef struct high_sections //상단 족보
{
	int arr[6]; // 점수 배열
	char flag[6]; // 점수 중복 쓰기 방지
}high;

typedef struct low_sections // 하단 족보
{
	int arr[6]; // 점수 배열
	char flag[6]; // 점수 중복 쓰기 방지
}low;

int print_mainmenu(FILE *, char *); // 메인 메뉴
int print_turn(void); // 가위바위보로 턴 정하기
void play(int, high*, low*, high*, low*, FILE*); // 플레이
void dice_and_select(int[],int, high*, low*); // 주사위 굴리기 밑 선택
void reset_dice(int []); // 주사위 0으로 초기화
void print_Dnm_score(int[], int[], high*, low*); // 동적 점수판 출력
void selects(int[],high*, low*); // 점수 선택
void reset_structer(high*, low*, high*, low*); // 구조체 숫자 0, 알파벳 O으로 초기화
void result(int, int, FILE*); // 마지막 비교 후 결과 출력
void rule(void); // 규칙 출력
void using_music(void); // 사용 음악 출력

int main(void) // main
{
	system("title Yacht Dice"); // exe 제목 Yacht Dice로 지정
	PlaySound(TEXT("bgm.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP); // 음악 출력, 무한 반복
	high *p1_H_s = (high*)malloc(sizeof(high)), *p2_H_s = (high*)malloc(sizeof(high)); // 구조체 포인터 동적할당
	low *p1_L_s = (low*)malloc(sizeof(low)) , *p2_L_s = (low*)malloc(sizeof(low)); // 구조체 동적할당
	FILE* tfp = (FILE*)malloc(sizeof(FILE));
	static char* timpe = (char*)malloc(sizeof(char) * 20); // 시간을 받을 배열 동적할당
	tfp = fopen("lasttime.txt", "r+");
	int turn, sel=4; // 턴 및 메인 메뉴 선택 초기화 4 = 프로그램 종료

	reset_structer(p1_H_s, p1_L_s, p2_H_s, p2_L_s);
	while (sel != 1) // 1이면 가위바위보로 이동
	{
		sel = print_mainmenu(tfp, timpe);
		if (sel == 2)
			rule();
		else if (sel == 3)
			using_music();
		else if (sel == 4)
			return 0;
	}
	free(timpe);
	turn = print_turn(); // 가위바위보 후 턴 반환
	
	if (turn % 2 == 0) // 턴에 따라 플레이어 선택
		printf("[플레이어 1] 부터 시작합니다.\n");
	else
		printf("[플레이어 2] 부터 시작합니다.\n");

	PlaySound(NULL, 0, 0);

	// 로딩하는 것 같은 화면 효과
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

	play(turn, p1_H_s, p1_L_s, p2_H_s, p2_L_s, tfp); // 플레이

	PlaySound(NULL, 0, 0);
	// 동적할당 반환
	free(p1_H_s);
	free(p1_L_s);
	free(p2_H_s);
	free(p2_L_s);
	return 0;
}

void using_music(void)
{
	system("mode con cols=48 lines=36"); // 화면 크기 변경
	printf("[MAIN MENU] Kevin MacLeod - Samba Isobel\n"); // 유튜브 무료음원 라이브러리에서 구함.
	printf("[GAME OST] Kevin MacLeod ~ Apero Hour\n");
	system("pause");
}

void rule(void) // 규칙 설명 출력
{
	system("mode con cols=80 lines=50");
	int i;
	for (i = 0; i < 48; i++)
	{
		if (i == 0)
			printf("[게임 진행]\n");
		else if (i == 2)
			printf("1. 주사위 5개를 던집니다.\n");
		else if (i == 4)
			printf("2. 총 3번 돌릴 수 있고 원하는 주사위를 1~5으로 선택하고 취소할 수 있습니다.\n");
		else if (i == 6)
			printf("[0 주사위 다시 돌리기  9 점수 선택] 중 선택합니다.\n");
		else if (i == 7)
			printf("*점수 선택 시 주사위 다시 돌리기가 불가능합니다.*\n");
		else if (i == 9)
			printf("3. 주사위를 3번 돌렸거나[점수 선택]을\n");
		else if (i == 10)
			printf("선택했을 경우 반드시 족보(점수판)에 기록해야 합니다.\n");
		else if (i == 12)
			printf("4. 총 24턴을 진행하면 게임이 끝나고 점수 총합으로 승패를 정합니다.\n");
		else if (i == 14)
			printf("[족보]\n");
		else if (i == 16)
			printf("[상단]\n");
		else if (i == 17)
			printf("Ones: 주사위 1 눈의 총합.\n");
		else if (i == 18)
			printf("Twos : 주사위 2 눈의 총합.\n");
		else if (i == 19)
			printf("Threes : 주사위 3 눈의 총합.\n");
		else if (i == 20)
			printf("Fours : 주사위 4 눈의 총합.\n");
		else if (i == 21)
			printf("Fives : 주사위 5 눈의 총합.\n");
		else if (i == 22)
			printf("Sixes : 주사위 6 눈의 총합.\n");
		else if (i == 23)
			printf("[보너스] : 상단 항목의 점수가 63점 이상일때, +40점.\n");
		else if (i == 25)
			printf("[하단]\n");
		else if (i == 26)
			printf("Choice : 주사위 5 개의 총합.\n");
		else if (i == 27)
			printf("4 of a Kind : 4개의 주사위가 동일할때, 동일한 주사위 눈 4의 총합.\n");
		else if (i == 28)
			printf("Full House : 3개, 2개의 주사위가 동일할때, 주사위 5개의 총합.\n");
		else if (i == 29)
			printf("Little Straight : 주사위 눈이 1, 2, 3, 4, 5 고정 30점.\n");
		else if (i == 30)
			printf("Big Straight : 주사위 눈이 2, 3, 4, 5, 6 고정 30점.\n");
		else if (i == 31)
			printf("Yacht : 5개의 주사위가 모두 동일 고정 50점.\n");
		else
			printf("                                               \n");
	}
	system("pause");
}

void result(int p1, int p2, FILE * fp)
{
	time_t T = time(NULL); // 시간을 받기위해 time_t 구조체 T 선언
	struct tm tm = *localtime(&T); // 현재 지역 시간 받기
	system("cls");
	if (p1 > p2) // 승패 받기
		printf("[플레이어 1] 승리!!!");
	else
		printf("[플레이어 2] 승리!!!");
	fprintf(fp, "%d-%d-%d %d:%d:%d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); // 파일 쓰기
	fclose(fp); // 파일 닫기
	free(fp);
	system("pause");
}

void reset_structer(high* p1_H_s, low* p1_L_s, high* p2_H_s, low* p2_L_s) // 상단, 하단 구조체 초기화
{
	int i;
	for (i = 0; i < 6; i++)
	{
		p1_H_s->arr[i] = p1_L_s->arr[i] = p2_H_s->arr[i] = p2_L_s->arr[i] = 0;
		p1_H_s->flag[i] = p1_L_s->flag[i] = p2_H_s->flag[i] = p2_L_s->flag[i] = 'O';
	}
}

void selects(int score[], high * hi, low * lo) // 점수 선택
{
	int i, j, sel;
	system("cls");
	printf("┌───────────────────────┐\n");
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
	printf("└───────────────────────┘\n");
	while (1) //조건 1.플래그 판별 hi, lo 2. 플래그가 O 이어야함, 3. 1 ~ 12 이어야 함
	{
		printf("[점수 선택]: ");
		scanf("%d", &sel);
		if (sel < 1 || sel > 12) // 범위 벗어남 방지
		{
			printf("범위를 벗어났습니다. 다시 선택하여주세요.\n");
			continue; // 다시 입력 받기 위해 돌아가기
		}
		sel -= 1; // 인덱스로 사용하기 위해 선택 -1
		if (sel < 6) // 상단, 하단 구별 [상단]
		{
			if (hi->flag[sel] == 'X')
			{
				printf("중복할 수 없습니다. 다시 선택하여주세요.\n");
				continue;
			}
			hi->arr[sel] = score[sel]; // 점수 배열 쓰기
			hi->flag[sel] = 'X'; // 중복 쓰기 방지을 위해 'X' 쓰기
			return; // 함수 탈출
		}
		else //[하단]
		{
			if (lo->flag[sel-6] == 'X')
			{
				printf("중복할 수 없습니다. 다시 선택하여주세요.\n");
				continue;
			}
			lo->arr[sel - 6] = score[sel];
			lo->flag[sel - 6] = 'X';
			return;
		}
	}
}

void reset_dice(int dice[]) // 주사위 배열 초기화
{
	int i;
	for (i = 0; i < 5; i++)
		dice[i] = 0;
}

void print_Dnm_score(int score[], int temp[], high * hi, low * lo) // 동적 점수 출력
{
	int cpy[5], i, j;
	for (i = 0; i < 5; i++)
		cpy[i] = temp[i];
	for (i = 0; i < 12; i++) // 초기화
		score[i] = 0;
	qsort(cpy, sizeof(cpy) / sizeof(int), sizeof(int), compare);
	// 점수 집계


	for(j=0;j<6;j++) // 상단 점수 집계 (0 이상 없음)
		for (i = 0; i < 5; i++)
			if(cpy[i] == j+1)
				score[j] += cpy[i];
	

	for (i = 0; i < 5; i++) // 초이스 (0 이상 없음)
		score[6] += cpy[i];


	for (i = 0; i < 2; i++) // 4 오브 어 카인드 (0 수정 필요)
	{
		if ((cpy[i] == cpy[i + 1] && cpy[i + 1] == cpy[i + 2] && cpy[i + 2] == cpy[i + 3]) && (cpy [i] != 0 && cpy[i+1] != 0 && cpy[i + 2] != 0 && cpy[i + 3] != 0))
		{
			for (j = 0; j < 4; j++)
				score[7] += cpy[i + j];
			break;
		}
	}


	// 풀하우스 (0 수정 필요)
	if ((cpy[0] == cpy[1] && (cpy[2] == cpy[3] && cpy[3] == cpy[4]) || (cpy[0] == cpy[1] && cpy[1] == cpy[2]) && cpy[3] == cpy[4]) && (cpy [0] != 0 && cpy[1] != 0 && cpy[2] != 0 && cpy[3] != 0 && cpy[4] != 0))
		for (i = 0; i < 5; i++)
			score[8] += cpy[i];
			
			
	//리틀 스트레이트 (0 이상 없음)
	if (cpy[0] == 1 && cpy[1] == 2 && cpy[2] == 3 && cpy[3] == 4 && cpy[4] == 5)
		score[9] = 30;
	
	
	//빅 스트레이트 (0 이상 없음)
	if (cpy[0] == 2 && cpy[1] == 3 && cpy[2] == 4 && cpy[3] == 5 && cpy[4] == 6)
		score[10] = 30;

	//요트
	if (cpy[0] == cpy[1] && cpy[1] == cpy[2] && cpy[2] == cpy[3] && cpy[3] == cpy[4] && (cpy[0] !=0 && cpy[1] != 0 && cpy[2] != 0 && cpy[3] != 0 && cpy[4] != 0))
		score[11] = 50;

	// 동적 점수판 출력
	printf("┌───────────────────────┐\n");
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
	printf("└───────────────────────┘\n");
}

void dice_and_select(int temp[],int last, high* hi, low* lo) // 주사위 와 선택(함수 이용) 모두 담당. 주 로직
{
	int dice[5] = { 0 }, i, sel, tmp; // 주사위 배열, 선택, 스왑을 위한 변수
	int score[12]; // 점수 배열
	for(;last >= 0;last--) // 턴 0까지 -1 탈출
	{
		for (i = 0; i < 5; i++)
			dice[i] = rand() % 6 + 1; // 주사위 무작위 숫자 쓰기
		for (i = 0; i < 5; i++)
		{
			if(temp[i] != 0) // 이미 선택한 숫자의 주사위를 제외하기 위해
				dice[i] = 0; // 해당 주사위를 0으로 바꿈
		}
		while (1)
		{
			system("cls");
			print_Dnm_score(score, temp, hi, lo); // 동적 점수판 출력
			printf("주사위 결과\n\n");
			printf("[주사위]:");
			for (i = 0; i < 5; i++)
				printf(" %d", dice[i]); // 주사위 출력
			printf("\n         ↓↓↓↓↓\n");
			printf("[ 선택 ]:");
			for (i = 0; i < 5; i++)
				printf(" %d", temp[i]); // 플레이어의 선택한 주사위
			printf("\n[0 = 다시 돌리기, 9 = 점수 선택] 주사위 %d번 남음\n", last);
			printf("[주사위 선택]: ");
			scanf("%d", &sel);
			if (sel == 9 || last == 0)
			{
				for (i = 0; i < 5; i++)
					if (temp[i] == 0) // 모든 주사위를 선택하지 않고 넘어갔을때
					{
						tmp = temp[i]; // 스왑으로 자동 넣기
						temp[i] = dice[i];
						dice[i] = tmp;
					}
				system("cls");
				print_Dnm_score(score, temp, hi, lo); // 점수 재 반영을 위해 한번더 점수판 출력
				selects(score, hi, lo); // 점수 선택
				return;
			}
			else if (sel > 5 || sel < 1) // 1 ~ 5 를 벗어나면 주사위 다시 돌리기로 인식
				break;
			else // 1~5 에 따라 스왑
			{
				tmp = temp[sel - 1];
				temp[sel - 1] = dice[sel - 1];
				dice[sel - 1] = tmp;
			}
		}
	}

}

int print_turn(void) // 턴 정하는 보조 로직
{
	system("cls"); // 화면 지우기
	int res, ans;
	printf("\n\n               [Yacht Dice game]               \n\n\n                  [선택하시오]                 \n\n\n\n\n\n                    [1. 가위]                  \n\n\n                    [2. 바위]                  \n\n\n                     [3. 보]                  \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	while (1)
	{
		srand((unsigned int)time(NULL)); // 시간에 따라 rand의 값 변경
		ans = rand() % 3 + 1; // 시간에 따른 무작위 1 ~ 3 중 값 하나 받음
		printf("[입력]: ");
		scanf("%d", &res); // 가위, 바위, 보
		system("cls");
		switch (res) // switch 문을 이용
		{
		case 1: // 가위
			if (ans == 1)
			{
				printf("[입력]: 가위\n");
				printf("ans [가위]: 비겼습니다.\n");
			}
			else if (ans == 2)
			{
				printf("[입력]: 가위\n");
				printf("ans [바위]: 졌습니다.\n");
				return 1;
			}
			else if (ans == 3)
			{
				printf("[입력]: 가위\n");
				printf("ans [보]: 이겼습니다.\n");
				return 0;
			}
			break;
		case 2: // 바위
			if (ans == 1)
			{
				printf("[입력]: 바위\n");
				printf("ans [가위]: 이겼습니다.\n");
				return 0;
			}
			else if (ans == 2)
			{
				printf("[입력]: 바위\n");
				printf("ans [바위]: 비겼습니다.\n");
			}
			else if (ans == 3)
			{
				printf("[입력]: 바위\n");
				printf("ans [보]: 졌습니다.\n");
				return 1;
			}
			break;
		case 3: // 보
			if (ans == 1)
			{
				printf("[입력]: 보\n");
				printf("ans [가위]: 졌습니다.\n");
				return 1;
			}
			else if (ans == 2)
			{
				printf("[입력]: 보\n");
				printf("ans [바위]: 이겼습니다.\n");
				return 0;
			}
			else if (ans == 3)
			{
				printf("[입력]: 보\n");
				printf("ans [보]: 비겼습니다.\n");
			}
			break;
		}
	}
	return 0;
}

int print_mainmenu(FILE * fp, char * timpe) // 메인 메뉴 담당
{
	int sel;
	fscanf(fp,"%[^\n]s", timpe); // char * timpe에 마지막으로 플레이한 시간 넣음
	system("mode con cols=48 lines=36");
	printf("\n\n               [Yacht Dice game]               \n\n\n[마지막으로 플레이 한 시간: %s]\n\n\n\n\n\n", timpe); // 마지막으로 플레이한 시간 출력
	printf("\n                [1. 게임 시작]                 \n\n\n                [2. 게임  룰 ]                 \n\n\n                [3. 사용 음악]                 \n\n\n                [4. 게임 종료]                 \n\n\n\n\n\n\n\n\n\n\n\n\n\n[선택]: ");
	scanf("%d", &sel);
	return sel; // 메인메뉴 선택 반환
}

void play(int turn, high * p1_H_s, low * p1_L_s, high * p2_H_s, low * p2_L_s, FILE * fp) // 턴에 따른 스코어 보드 출력
{
	system("mode con cols=128 lines=36");
	/*주사위가 dice_and_selects 를 호출할 시 바로 주사위가 나오기 때문에 last = 2
	pan은 1~32 까지 돌면서 화면에 출력하는 변수, 보조 합계 및 합계 subtotal, total*/
	int last=2, i, pan, p1_subtotal, p2_subtotal, p1_total, p2_total, conti=1;
	int p1_temp[5] = { 0 }, p2_temp[5] = {0}; // 플레이어의 주사위 배열
	while (conti <= 24) // 1 ~ 24 턴 까지
	{
		if (turn % 2 == 0) // 플레이어 1
		{
			system("cls");
			for (pan = 0; pan < 32; pan++)
			{
				if (pan == 0)
				{
					printf("┌─────────────┬─────────────────┬────────────────┐\n");
				}
				else if (pan == 1)
				{
					printf("│    [Turn]   │   [플레이어 1]  │   [플레이어 2] │\n");
				}
				else if (pan == 2)
				{
					printf("│   %2d / 24   │     your turn   │                │\n", conti); // 현재 진행한 턴 출력
				}
				else if (pan == 3 || pan == 5 || pan == 7 || pan == 9 || pan == 11 || pan == 13 || pan == 15 || pan == 17 || pan == 19 || pan == 21 || pan == 23 || pan == 25 || pan == 27 || pan == 29)
				{
					printf("├─────────────┼─────────────────┼────────────────┤\n");
				}
				else if (pan == 4)
				{
					printf("│    Ones     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[0], p1_H_s->flag[0], p2_H_s->arr[0], p2_H_s->flag[0]);
				}
				else if (pan == 6)
				{
					printf("│    Twos     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[1], p1_H_s->flag[1], p2_H_s->arr[1], p2_H_s->flag[1]);
				}
				else if (pan == 8)
				{
					printf("│   Threes    │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[2], p1_H_s->flag[2], p2_H_s->arr[2], p2_H_s->flag[2]);
				}
				else if (pan == 10)
				{
					printf("│   Fours     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[3], p1_H_s->flag[3], p2_H_s->arr[3], p2_H_s->flag[3]);
				}
				else if (pan == 12)
				{
					printf("│   Fives     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[4], p1_H_s->flag[4], p2_H_s->arr[4], p2_H_s->flag[4]);
				}
				else if (pan == 14)
				{
					printf("│   Sixes     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[5], p1_H_s->flag[5], p2_H_s->arr[5], p2_H_s->flag[5]);
				}
				else if (pan == 16)
				{
					for (p1_subtotal = 0, p2_subtotal = 0, i = 0; i < 6; i++) // 보조 합계를 0으로 다시 초기화 후 사용
					{
						// 점수가 쓰인 배열만 보조 합계 합산
						if (p1_H_s->flag[i] == 'X')
							p1_subtotal += p1_H_s->arr[i];
						if (p2_H_s->flag[i] == 'X')
							p2_subtotal += p2_H_s->arr[i];
					}
					if (p1_subtotal >= 63 && p2_subtotal >= 63) // 보조 합계가 63 이상일 경우 +40 점
					{
						printf("│    Bonus    │        +40       │       +40       │\n");
					}
					else
					{
						if (p1_subtotal >= 63)
						{
							printf("│    Bonus    │        +40       │                 │\n");
							p1_subtotal += BONUS;
						}
						else if (p2_subtotal >= 63)
						{
							printf("│    Bonus    │                  │       +40       │\n");
							p2_subtotal += BONUS;
						}
					}
					printf("│   Subtotal  │       %2d/63     │      %2d/63     │\n", p1_subtotal, p2_subtotal); // 보조 합계 출력
				}
				else if (pan == 18)
				{
					printf("│   Choice    │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[0], p1_L_s->flag[0], p2_L_s->arr[0], p2_L_s->flag[0]);
				}
				else if (pan == 20)
				{
					printf("│ 4 of a Kind │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[1], p1_L_s->flag[1], p2_L_s->arr[1], p2_L_s->flag[1]);
				}
				else if (pan == 22)
				{
					printf("│ Full House  │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[2], p1_L_s->flag[2], p2_L_s->arr[2], p2_L_s->flag[2]);
				}
				else if (pan == 24)
				{
					printf("│ L. Straight │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[3], p1_L_s->flag[3], p2_L_s->arr[3], p2_L_s->flag[3]);
				}
				else if (pan == 26)
				{
					printf("│ B.Straight  │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[4], p1_L_s->flag[4], p2_L_s->arr[4], p2_L_s->flag[4]);
				}
				else if (pan == 28)
				{

					printf("│    Yacht    │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[5], p1_L_s->flag[5], p2_L_s->arr[5], p2_L_s->flag[5]);
				}
				else if (pan == 30)
				{
					for (p1_total = 0, p2_total = 0, i = 0; i < 6; i++) // 합계 출력
					{
						// 점수가 쓰인 배열만 보조 합계 합산
						if (p1_L_s->flag[i] == 'X')
							p1_total += p1_L_s->arr[i];
						if (p2_L_s->flag[i] == 'X')
							p2_total += p2_L_s->arr[i];
					}
					p1_total += p1_subtotal; // 합계에 보조합계 합산
					p2_total += p2_subtotal;
					printf("│    total    │        %3d      │       %3d      │\n", p1_total, p2_total); // 합계 출력
				}
				else
				{
					printf("└─────────────┴─────────────────┴────────────────┘\n");
				}
			}
			printf("[플레이어 1의 턴]\n");
			printf("주사위 굴리기를 ");
			system("pause"); // "주사위 굴리기를 계속하시려면 아무 키나 누르십시오 . . ." 출력됨.
			dice_and_select(p1_temp, last, p1_H_s, p1_L_s); // p1 주사위 선택
			reset_dice(p1_temp); // 플레이어 1의 선택 주사위 초기화
			conti += 1; // 진행한 턴 1 추가
		}
		else // 플레이어 2 [플레이어 1]과 동일]
		{
			system("cls");
			for (pan = 0; pan < 32; pan++) 
			{
				if (pan == 0)
				{
					printf("┌─────────────┬─────────────────┬────────────────┐\n");
				}
				else if (pan == 1)
				{
					printf("│    [Turn]   │   [플레이어 1]  │   [플레이어 2] │\n");
				}
				else if (pan == 2)
				{
					printf("│   %2d / 24   │                 │     your turn  │\n", conti);
				}
				else if (pan == 3 || pan == 5 || pan == 7 || pan == 9 || pan == 11 || pan == 13 || pan == 15 || pan == 17 || pan == 19 || pan == 21 || pan == 23 || pan == 25 || pan == 27 || pan == 29)
				{
					printf("├─────────────┼─────────────────┼────────────────┤\n");
				}
				else if (pan == 4)
				{
					printf("│    Ones     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[0], p1_H_s->flag[0], p2_H_s->arr[0], p2_H_s->flag[0]);
				}
				else if (pan == 6)
				{
					printf("│    Twos     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[1], p1_H_s->flag[1], p2_H_s->arr[1], p2_H_s->flag[1]);
				}
				else if (pan == 8)
				{
					printf("│   Threes    │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[2], p1_H_s->flag[2], p2_H_s->arr[2], p2_H_s->flag[2]);
				}
				else if (pan == 10)
				{
					printf("│   Fours     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[3], p1_H_s->flag[3], p2_H_s->arr[3], p2_H_s->flag[3]);
				}
				else if (pan == 12)
				{
					printf("│   Fives     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[4], p1_H_s->flag[4], p2_H_s->arr[4], p2_H_s->flag[4]);
				}
				else if (pan == 14)
				{
					printf("│   Sixes     │       %2d [%c]    │      %2d [%c]    │\n", p1_H_s->arr[5], p1_H_s->flag[5], p2_H_s->arr[5], p2_H_s->flag[5]);
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
						printf("│    Bonus    │        +40       │       +40       │\n");
					}
					else
					{
						if (p1_subtotal >= 63)
						{
							printf("│    Bonus    │        +40       │                 │\n");
							p1_subtotal += BONUS;
						}
						else if (p2_subtotal >= 63)
						{
							printf("│    Bonus    │                  │       +40       │\n");
							p2_subtotal += BONUS;
						}
					}
					printf("│   Subtotal  │       %2d/63     │      %2d/63     │\n", p1_subtotal, p2_subtotal);
				}
				else if (pan == 18)
				{
					printf("│   Choice    │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[0], p1_L_s->flag[0], p2_L_s->arr[0], p2_L_s->flag[0]);
				}
				else if (pan == 20)
				{
					printf("│ 4 of a Kind │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[1], p1_L_s->flag[1], p2_L_s->arr[1], p2_L_s->flag[1]);
				}
				else if (pan == 22)
				{
					printf("│ Full House  │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[2], p1_L_s->flag[2], p2_L_s->arr[2], p2_L_s->flag[2]);
				}
				else if (pan == 24)
				{
					printf("│ L. Straight │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[3], p1_L_s->flag[3], p2_L_s->arr[3], p2_L_s->flag[3]);
				}
				else if (pan == 26)
				{
					printf("│ B.Straight  │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[4], p1_L_s->flag[4], p2_L_s->arr[4], p2_L_s->flag[4]);
				}
				else if (pan == 28)
				{

					printf("│    Yacht    │       %2d [%c]    │      %2d [%c]    │\n", p1_L_s->arr[5], p1_L_s->flag[5], p2_L_s->arr[5], p2_L_s->flag[5]);
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
					printf("│    total    │        %3d      │       %3d      │\n", p1_total, p2_total);
				}
				else
				{
					printf("└─────────────┴─────────────────┴────────────────┘\n");
				}
			}
			printf("[플레이어 2의 턴]\n");
			printf("주사위 굴리기를 ");
			system("pause");
			dice_and_select(p2_temp, last, p2_H_s, p2_L_s); // p2 주사위 선택
			reset_dice(p2_temp); // 플레이어2 의 선택 주사위 초기화
			conti += 1;
		}
		turn += 1;
	}
	result(p1_total, p2_total, fp);
	return;
}