#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "board.h"
#include "pos.h"

game* new_game(unsigned int run, unsigned int hangtime, unsigned int width,
               unsigned int height, enum type type){
    if (((run > height) && (run > width)) || (run <= 1)){
        fprintf(stderr,"new_game: Invalid run size (run): %d\n",run);
        exit(1);
    }

    if (height < 1){
        fprintf(stderr,"new_game: Invalid height (height): %d\n",height);
        exit(1);
    }

    if (width < 1){
        fprintf(stderr,"new_game: Invalid width (width): %d\n",width);
        exit(1);
    }
    
    game* g = (game*)malloc(sizeof(game));
    if (g==NULL){
        fprintf(stderr,"Unable to allocate memory");
        exit(1);
    }
    g->b = board_new(width, height, type);
    g->hanging = posqueue_new();
    g->hangtime = hangtime;
    g->player = BLACKS_TURN;
    g->run = run;
    return g;
}

void game_free(game* g){
    board_free(g->b);
    posqueue_free(g->hanging);
    free(g);
}

void falls_by(game* g, pos p, unsigned int a){
    if ((board_get(g->b,p) != EMPTY) && !posqueue_member(g->hanging,p)){
        pos newp = make_pos(p.r+a,p.c);
        board_set(g->b,newp,board_get(g->b,p));
        board_set(g->b,p,EMPTY);
    }
}

bool place_piece(game* g, pos p){
    if (g->b->height < p.r || g->b->width < p.c){
        return false;
    }
    if (board_get(g->b,p) == EMPTY){
        //Place Current Piece
        board_set(g->b,p,g->player+1);
        pos_enqueue(g->hanging,p);
        
        //Gravity
        if (g->hanging->len > g->hangtime){
            pos target = pos_dequeue(g->hanging);
            pos under_target = make_pos(target.r + 1, target.c); 
            unsigned int drop = 0;


            while ((under_target.r < g->b->height) &&
                   board_get(g->b,under_target) == EMPTY){
                drop++;
                under_target.r++;
            }
            if (drop > 0){
                for (unsigned int i = 0; i <= target.r; i++){
                    falls_by(g,make_pos(target.r-i,target.c),drop);
                }
            }
        }
        
        //Switch Player
        g->player = g->player == 1 ? 0 : 1;
        return true;
    } else {
        return false;
    }
}

bool run(game* g, cell* curr, unsigned int r, unsigned int c, unsigned int dr, 
         unsigned int dc){
    int next_r, next_c;
    for (unsigned int i = 0; i < g->run; i++){
        next_r = r+i*dr, next_c = c+i*dc;
        if (next_r >= g->b->height || next_c >= g->b->width || next_c < 0){
            return false;
        }
        if (board_get(g->b,make_pos(next_r, next_c)) != *curr){
            return false;
        }
    }
    return true;
}

outcome cell_to_outcome(cell c){
    switch(c){
        case EMPTY:
            return IN_PROGRESS;
        case BLACK:
            return BLACK_WIN;
        case WHITE:
            return WHITE_WIN;
    }
}

outcome game_outcome(game* g){
    cell curr;
    unsigned int non_empty = 0;
    outcome winner = IN_PROGRESS;
    for (unsigned int r = 0; r < g->b->height; r++){
        for (unsigned int c = 0; c < g->b->width; c++){
            curr = board_get(g->b,make_pos(r,c));
            if (curr != EMPTY){
                non_empty++;
                if (run(g,&curr,r,c,0,1) || run(g,&curr,r,c,1,0) || 
                    run(g,&curr,r,c,1,1) || run(g,&curr,r,c,1,-1)){
                    if (winner == cell_to_outcome(curr) || 
                        winner == IN_PROGRESS){
                        winner = cell_to_outcome(curr);
                    } else {
                        return DRAW;
                    }
                }
            }
        }
    }
    if (non_empty == g->b->height*g->b->width){
        return DRAW;
    } else {
        return winner;
    }
}