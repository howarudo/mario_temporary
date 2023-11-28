#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>//標準ライブラリではないから、環境依存の可能性あり


#define COURSE_WIDTH    (256)
#define COURSE_HEIGHT   (16)

#define SCREEN_WIDTH    (16)
#define SCREEN_HEIGHT   (16)

#define UPDATE_FPS      (60)
#define UPDATE_INTERVAL (1000 / UPDATE_FPS)

#define DRAW_FPS        (10)
#define DRAW_INTERVAL   (1000 / DRAW_FPS)

typedef struct {
    float x, y;
}VEC2;

typedef struct {
    VEC2 position;
    VEC2 velocity;
    bool isJumping; // プレイヤーがジャンプ中かどうかを追跡する変数　マリオがジャンプするロジックを作るときにこのフラグ管理が必要
}PLAYER;

char course[COURSE_HEIGHT][COURSE_WIDTH] = {
"                                                                                                                                                                                                                   ",
"                                                                                                                                                                                                                   ",
"                                                                                                                                                                                                                   ",
"                   ccc              cccc                           ccc              cccc                           ccc              cccc                           ccc              cccc              g            ",
"                   ccc     ccccc    cccc                ccc        ccc              cccc                ccc        ccc     ccccc    cccc                ccc        ccc     ccccc    cccc              g ccc        ",
"                           ccccc                        ccc                                             ccc                ccccc                        ccc                ccccc                      g ccc        ",
"                      q                                                         bbbbbbbb   bbbq              q           bbb    bqqb                                                        bb        g            ",
"                                                                                                                                                                                           bbb        g            ",
"                                                                                                                                                                                          bbbb        g            ",
"                                                                                                                                                                                         bbbbb        g    fff     ",
"                q   bqbqb                     pp         pp                  bqb              b     bb    q  q  q     b          bb      b  b          bb  b            bqbqb           bbbbbb        g    fff     ",
"  m                                   pp      pp  m      pp                                       m                                     bb  bb    m   bbb  bb                          bbbbbbb    m   g   fffff    ",
" mmm                        pp        pp      pp mmm     pp      m                               mmm                                   bbb  bbb  mmm bbbb  bbb   m pp              pp bbbbbbbb   mmm  g   fffff  m ",
"mmmmm      tttttmmm    ttt  pp        pp tttt ppmmmmm    pptttttmmm    ttt               tttt   mmmmm      tttttmmm    ttt            bbbbttbbbbmmmmbbbbb  bbbbtmmmpp  ttt         ppbbbbbbbbb  mmmmm b   ffffftmmm",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
};

char screen[SCREEN_HEIGHT][SCREEN_WIDTH];

char aa[256][4 + 1];

PLAYER player;

bool keyPressed[256];

void ClearScreen() {
    #ifdef _WIN32
    // Windows用のコマンド
    system("cls");
    #else
    // macOSやLinux用のコマンド
    system("clear");
    #endif
}

void DrawScreen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++){
        for (int x = 0; x < SCREEN_WIDTH; x++){
            screen[y][x] = course[y][x];
        }
    }
    {
        int x = (int)roundf(player.position.x);
        int y = (int)roundf(player.position.y);

        /*
        if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        screen[y][x] = '@';
        }
        */
        screen[y][x] = '@';
    }
    ClearScreen();
    for (int y = 0; y < SCREEN_HEIGHT; ++y){
        for (int x = 0; x < SCREEN_WIDTH; ++x){
            printf("%s", aa[screen[y][x]]);
        }
        printf("\n");
    }

}

void Init(){
    player.position.x = SCREEN_WIDTH / 2;
    player.position.y = 13;
    memset(keyPressed, 0, sizeof keyPressed);
    DrawScreen();
}

bool IntersectCourse(VEC2 _v) {
    int x = (int)_v.x;
    int y = (int)_v.y;
    switch(course[y][x]){
        case'b':
            if (player.isJumping && player.velocity.y > 0) {) {
                course[y][x] = ' '; // ブロックを空白に変更
                //return false; // 衝突せず、通過させる
            }
        case'p':
        case'q':
            return ture;
    }
    return false;
}

int main() {
/*
    //OSによる分岐
#ifdef _WIN32
    _getch();
#else
    getch();
#endif
*/
    //ここからがゲームのコード
    snprintf(aa[0], sizeof(aa[0]), "x");
    snprintf(aa[' '], sizeof(aa[' ']), " ");
    snprintf(aa['b'], sizeof(aa['b']), "■");
    snprintf(aa['p'], sizeof(aa['p']), "□");
    snprintf(aa['q'], sizeof(aa['q']), "？");
    snprintf(aa['m'], sizeof(aa['m']), "^");
    snprintf(aa['t'], sizeof(aa['t']), "Y");
    snprintf(aa['c'], sizeof(aa['c']), "~");
    snprintf(aa['g'], sizeof(aa['g']), "|");
    snprintf(aa['f'], sizeof(aa['f']), "田");
    snprintf(aa['@'], sizeof(aa['@']), "★");

    Init();

    clock_t lastUpdateClock = clock();
    clock_t lastDrawClock = clock();
    while(1){
        clock_t nowClock = clock();
            VEC2 left = {
                player.position.x,
                player.position.y + 0.5ff
            };
            if (IntersectCourse(keft)) {
                player.position.x = floorf(player.position.x) + 1.0f;
                player.velocity.x = 0;
            };
            VEC2 right = {
                player.position.x + 1.0f,
                player.position.y + 0.5ff
            };
            if (IntersectCourse(right)) {
                player.position.x = floorf(player.position.x);
                player.velocity.x = 0;
            };
            VEC2 down = {
                player.position.x + 0.5f,
                player.position.y + 1.0f
            };
            if(IntersectCourse(down)) {
                player.position.y = floorf(player.position.y);
                player.velocity.y = 0;
            };
            VEC2 up = {
                player.position.x + 0.5f,
                player.position.y
            };
            if(IntersectCourse(up)) {
                player.position.y = floorf(player.position.y) + 1.0f;
                player.velocity.y = 0;
            };

        if(nowClock >= lastUpdateClock + UPDATE_INTERVAL){
            lastUpdateClock = nowClock;
        }

        if(nowClock >= lastDrawClock + DRAW_INTERVAL){
            lastDrawClock = nowClock;

            // player.position.x += 1;
            // if (player.position.x >= SCREEN_WIDTH) {
            //     player.position.x = 0;
            //     }
            DrawScreen();
        }
        /*
        if (kbhit()) {//kbhitはconio.h内の関数
            switch(getch()) {
                printf("hello");
            }
        }
        */
    }

    return 0;

}
