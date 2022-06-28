#ifndef BOARD_H
#define BOARD_H

#include "pos.h"


enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;


union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};


struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;


/* creates a fully-empty board of the desired size */
board* board_new(unsigned int width, unsigned int height, enum type type);

/* fully frees the board */
void board_free(board* b);

/*  */
char num_to_label(unsigned int num);

/*  */
unsigned int label_to_num(char lab);

/* prints the board to the screen, along with row and column headers */
void board_show(board* b);

/* retrieves a cell within the board */
cell board_get(board* b, pos p);

/* modifies a cell within the board */
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
