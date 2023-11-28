#include <stdio.h>
#include<curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int mygetch() {
    char ch;
    int error;
    static struct termios Otty, Ntty;

    fflush(stdout);
    tcgetattr(0, &Otty);
    Ntty = Otty;

    Ntty.c_iflag  =  0;     /* input mode       */
    Ntty.c_oflag  =  0;     /* output mode      */
    Ntty.c_lflag &= ~ICANON;    /* line settings    */

#if 1
    /* disable echoing the char as it is typed */
    Ntty.c_lflag &= ~ECHO;  /* disable echo     */
#else
    /* enable echoing the char as it is typed */
    Ntty.c_lflag |=  ECHO;  /* enable echo      */
#endif

    Ntty.c_cc[VMIN]  = CMIN;    /* minimum chars to wait for */
    Ntty.c_cc[VTIME] = CTIME;   /* minimum wait time    */

#if 1
    /*
    * use this to flush the input buffer before blocking for new input
    */
    #define FLAG TCSAFLUSH
#else
    /*
    * use this to return a char from the current input buffer, or block if
    * no input is waiting.
    */
    #define FLAG TCSANOW

#endif

    if ((error = tcsetattr(0, FLAG, &Ntty)) == 0) {
        error  = read(0, &ch, 1 );        /* get char from stdin */
        error += tcsetattr(0, FLAG, &Otty);   /* restore old settings */
    }

    return (error == 1 ? (int) ch : -1 );
}
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

char aa[256][2 + 1];

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

        if (kbhit()) {
            switch(getch()) {
                printf("hello");
            }
        }
    }

    return 0;

}
