#include "logic.h"
#include "board.h"
#include "pos.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void logic_tests(enum type t){
    printf("%s GAME TESTS:\n", t ? "BITS" : "MATRIX");
    //GAME 1
    game* g1 = new_game(2, 2, 4, 4, t);
    place_piece(g1,make_pos(3,0));
    place_piece(g1,make_pos(3,3));
    board_show(g1->b);
    printf("Test game outcome for IN_PROGRESS: \n"
            "Game outcome should be 0, game outcome is: %d\n\n",
            game_outcome(g1));
    place_piece(g1,make_pos(2,1));
    board_show(g1->b);
    printf("Test game outcome for diagonal left black victory:\n"
            "Winner should be 1, Winner is: %d\n\n",game_outcome(g1));
    place_piece(g1,make_pos(2,3));
    board_show(g1->b);
    printf("Test 2-winner draw, hangtime functionality, and vertical victory:\n"
            "Game outcome should be 3, game outcome is: %d\n\n",
            game_outcome(g1));
    game_free(g1);

    //GAME 2
    game* g2 = new_game(3,0,3,3,t);
    place_piece(g2,make_pos(2,0));
    place_piece(g2,make_pos(1,0));
    place_piece(g2,make_pos(2,1));
    place_piece(g2,make_pos(0,1));
    printf("Hangtime 0 is %sfunctional.\n\n",
            board_get(g2->b,make_pos(1,1))==2 ? "" : "not ");
    place_piece(g2,make_pos(2,2));
    board_show(g2->b);
    printf("Test full row, bottom row victory:\n"
            "Winner should be 1, Winner is: %d\n\n",game_outcome(g2));
    game_free(g2);

    //GAME 3
    game* g3 = new_game(3,10,3,3,t);
    place_piece(g3,make_pos(1,1));
    place_piece(g3,make_pos(0,0));
    place_piece(g3,make_pos(0,1));
    unsigned int piece_good = place_piece(g3,make_pos(0,0));
    piece_good = (!piece_good) && (board_get(g3->b,make_pos(0,1))==WHITE) && 
                 ((board_get(g3->b,make_pos(0,0))==WHITE));
    printf("place_piece does not overwrite, returns false when a piece is "
            "attempted to write over,\nand does not change the turn: %s\n\n", 
            !piece_good ? "true" : "false");
    place_piece(g3,make_pos(0,2));
    place_piece(g3,make_pos(1,2));
    place_piece(g3,make_pos(2,1));
    place_piece(g3,make_pos(2,0));
    place_piece(g3,make_pos(1,0));
    place_piece(g3,make_pos(2,2));
    board_show(g3->b);
    printf("Test full-board victory:\n"
            "Game outcome should be 3, game outcome is: %d\n\n",
            game_outcome(g3));
    game_free(g3);

    //GAME 4
    game* g4 = new_game(3,4,10,3,t);
    place_piece(g4,make_pos(2,4));
    place_piece(g4,make_pos(0,9));
    place_piece(g4,make_pos(0,0));
    place_piece(g4,make_pos(1,8));
    place_piece(g4,make_pos(0,2));
    place_piece(g4,make_pos(0,7));
    board_show(g3->b);
    printf("Test diagonal right victory:\n"
            "Winner should be 2, winner is: %d\n\n",game_outcome(g4));
    place_piece(g4,make_pos(2,7));
    place_piece(g4,make_pos(1,9));
    place_piece(g4,make_pos(0,0));
    place_piece(g4,make_pos(0,9));
    board_show(g3->b);
    printf("Test right column victory:\n"
            "Winner should be 2, winner is: %d\n\n",game_outcome(g4));
    game_free(g4);

    //GAME 5
    game* g5 = new_game(3,10,2,10,t);
    place_piece(g5,make_pos(0,0));
    place_piece(g5,make_pos(2,0));
    place_piece(g5,make_pos(1,1));
    place_piece(g5,make_pos(2,1));
    board_show(g5->b);
    printf("Test game outcome is IN_PROGRESS when diagonal and horizontal "
            "spans do not reach run length.\nGame outcome should be 0. Game "
            "outcome is: %d\n\n",game_outcome(g5));
    place_piece(g5,make_pos(7,0));
    place_piece(g5,make_pos(6,1));
    place_piece(g5,make_pos(8,0));
    place_piece(g5,make_pos(7,1));
    place_piece(g5,make_pos(9,0));
    board_show(g5->b);
    printf("Test vertical victory:\nWinner should be 1. Winner is: %d\n\n",
            game_outcome(g5));
    game_free(g5);
    printf("\n\n");
}

void print_posqueue(posqueue* q){
    pq_entry* curr = q->head;
    if (q->len == 0){
        printf("\n");
    } else {
        while (curr->next != NULL){
            printf("(%d,%d) -> ",curr->p.r,curr->p.c);
            curr = curr->next;
        }
        printf("(%d,%d)\n",curr->p.r,curr->p.c);
    }
}

void position_tests(){
    printf("POSITION TESTS:\n");
    posqueue* q = posqueue_new();
    pos_enqueue(q,make_pos(1,1));
    pos_enqueue(q,make_pos(1,2));
    printf("Check posqueue_new and pos_enqueue: Should be (1,1) -> (1,2): ");
    print_posqueue(q);
    printf("Check posqueue_member: It is %sworking\n",
            (posqueue_member(q,make_pos(1,1)) && 
            (!posqueue_member(q,make_pos(5,5))) ? "" : "not "));
    pos_dequeue(q);
    printf("Check dequeue: Should be (1,2): ");
    print_posqueue(q);
    pos_dequeue(q);
    printf("Check dequeue: Should be Empty list: ");
    print_posqueue(q);
    bool pos_equals_test = pos_equals(make_pos(1,1),make_pos(1,1)) &&
                           !(pos_equals(make_pos(4,2),make_pos(1,0))); 
    printf("Check pos_equals: It is %sworking\n",pos_equals_test ? "" : "not ");
    posqueue_free(q);
    printf("\n\n");
}

void board_tests(enum type t){
    printf("%s BOARD TESTS:\n",t ? "BITS" : "MATRIX");
    board* b = board_new(66,66,t);
    board_set(b,make_pos(0,1),BLACK);
    board_set(b,make_pos(0,3),WHITE);
    board_show(b);
    printf("Test board_set and board_get: (0,0) should be 0, and is: %d\n",
            board_get(b,make_pos(0,0)));
    printf("Test board_set and board_get: (0,1) should be 1, and is: %d\n",
            board_get(b,make_pos(0,1)));
    printf("Test board_set and board_get: (0,3) should be 2, and is: %d\n",
            board_get(b,make_pos(0,3)));
    printf("\n\n");
    free(b);
}

int main(int argc, char* argv[]){
    position_tests();
    board_tests(MATRIX);
    board_tests(BITS);
    logic_tests(MATRIX);
    logic_tests(BITS);
}