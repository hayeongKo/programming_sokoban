#include <stdio.h>
#include <conio.h>
#include "cursor.h"

enum { ESC = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, BACK = 8 };
#define putchxy(x, y, c) {gotoxy(x, y);putch(c);}
#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
char arStage[][18][21] = {
   {
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111000111111111111",
      "11111200111111111111",
      "11111002111111111111",
      "11100202011111111111",
      "11101011011111111111",
      "10001011011111003311",
      "10200200040000003311",
      "11111011101011003311",
      "11111000001111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
   },
   {
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11113300100000111111",
      "11113300102002001111",
      "11113300121111001111",
      "11113300004011001111",
      "11113300101002011111",
      "11111111101120201111",
      "11111102002020201111",
      "11111100001000001111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
   },
   {
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111110000041111",
      "11111111110212011111",
      "11111111110200211111",
      "11111111111202011111",
      "11111111110201011111",
      "11333300110200200111",
      "11133300002002000111",
      "11333300111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
      "11111111111111111111",
   },
};
char ns[18][21];
int nx, ny;
char* arTile[] = { "  ", "■", "⊙", "◇", "★" };
enum { EMPTY = 48, WALL, PACK, DEPOT, MAN };
int stage = 0;

void getTime(time_t);
void getCount(int);

int main()
{
    int ch;
    int x, y;
    int dx, dy;
    BOOL bEnd;
    int flag = 0;

    showcursor(FALSE);

    // 전체 무한 루프
    for (;;) {
        time_t start = time(NULL);
        int count = 0;
        int previous;
        int reset = 0;
        // 게임판 초기화
        memcpy(ns, arStage[stage], sizeof(ns));
        for (y = 0; y < 18; y++) {
            for (x = 0; x < 20; x++) {
                // 주인공의 위치 찾아 놓는다.
                if (ns[y][x] == MAN) {
                    nx = x;
                    ny = y;
                    ns[y][x] = EMPTY;
                }
            }
        }
        clrscr();
        putsxy(45, 2, "SOKOBAN");
        putsxy(45, 4, "ESC:종료");
        putsxy(45, 6, "1, 2, 3:스테이지 이동");
        putsxy(45, 8, "경과시간:");
        putsxy(45, 10, "이동횟수:");


        // 게임 진행 루프
        for (;;) {
            // 화면을 그린다.
            for (y = 0; y < 18; y++) {
                for (x = 0; x < 20; x++) {
                    putsxy(x * 2, y, arTile[ns[y][x] - '0']);
                }
            }
            putsxy(nx * 2, ny, arTile[MAN - '0']);

            // 주인공 이동
            ch = getch();
            if (ch == 0xE0 || ch == 0) {
                ch = getch();
                dx = dy = 0;
                switch (ch) {
                case LEFT:
                    getTime(start);
                    getCount(++count);
                    dx = -1;
                    previous = LEFT;
                    break;
                case RIGHT:
                    getTime(start);
                    getCount(++count);
                    dx = 1;
                    previous = RIGHT;
                    break;
                case UP:
                    getTime(start);
                    getCount(++count);
                    dy = -1;
                    previous = UP;
                    break;
                case DOWN:
                    getTime(start);
                    getCount(++count);
                    dy = 1;
                    previous = DOWN;
                    break;
                }

                // 벽이 아니어야 한다.
                if (ns[ny + dy][nx + dx] != WALL) {
                    // 짐을 미는 경우
                    if (ns[ny + dy][nx + dx] == PACK) {
                        // 그 다음 칸이 비어 있거나 창고여야 한다.
                        if (ns[ny + dy * 2][nx + dx * 2] == EMPTY ||
                            ns[ny + dy * 2][nx + dx * 2] == DEPOT) {
                            if (arStage[stage][ny + dy][nx + dx] == DEPOT) {
                                ns[ny + dy][nx + dx] = DEPOT;
                            }
                            else {
                                ns[ny + dy][nx + dx] = EMPTY;
                            }
                            ns[ny + dy * 2][nx + dx * 2] = PACK;
                        }
                        else {
                            dx = dy = 0;
                        }
                    }
                    // 새 위치로 이동
                    nx += dx;
                    ny += dy;
                }
            }
            else {
                if (ch == ESC) {
                    showcursor(TRUE);
                    return;
                }
                if (ch == '1') {
                    stage = 0;
                    break;
                }
                if (ch == '2') {
                    stage = 1;
                    break;
                }
                if (ch == '3') {
                    stage = 2;
                    break;
                }
                if (ch == BACK) {
                    if (flag == 0) {
                        reset = 1;
                        flag++;
                    }
                    else {
                        putsxy(45, 12, "더이상 취소할 수 없습니다.");
                    }
                    
                }
                    
            }

            // 게임 끝 처리
            bEnd = TRUE;
            for (y = 0; y < 18; y++) {
                for (x = 0; x < 20; x++) {
                    if (ns[y][x] == DEPOT) {
                        bEnd = FALSE;
                    }
                }
            }
            if (reset == 1) {
                break;
            }

            if (bEnd) {
                clrscr();
                putsxy(10, 10, "참 잘했어요. 다음 스테이지로 이동합니다.");
                delay(2000);
                stage++;
                if (stage == 3) stage = 0;
                break;
            }
        }
    }
}

void getTime(time_t start) {
    time_t end = time(NULL);
    char str[100];
    sprintf(str, "%d", (int)(end - start));
    putsxy(55, 8, str);
}

void getCount(int count) {
    char counts[100];
    itoa(count, counts, 10);
    putsxy(55, 10, counts);
}