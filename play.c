#include "logic.h"
#include "board.h"
#include "pos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void take_input(game* g, unsigned int* row, unsigned int* col){
    printf("\n%s: ", g->player ? "White" : "Black");
    char rowc, colc;
    scanf("%c%c%*c",&rowc,&colc);
    *row = label_to_num(rowc);
    *col = label_to_num(colc);
    printf("----------\n\n");
}

int main(int argc, char* argv[]){
    unsigned int parameters_len = 5;
    int parameters[5];
    if (argc > 10){
        fprintf(stderr,"main: too many arguments provided (argc): %d\n", argc);
        exit(1);
    }
    for (unsigned int i = 0; i < parameters_len; i++){
        parameters[i] = -1;
    }
    for (unsigned int i = 1; i < argc; i++){
        if (!strcmp(argv[i],"-h")){
            parameters[3] = atoi(argv[i+1]);
        } else if (!strcmp(argv[i],"-w")) {
            parameters[2] = atoi(argv[i+1]);
        } else if (!strcmp(argv[i],"-r")) {
            parameters[0] = atoi(argv[i+1]);
        } else if (!strcmp(argv[i],"-t")) {
            parameters[1] = atoi(argv[i+1]);
        } else if (!strcmp(argv[i],"-m")){
            parameters[4] = MATRIX;
        } else if (!strcmp(argv[i],"-b")){
            parameters[4] = BITS;
        }
    }
    for (unsigned int i = 0; i < parameters_len; i++){
        if (parameters[i] == -1){
            char* error;
            switch (i){
                case 0:
                    error = "-h";
                    break;
                case 1:
                    error = "-w";
                    break;
                case 2:
                    error = "-r";
                    break;
                case 3:
                    error = "-t";
                    break;
                case 4:
                    error = "(-b or -m)";
                    break;
            }
            fprintf(stderr, "main: %s flag missing\n",error);
            exit(1);
        }
    }
    game* g = new_game(parameters[0],parameters[1],parameters[2],parameters[3],
                       parameters[4]);
    printf("\n");
    while (game_outcome(g) == IN_PROGRESS){
        board_show(g->b);
        unsigned int row, col, UPPER_BOUND = 63;
        bool pp;
        do {
            take_input(g,&row,&col);
            while  (row == UPPER_BOUND || col == UPPER_BOUND) { 
                if (row == UPPER_BOUND && col == UPPER_BOUND) {
                    printf("Row and column entries are invalid, try again.\n");
                } else if (row == UPPER_BOUND) {
                    printf("Row entry is invalid, try again.\n");
                } else if (col == UPPER_BOUND){
                    printf("Column entry is invalid, try again.\n");
                }
                take_input(g,&row,&col);
            }
            pp = place_piece(g,make_pos(row,col));
            if (!pp) {
                printf("That location is already occupied, try again.\n");
            }
        } while (!pp);
    }
    board_show(g->b);
    switch (game_outcome(g)) {
        case 0:
            fprintf(stderr,"main: invalid game outcome\n");
            exit(1);
        case 1:
            printf("\nBLACK WINS!!\n\n");
            break;
        case 2:
            printf("WHITE WINS!!\n");
            break;
        case 3:
            printf("DRAW!!\n");
            break;
    }
}