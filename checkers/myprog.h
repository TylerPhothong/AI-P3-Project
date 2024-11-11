#ifndef COMPUTER_H
#define COMPUTER_H

#define Empty 0x00
#define Piece 0x20
#define King 0x60
#define Red 0x00
#define White 0x80

#define number(x) ((x) & 0x1f)
#define empty(x) ((((x) >> 5) & 0x03) == 0 ? 1 : 0)
#define piece(x) ((((x) >> 5) & 0x03) == 1 ? 1 : 0)
#define king(x) ((((x) >> 5) & 0x03) == 3 ? 1 : 0)
#define color(x) ((((x) >> 7) & 1) + 1)

#define Clear 0x1f

struct State {
    int player;
    char board[8][8];
    char movelist[48][12];
    int numLegalMoves;
};

void CopyState(char *dest, char src);
void ResetBoard(void);
void AddMove(char move[12]);
void FindKingMoves(char board[8][8], int x, int y);
void FindMoves(int player, char board[8][8], int x, int y);
void AddJump(char move[12]);
int FindKingJump(int player, char board[8][8], char move[12], int len, int x, int y);
int FindJump(int player, char board[8][8], char move[12], int len, int x, int y);
int FindLegalMoves(struct State *state);
int Evaluate(struct State *state);
int Minimax(struct State *state, int depth, int alpha, int beta, int maximizingPlayer);
void FindBestMove(int player);
void NumberToXY(char num, int *x, int *y);
int MoveLength(char move[12]);
int TextToMove(char *mtext, char move[12]);
void MoveToText(char move[12], char *mtext);
void PerformMove(char board[8][8], char move[12], int mlen);

#endif
