#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#include <time.h>
#include <limits.h>
#include "myprog.h"

#ifndef CLK_TCK
#define CLK_TCK CLOCKS_PER_SEC
#endif

float SecPerMove;
char board[8][8];
char bestmove[12];
int me, cutoff, endgame;
long NumNodes;
int MaxDepth;

/*** For timing ***/
clock_t start;
struct tms bff;

/*** For the jump list ***/
int jumpptr = 0;
int jumplist[48][12];

/*** For the move list ***/
int numLegalMoves = 0;
int movelist[48][12];

/* Print the amount of time passed since my turn began */
void PrintTime(void) {
    clock_t current;
    float total;
    current = times(&bff);
    total = (float) ((float)current - (float)start) / CLK_TCK;
    fprintf(stderr, "Time = %f\n", total);
}

/* Determine if I'm low on time */
int LowOnTime(void) {
    clock_t current;
    float total;
    current = times(&bff);
    total = (float) ((float)current - (float)start) / CLK_TCK;
    return total >= (SecPerMove - 1.0);
}

/* Evaluation function to score the board based on pieces */
int Evaluate(struct State *state) {
    int score = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (piece(state->board[y][x])) {
                if (color(state->board[y][x]) == me)
                    score += 5;
                else
                    score -= 5;
            }
            if (king(state->board[y][x])) {
                if (color(state->board[y][x]) == me)
                    score += 10;
                else
                    score -= 10;
            }
        }
    }
    return score;
}

/* Minimax with Alpha-Beta Pruning */
int Minimax(struct State *state, int depth, int alpha, int beta, int maximizingPlayer) {
    if (depth == 0 || state->numLegalMoves == 0) return Evaluate(state);

    int bestScore;
    if (maximizingPlayer) {
        bestScore = INT_MIN;
        for (int i = 0; i < state->numLegalMoves; i++) {
            struct State newState = *state;
            PerformMove(newState.board, newState.movelist[i], MoveLength(newState.movelist[i]));
            FindLegalMoves(&newState);
            int eval = Minimax(&newState, depth - 1, alpha, beta, 0);
            bestScore = (eval > bestScore) ? eval : bestScore;
            alpha = (alpha > eval) ? alpha : eval;
            if (beta <= alpha) break;
        }
    } else {
        bestScore = INT_MAX;
        for (int i = 0; i < state->numLegalMoves; i++) {
            struct State newState = *state;
            PerformMove(newState.board, newState.movelist[i], MoveLength(newState.movelist[i]));
            FindLegalMoves(&newState);
            int eval = Minimax(&newState, depth - 1, alpha, beta, 1);
            bestScore = (eval < bestScore) ? eval : bestScore;
            beta = (beta < eval) ? beta : eval;
            if (beta <= alpha) break;
        }
    }
    return bestScore;
}

/* Finds the best move by applying Minimax with Alpha-Beta pruning */
void FindBestMove(int player) {
    struct State state;
    state.player = player;
    memcpy(state.board, board, 64 * sizeof(char));
    FindLegalMoves(&state);

    int bestScore = INT_MIN;
    int bestMoveIndex = -1;
    for (int i = 0; i < state.numLegalMoves; i++) {
        struct State newState = state;
        PerformMove(newState.board, newState.movelist[i], MoveLength(newState.movelist[i]));
        int score = Minimax(&newState, MaxDepth, INT_MIN, INT_MAX, 0);
        if (score > bestScore) {
            bestScore = score;
            bestMoveIndex = i;
        }
    }
    if (bestMoveIndex != -1)
        memcpy(bestmove, state.movelist[bestMoveIndex], MoveLength(state.movelist[bestMoveIndex]));
}

/* Main function */
int main(int argc, char *argv[]) {
    char buf[1028], move[12];
    int len, mlen, player1;
    SecPerMove = atof(argv[1]);
    MaxDepth = (argc == 4) ? atoi(argv[3]) : -1;

    /* Determine if I am Player1 (Red) or Player2 (White) */
    len = read(STDIN_FILENO, buf, 1028);
    buf[len] = '\0';
    player1 = (strncmp(buf, "Player1", strlen("Player1")) == 0) ? 1 : 0;
    me = player1 ? 1 : 2;

    ResetBoard();
    srand((unsigned int)time(0));

    if (player1) {
        start = times(&bff);
        goto determine_next_move;
    }

    for (;;) {
        len = read(STDIN_FILENO, buf, 1028);
        buf[len] = '\0';
        start = times(&bff);
        memset(move, 0, 12 * sizeof(char));
        mlen = TextToMove(buf, move);
        PerformMove(board, move, mlen);

determine_next_move:
        FindBestMove(me);
        if (bestmove[0] != 0) {
            mlen = MoveLength(bestmove);
            PerformMove(board, bestmove, mlen);
            MoveToText(bestmove, buf);
        } else {
            exit(1);
        }

        write(STDOUT_FILENO, buf, strlen(buf));
        fflush(stdout);
    }

    return 0;
}
