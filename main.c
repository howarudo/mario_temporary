#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>

    int getch(void) {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }
#endif
*/

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
    for (int y = 0; y < SCREEN_HEIGHT; ++y){
        for (int x = 0; x < SCREEN_WIDTH; ++x){
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
    DrawScreen();
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
    snprintf(aa['f'], sizeof(aa['f']), "●");
    snprintf(aa['@'], sizeof(aa['@']), "★");

    Init();

    clock_t lastUpdateClock = clock();
    clock_t lastDrawClock = clock();
    while(1){
        clock_t nowClock = clock();
        if(nowClock >= lastUpdateClock + UPDATE_INTERVAL){
            lastUpdateClock = nowClock;
        }

        /*
        if(nowClock >= lastDrawClock + DRAW_INTERVAL){
            lastDrawClock = nowClock;

            player.position.x += 1;
            DrawScreen();
        }
        */

        if(nowClock >= lastDrawClock + DRAW_INTERVAL){
            lastDrawClock = nowClock;

            player.position.x += 1;
            if (player.position.x >= SCREEN_WIDTH) {
                player.position.x = 0;
                }
            DrawScreen();
        }

    }


    return 0;

}
