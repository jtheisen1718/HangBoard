#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include <limits.h>

cell** make_cell_array(int width, int height){
    cell** u = (cell**)malloc(sizeof(cell*)*height);
    if (u == NULL){
        fprintf(stderr,"Unable to allocate memory\n");
        exit(1);
    }
    for (unsigned int r = 0; r < height; r++){
        u[r] = (cell*)malloc(sizeof(cell)*width);
        if (u[r] == NULL){
            fprintf(stderr,"Unable to allocate memory\n");
            exit(1);
        }
        for (unsigned int c = 0; c < width; c++){
            u[r][c] = EMPTY;
        }
    }
    return u;
}

board* board_new(unsigned int width, unsigned int height, enum type type){
    board* b = (board*)malloc(sizeof(board));
    if (b==NULL){
        fprintf(stderr,"Unable to allocate memory\n");
        exit(1);
    }
    b->width = width;
    b->height = height;
    b->type = type;
    switch (type){
        case BITS:{
            unsigned int num_bits = width * height * 2;
            unsigned int CELLS_PER_ELEMENT = 16;
            unsigned int array_len = 1 + (num_bits / CELLS_PER_ELEMENT) +
                                     ((num_bits % CELLS_PER_ELEMENT) ? 0 : 1);
            unsigned int* array = (unsigned int*)malloc(sizeof(unsigned int) * 
                                   array_len);
            for (unsigned int i = 0; i < array_len; i++){
                array[i] = 0;
            }
            b->u.bits = array;
            break;
            }
        case MATRIX:{
            b->u.matrix = make_cell_array(width,height);
            break;}

    }
    return b;
}


void board_free(board* b){
    switch(b->type){
        case BITS:
            free(b->u.bits);
            break;
        case MATRIX:
            for (unsigned int r = 0; r < b->height; r++){
                free(b->u.matrix[r]);
            }
            free(b->u.matrix);
            break;
    }
    free(b);
}

char num_to_label(unsigned int num){
    if (num < 10){
        return (num + 48);
    } else if (num < 36){
        return (num + 55);
    } else if (num < 62){
        return num + 61;
    } else {
        return '?';
    }
}

unsigned int label_to_num(char lab){
    unsigned int l = (unsigned int) lab;
    if (l > 47 && l < 58) {
        return l - 48;
    } else if (l > 64 && l < 91){
        return l - 54;
    } else if (l > 96 && l < 123) {
        return l - 61;
    } else {
        return 63;
    }
}

void board_show(board* b){
    printf("  ");
    for (unsigned int c = 0; c < b->width; c++){
        printf("%c",num_to_label(c));
    }
    printf("\n\n");
    for (unsigned int r = 0; r < b->height; r++){
        printf("%c ",num_to_label(r));
        for (unsigned int c = 0; c < b->width; c++){
            switch (board_get(b,make_pos(r,c))){
                case EMPTY:
                    printf(".");
                    break;
                case BLACK:
                    printf("*");
                    break;
                case WHITE:
                    printf("o");
                    break;
            }
        }
        printf("\n");
    }
}

unsigned int get_index(board* b, pos p){
    return p.r*b->width + p.c;
}

cell board_get(board* b, pos p){
    if (b->height < p.r){
        fprintf(stderr,"board_get: p.r out of bounds (p.r = %d)\n",p.r);
        exit(1);
    }
    if (b->width < p.c){
        fprintf(stderr,"board_get: p.c out of bounds (p.c = %d)\n",p.c);
        exit(1);
    }
    switch (b->type){
        case BITS: {
            unsigned int CELLS_PER_ELEMENT = 16, index = get_index(b,p);
            unsigned int element = index / CELLS_PER_ELEMENT;
            unsigned int cell_shift = index % CELLS_PER_ELEMENT;
            unsigned int shifted_element = b->u.bits[element] >> (cell_shift*2);
            switch (shifted_element & 3){
                case 0:
                    return EMPTY;
                case 1:
                    return BLACK;
                case 2:
                    return WHITE;
                default:
                    fprintf(stderr,"Invalid output blah\n");
                    exit(1);
            }
        }
        case MATRIX:
            return b->u.matrix[p.r][p.c];
    }
}

unsigned int clear_change(unsigned int element, cell c, unsigned int bit_shift){
    unsigned int ones = UINT_MAX;
    unsigned int offset = 3;
    offset = offset << bit_shift;
    unsigned int clear_index = ones ^ offset;
    unsigned int cleared =  element & clear_index;
    switch (c){
        case EMPTY:
            return cleared;
        case BLACK:
            return (cleared | (1 << bit_shift));
        case WHITE:
            return (cleared | (2 << bit_shift));
    }
}

void board_set(board* b, pos p, cell c){
    if (b->height < p.r){
        fprintf(stderr,"board_set: p.r out of bounds (p.r = %d)\n",p.r);
        exit(1);
    }
    if (b->width < p.c){
        fprintf(stderr,"board_set: p.c out of bounds (p.c = %d)\n",p.c);
        exit(1);
    }
    switch (b->type){
        case BITS:{
            unsigned int CELLS_PER_ELEMENT = 16, index = get_index(b,p);
            unsigned int element = index / CELLS_PER_ELEMENT;
            unsigned int cell_shift = index % CELLS_PER_ELEMENT;
            b->u.bits[element] = clear_change(b->u.bits[element],c,
                                              cell_shift*2);
            break;
        }
        case MATRIX:
            b->u.matrix[p.r][p.c] = c;
            break;
    }
}