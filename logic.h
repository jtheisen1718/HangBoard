#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "board.h"

enum turn {
    BLACKS_TURN,
    WHITES_TURN
};

typedef enum turn turn;


enum outcome {
    IN_PROGRESS,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;


struct game {
    unsigned int run, hangtime;
    board* b;
    posqueue* hanging;
    turn player;
};

typedef struct game game;

/* creates a new game with the specified size and parameters 
   and using the desired data representation */
game* new_game(unsigned int run, unsigned int hangtime, unsigned int width,
               unsigned int height, enum type type);

/* Entirely deallocates a game */
void game_free(game* g);

/* drops the piece in game g at position p a spots */
void falls_by(game* g, pos p, unsigned int a);

/* Checks if the game g has a horizontal or vertical (indicated by the boolean 
   horizontal) run that starts at position (r,c). Curr is a pointer to the value
   of */
bool straight_run(game* g, cell* curr, unsigned int r, unsigned int c, 
                  bool horizontal);

/* Checks if the game g has a diagonal left or right (indicated by the boolean 
   left) run that starts at position (r,c). Curr is a pointer to the value
   of */
bool diagonal_run(game*g, cell* curr, unsigned int r, unsigned int c,
                  bool left);
/* places a piece belonging to the player whose turn it is at the specified 
   coordinates. If the spot is already taken, it does nothing and returns false.
   otherwise it returns true. Also simulates gravity and changes the turn to the
   next player. */
bool place_piece(game* g, pos p);

/* reports the outcome of a completed game, or that the game is still in 
   progress. */
outcome game_outcome(game* g);

#endif /* LOGIC_H */
