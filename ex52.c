// Created by omer on 19/06/18.
//
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define STDERR_FD 2
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'
#define MOVE_DOWN 's'
#define CHANGE_POSE 'w'
#define QUIT 'q'
#define BOARD_SIZE 20
#define VERTICAL 0
#define HORIZONTAL 1
typedef struct GameEnvironment{
    char mainBoard[BOARD_SIZE][BOARD_SIZE];
    int j_shape;
    int i_shape;
    int posOfShape;
}GameEnvironment;
//function decleration .
void moveShape();
//Global
GameEnvironment gameEnvironment;
/**
 * check if we're out the bounds .
 * @return 1 if we're out the bounds, else 0.
 */
int checkBounds() {
    if (gameEnvironment.posOfShape == HORIZONTAL) {
        if (gameEnvironment.j_shape + 2 >= BOARD_SIZE - 2) {
            return 1;
        }
    }
    if (gameEnvironment.j_shape - 1< 1) {
        return 1;
    }
    if (gameEnvironment.posOfShape == VERTICAL) {
        if (gameEnvironment.j_shape >= BOARD_SIZE - 1) {
            return 1;
        }

    }
    return 0;
}
/**
 * deleting currnet shape.
 */
void deleteShape() {
    gameEnvironment.mainBoard[gameEnvironment.i_shape][gameEnvironment.j_shape] = ' ';
    if(gameEnvironment.posOfShape ==  HORIZONTAL) {
        gameEnvironment.mainBoard[gameEnvironment.i_shape][gameEnvironment.j_shape+1] = ' ';
        gameEnvironment.mainBoard[gameEnvironment.i_shape][gameEnvironment.j_shape + 2] = ' ';
    } else if(gameEnvironment.posOfShape == VERTICAL) {
        gameEnvironment.mainBoard[gameEnvironment.i_shape + 1][gameEnvironment.j_shape] = ' ';
        gameEnvironment.mainBoard[gameEnvironment.i_shape + 2][gameEnvironment.j_shape] = ' ';
    }
}
/**
 * change pose of  the shape .
 */
void changePose(){
    int currI = gameEnvironment.i_shape;
    int currJ = gameEnvironment.j_shape;
    deleteShape();
    if (gameEnvironment.posOfShape == HORIZONTAL) {
        gameEnvironment.j_shape = currJ +1;
        gameEnvironment.i_shape = currI -1;
        gameEnvironment.posOfShape = VERTICAL;
        moveShape(1);
    } else {
        gameEnvironment.posOfShape = HORIZONTAL;

        if (currJ < 2) {
            gameEnvironment.j_shape = 1;
        } else if (currJ + 4 >= BOARD_SIZE) {
            gameEnvironment.j_shape = BOARD_SIZE -4;
        } else {
            gameEnvironment.j_shape = currJ - 1;
            gameEnvironment.i_shape = currI + 1;
        }

    }


}
/**
 * create New Board .
 * @param gameEnvironment
 */
void createNewBoard() {
    int i,j;
    for (j = 0 ; j < BOARD_SIZE; j++) {
        gameEnvironment.mainBoard[BOARD_SIZE - 1] [j] = '*';
    }
    for (i = 0; i < BOARD_SIZE - 1; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if ((j==0) || (j == BOARD_SIZE -1)){
                gameEnvironment.mainBoard[i][j] = '*';
            } else {
                gameEnvironment.mainBoard[i][j] = ' ';
            }

        }
    }
}
/**
 *
 * print the board .
 */
void printGame() {
    system("clear");
    int i,k ;
    for (i = 0; i < BOARD_SIZE; i++) {
        for ( k = 0; k < BOARD_SIZE; k++) {
            printf("%c",gameEnvironment.mainBoard[i][k]);
        }
        printf("\n");
    }
}
/**
 * initiliaze the game environment .
 */
void initGame() {
    createNewBoard() ;
    gameEnvironment.posOfShape = HORIZONTAL;
    gameEnvironment.i_shape = 0;
    gameEnvironment.j_shape = BOARD_SIZE / 2 -1;
    moveShape();

}
/**
 * move the shape down.
 * @param posChanged 1 if the pose of the shape was changed right now so we don't need to delete the shape,
 * else 0.
 */
void moveShape(int posChanged) {
    if (!posChanged) {
        deleteShape();
    }
    gameEnvironment.i_shape = gameEnvironment.i_shape +1;
    //check Bounds;
    if (gameEnvironment.j_shape <= 0) {
        return;
    }
    if(gameEnvironment.posOfShape == VERTICAL) {
        if (gameEnvironment.j_shape >= BOARD_SIZE - 1) {
            return;
        }
        //put the shape in new location in gameEnvironment.
        gameEnvironment.mainBoard[gameEnvironment.i_shape ][gameEnvironment.j_shape] = '-';
        gameEnvironment.mainBoard[gameEnvironment.i_shape + 1][gameEnvironment.j_shape] = '-';
        gameEnvironment.mainBoard[gameEnvironment.i_shape + 2][gameEnvironment.j_shape] = '-';
        if (gameEnvironment.i_shape + 2 >= BOARD_SIZE -1) {
            initGame();
        }
    } else if(gameEnvironment.posOfShape == HORIZONTAL){
        if (gameEnvironment.j_shape + 2 >= BOARD_SIZE) {
            return;
        }
        gameEnvironment.mainBoard[gameEnvironment.i_shape ][gameEnvironment.j_shape] = '-';
        gameEnvironment.mainBoard[gameEnvironment.i_shape ][gameEnvironment.j_shape+ 1] = '-';
        gameEnvironment.mainBoard[gameEnvironment.i_shape][gameEnvironment.j_shape + 2] = '-';
        if (gameEnvironment.i_shape >= BOARD_SIZE -1) {
            initGame();
        }
    }

}
/**
 *move One Step the shape and update the game evironment and priniting.
 * @param sig
 */
void moveOneStep (int sig) {
    signal(SIGALRM,moveOneStep);
    alarm(1);
    moveShape(0);
    printGame();

}
/**
 *reading the char and act according it .
 * @param sig .
 */
void getCharFromPipe(int sig) {
signal(SIGUSR2 ,getCharFromPipe );
    //if we're not out the bounds
    char c = (char)getchar();
    if (c == QUIT) {
        exit(1);
    }
    if (!checkBounds()) {
    switch (c) {
        case QUIT:
            exit(1);
        case MOVE_RIGHT:
            deleteShape();
            gameEnvironment.j_shape = gameEnvironment.j_shape + 1;
            moveShape(0);
            break;
        case MOVE_LEFT:
            deleteShape();
            gameEnvironment.j_shape = gameEnvironment.j_shape - 1;
            moveShape(0);
            break;
        case MOVE_DOWN:
            deleteShape();
            gameEnvironment.i_shape = gameEnvironment.i_shape + 1;
            moveShape(0);
            break;
        case CHANGE_POSE:
            changePose();
            break;

    }
    } else {
        deleteShape();
        moveShape(0);
    }
    printGame();

}


/**
 *main.
 * @return
 */
int main() {
    initGame();
    printGame();
    signal(SIGALRM,moveOneStep);
    alarm(1);
    signal(SIGUSR2, getCharFromPipe);
    while (1) {
        pause();
    }


}
