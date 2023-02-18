#include <stdio.h>
#include <stdlib.h>

#define AB_SIZE 5 // Alpha Beta Size, they will both never have more than 5
#define COLUMN_SIZE 7
#define ROW_SIZE 5
#define FREE_SIZE 35 // just column * row

typedef struct { 
     int x;
     int y;
} Set;

/*  Checks if set contains a certain pair
    @param set - set to check
    @param toFind - the pair to find
    @param nSize - size of Set *set 
    @return returns index where the pair toFind is found in set, return -1 if not found
*/
int contains(Set *set, int toFind[2], int nSize)
{
     int i; // index
     for(i = 0; i < nSize; i++) {
        if(set[i].x == toFind[0] && set[i].y == toFind[1]) 
          return i;
     }
     return -1;
}

/*  Remove a pair from a set
    @param set - set to remove from
    @param toRemove- the pair to remove
    @param nSize - size of Set *set 
*/
void removeSet(Set *set, int toRemove[2], int nSize) {
     int i; // index
     for(i = contains(set, toRemove, nSize); i < nSize - 1; i++) {
          set[i].x = set[i + 1].x;
          set[i].y = set[i + 1].y;
     }
     set[nSize - 1].x = 0;
     set[nSize - 1].y = 0; 
}

/*  Replace a pair with another pair from a set
    @param set - set to replace a pair from
    @param toRemove - the pair to remove
    @param toAdd - the pair to add
    @param nSize - size of Set *set 
*/
void replaceSet(Set *set, int toRemove[2], int toAdd[2], int nSize) 
{
     int i; // index
     for(i = 0; i < nSize; i++) {
          if(set[i].x  == toRemove[0] && set[i].y == toRemove[1]) {
               set[i].x = toAdd[0];
               set[i].y = toAdd[1];
          }
     }
}

/*  Display the board
    @param alpha - the current state of set alpha
    @param beta - the current state of set beta
*/
void displayBoard(Set *alpha, Set *beta) 
{
     // Board's columns, board's rows, arr is where we store the pair to send into contains()
     int column, row, arr[2];
     printf("\n    1   2   3   4   5");     
     for(column = 1; column <= COLUMN_SIZE; column++) {
          printf("\n  +---+---+---+---+---+\n%d |", column);
          for(row = 1; row <= ROW_SIZE; row++) {
               arr[0] = column;
               arr[1] = row;
               if(contains(alpha, arr, AB_SIZE) > -1)
                    printf(" O |");
               else if(contains(beta, arr, AB_SIZE) > -1)
                    printf(" X |");
               else
                    printf("   |");
          }

     }
     printf("\n  +---+---+---+---+---+\n\n");
}

/*  Process the player's next move, give prompts if input is wrong and change values if they are correct
    @param beta - the current state of set beta
    @param alpha - the current state of set alpha
    @param nFree - the current state of set nFree
    @param aTurn - indicates which player's turn it is
    @param ok - checks if moves made are valid
    @param prev - coordinates of where the piece chosen currently is
    @param prev - coordinates of where the player wants to move the piece to
*/
void nextPlayerMove(Set *beta, Set *alpha, Set *nFree, int *aTurn, int *ok, int *prev, int *next)
{     
     // Conditional 1-4 are for when pieces are being moved to free blocks

     /* Conditional 1: 
          Conditions: It is player 1's turn, their piece to move is occupied by their piece, their next 
               move goes up one column, their next move is either in the same row or beside that row. 
          If conditions are met: Signal that the move is valid. */
     if(*aTurn == 1 && contains(alpha, prev, AB_SIZE) != -1 && prev[0] == (next[0] + 1) && 
          (prev[1] == next[1] || next[1] == (prev[1]+1) || prev[1] == (next[1]+1))) {
               *ok = !*ok;
     } else if(*aTurn == 1) { //Make own prompts in case of wrong user input
          if(contains(alpha, prev, AB_SIZE) == -1)
               printf("\nInvalid Move! You do not have a piece at (%d, %d)\n", prev[0], prev[1]);
          if(prev[0] != (next[0] + 1))
               printf("\nInvalid Move! You must move your piece upward\n");
          if(!(prev[1] == next[1] || next[1] == (prev[1]+1) || prev[1] == (next[1]+1)))
               printf("\nInvalid Move! You must move your piece to the same row or to adjacent rows\n");
     }

     /* Conditional 2: 
          Conditions: It is player 2's turn, their piece to move is occupied by their piece, their next 
               move goes down one column, their next move is either in the same row or beside that row.
          If conditions are met: Signal that the move is valid. */
     if(*aTurn == 0 && contains(beta, prev, AB_SIZE) != -1 && next[0] == (prev[0] + 1) && 
          (prev[1] == next[1] || next[1] == (prev[1]+1) || prev[1] == (next[1]+1))) {
               *ok = !*ok;
     } else if(*aTurn == 0) { //Make own prompts in case of wrong user input
          if(contains(beta, prev, AB_SIZE) == -1)
               printf("\nInvalid Move! You do not have a piece at (%d, %d)\n", prev[0], prev[1]);
          if(next[0] != (prev[0] + 1))
               printf("\nInvalid Move! You must move your piece downward\n");
          if(!(prev[1] == next[1] || next[1] == (prev[1]+1) || prev[1] == (next[1]+1)))
               printf("\nInvalid Move! You must move your piece to the same row or to adjacent rows\n");
     }

     /* Conditional 3
          Conditions: The move made is valid, it is player 1's turn, the block player 1 is moving 
               their piece into is free.
          If conditions are met: Move the piece to the designated area, reset variables */
     if(*ok == 1 && *aTurn == 1 && contains(nFree, next, 35) != -1) {
          replaceSet(alpha, prev, next, AB_SIZE);
          *aTurn = !*aTurn;
          *ok = !*ok;
          displayBoard(alpha, beta);
          printf("P1: (%d, %d) moves to (%d, %d)\n\n", prev[0], prev[1], next[0], next[1]);
     }

     /* Conditional 4
          Conditions: The move made is valid, it is player 2's turn, the block player 2 is moving 
               their piece into is free.
          If conditions are met: Move the piece to the designated area, reset variables */
     if(*ok == 1 && *aTurn == 0 && contains(nFree, next, 35) != -1) {
          replaceSet(beta, prev, next, AB_SIZE);
          *aTurn = !*aTurn;
          *ok = !*ok;
          displayBoard(alpha, beta);
          printf("P2: (%d, %d) moves to (%d, %d)\n\n", prev[0], prev[1], next[0], next[1]);
     }

     // Conditional 5-8 are for when taking pieces

     /* Conditional 5 
          Conditions: The move made is valid, it is player 1's turn, a piece of player 2 is in
               the area player 1's piece wants to occupy. The row and column player 1 wants to put 
               the piece in is neither both odd nor both even. 
          If conditions are met: Signal that the move is invalid. */
     if(*ok == 1 && *aTurn == 1 && contains(beta, next, AB_SIZE) != -1 && next[0] % 2 != next[1] % 2) {
          *ok = !*ok;
          printf("Invalid Move! To you cannot occupy another piece's spot if it's position is neither both odd or both even!\n\n");
     }

     /* Conditional 6
          Conditions: The move made is valid, it is player 1's turn, a piece of player 2 is in 
               the area player 1's piece wants to occupy. The row and column player 1 wants to put the 
               piece in is either both odd or both even. 
          If conditions are met: Player 1's piece eliminates player 2's piece and occupies its spot. 
               Afterwards it is now player 2's turn so reset variable "*ok". */
     if(*ok == 1 && *aTurn == 1 && contains(beta, next, AB_SIZE) != -1 && next[0] % 2 == next[1] % 2) {
          removeSet(beta, next, AB_SIZE); 
          replaceSet(alpha, prev, next, AB_SIZE);
          *aTurn = !*aTurn;
          *ok = !*ok;
          displayBoard(alpha, beta);
          printf("P1: (%d, %d) moves to and takes a piece (%d, %d)\n\n", prev[0], prev[1], next[0], next[1]);
     }

     /* Conditional 7
          Conditions: The move made is valid, it is player 2's turn, a piece of player 1 is in
               the area player 2's piece wants to occupy. The row and column player 2 wants to put 
               the piece in is neither both odd nor both even. 
          If conditions are met: Signal that the move is invalid. */
     if(*ok == 1 && *aTurn == 0 && contains(alpha, next, AB_SIZE) != -1 && next[0] % 2 != next[1] % 2) {
          *ok = !*ok;
          printf("Invalid Move! To you cannot occupy another piece's spot if it's position is neither both odd or both even!\n\n");
     }

     /* Conditional 8
          Conditions: The move made is valid, it is player 2's turn, a piece of player 1 is in 
               the area player 2's piece wants to occupy. The row and column player 2 wants to put the 
               piece in is either both odd or both even. 
          If conditions are met: Player 2's piece eliminates player 1's piece and occupies its spot. 
               Afterwards it is now player 1's turn so reset variable "*ok". */
     if(*ok == 1 && *aTurn == 0 && contains(alpha, next, AB_SIZE) != -1 && next[0] % 2 == next[1] % 2) {
          removeSet(alpha, next, AB_SIZE); 
          replaceSet(beta, prev, next, AB_SIZE);
          *aTurn = !*aTurn;
          *ok = !*ok;
          displayBoard(alpha, beta);
          printf("P2: (%d, %d) moves to and takes a piece (%d, %d)\n\n", prev[0], prev[1], next[0], next[1]);
     }
}

/*  Get which blocks do not have alpha or beta pieces
    @param nFree - the current state of set nFree
    @param beta - the current state of set beta
    @param alpha - the current state of set alpha
*/
void getFree(Set *nFree, Set *beta, Set *alpha)
{
     // Board's columns, board's rows, arr is where we store the pair to send into contains(), counter counts how many spaces are occupied
     int column, row, arr[2], counter = 0; 
     // Get free spots in board
     for(column = 1; column <= COLUMN_SIZE; column++) {
          for(row = 1; row <= ROW_SIZE; row++) {
               arr[0] = column;
               arr[1] = row;
               // If neither alpha or beta is in this current column, row. 
               if(!(contains(alpha, arr, AB_SIZE) > -1 || contains(beta, arr, AB_SIZE) > -1)) {
                    nFree[counter].x = column;
                    nFree[counter].y = row;
               } else { // If they are then this is empty for free
                    nFree[counter].x = 0;
                    nFree[counter].y = 0;
               }
               counter++;
          }
     }
}

/*  Checks if set is empty
    @param set - set to check
    @param nSize - size of Set *set 
    @return returns 0 if set is not empty, returns 1 if set is empty
*/
int isSetEmpty(Set *set, int nSize)
{
     int i; // index
     for(i = 0; i < nSize; i++) {
          if(set[i].x != 0 || set[i].y != 0) 
               return 0; // Set is not empty
     }
     return 1; // Set is empty
}

/*  Checks if set is equal to another set
    @param set1 - set to compare
    @param set2 - set to compare
    @param nSize - size of Set *set1
    @return returns 0 if set is not equal, returns 1 if set is equal
*/
int isSetEqual(Set *set1, Set *set2, int nSize)
{
     // i is index, arr is where we store the pair to send into contains() 
     int i, arr[2];
     for(i = 0; i < nSize; i++) {
          arr[0] = set1[i].x;
          arr[1] = set1[i].y;
          // If set1[i] is not in set 2
          if(contains(set2, arr, nSize) == -1) 
               return 0; // Set is not equal
     }
     return 1;
}

/*  Checks if alpha has any legal moves left
    @param set - set alpha
    @param nSize - size of Set *set 
    @return returns 0 if set is not equal, returns 1 if set is equal
*/
int alphaHasMoves(Set *set, int nSize) 
{
     // i is index. left, right, center are set's possible moves. counter is a count of how many moves are invalid
     int i, left[2], right[2], center[2], counter = 0;
     for(i = 0; i < nSize; i++) {
          left[0] = set[i].x - 1;
          center[0] = set[i].x - 1;
          right[0] = set[i].x - 1;
          left[1] = set[i].y - 1;
          center[1] = set[i].y;
          right[1] = set[i].y + 1;
          // If empty or can no longer move formard  or can no longer move forward to the left or the middle or the right
          if(set[i].x - 1 < 1 || (contains(set, left, nSize) != -1 && contains(set, center, nSize) != -1 && contains(set, right, nSize) != -1))
               counter++;
     }
     if (counter >= nSize) // If counter is equal to nSize then that means beta no longer has any moves
          return 0;
     
     return 1;
}

/*  Checks if beta has any legal moves left
    @param set - set beta
    @param nSize - size of Set *set 
    @return returns 0 if set is not equal, returns 1 if set is equal
*/
int betaHasMoves(Set *set, int nSize) 
{
     // i is index. left, right, center are set's possible moves. counter is a count of how many moves are invalid
     int i, left[2], right[2], center[2], counter = 0;
     for(i = 0; i < nSize; i++) {
          left[0] = set[i].x + 1;
          center[0] = set[i].x + 1;
          right[0] = set[i].x + 1;
          left[1] = set[i].y - 1;
          center[1] = set[i].y;
          right[1] = set[i].y + 1;
          // If empty or can no longer move formard  or can no longer move forward to the left or the middle or the right
          if(set[i].x == 0 || set[i].x + 1 > 7 || (contains(set, left, nSize) != -1 && contains(set, center, nSize) != -1 && contains(set, right, nSize) != -1))
               counter++;
     }
     if (counter >= nSize) // If counter is equal to nSize then that means beta no longer has any moves 
          return 0;
     
     return 1;
}

/*  Checks if there are any game over conditions met, if there are then show winner
     @param beta - set beta
     @param alpha - set alpha
     @param over - checks if game is over
*/
void gameOver(Set *beta, Set *alpha, int *over)
{
     // Variables used to compare in isSetEqual
     Set Y[AB_SIZE] = {{1, 1}, {1, 3}, {1, 5}, {2, 2}, {2, 4}}; // Beta's starting position
     Set E[AB_SIZE] = {{6, 2}, {6, 4}, {7, 1}, {7, 3}, {7, 5}}; // Alpha's starting position

     /* Game Over (For system facts over)
          Conditions: Alpha is empty or Beta is empty or Alpha is not empty and is equal to 
               Set Y (Alpha's pieces occupies all of Beta's starting positions) or Beta is not 
               empty and is equal to Set E (Beta's pieces occupies all of Alpha's starting positions).
          Bonus: To avoid stalemate, it is made so that whoever runs out of moves first loses.  
          If conditions are met: signal that the game is over, show who won and why. */
     if(isSetEmpty(alpha, AB_SIZE) || isSetEmpty(beta, AB_SIZE) || ((isSetEmpty(alpha, AB_SIZE) == 0 && isSetEqual(alpha, Y, AB_SIZE)) || 
          (isSetEmpty(beta, AB_SIZE) == 0 && isSetEqual(beta, E, AB_SIZE))) || betaHasMoves(beta, AB_SIZE) == 0 || alphaHasMoves(alpha, AB_SIZE) == 0) {
               *over = 1;
     }

     // If the game is over then display how player won
     // Alpha Wins
     if(isSetEmpty(beta, AB_SIZE)) // If Beta is empty
          printf("Player 1 (Alpha) Wins! All of Beta's pieces were taken");
     // If alpha is not empty and is equal to set Y
     else if(isSetEmpty(alpha, AB_SIZE) == 0 && isSetEqual(alpha, Y, AB_SIZE))
          printf("Player 1 (Alpha) Wins! Alpha took Beta's starting spot");
     else if(betaHasMoves(beta, AB_SIZE) == 0) // If beta no longer has any legal moves
          printf("Player 1 (Alpha) Wins! Beta no long has any legal moves");

     // Beta Wins
     else if(isSetEmpty(alpha, AB_SIZE)) // If alpha is empty
          printf("Player 1 (Beta) Wins! All of Alpha's pieces were taken");
     // If beta is not empty and is equal to set E
     else if(isSetEmpty(beta, AB_SIZE) == 0 && isSetEqual(beta, E, AB_SIZE))
          printf("Player 2 (Beta) Wins! Beta took Alphas's starting spot");
     else if(alphaHasMoves(alpha, AB_SIZE) == 0) // If alpha no longer has any legal moves
          printf("Player 2 (Beta) Wins! Alpha no long has any legal moves");
}

int main() 
{
     //System Variables & Initialization - Sets
     Set alpha[AB_SIZE] = {{6, 2}, {6, 4}, {7, 1}, {7, 3}, {7, 5}}, //Alpha, Player 1's pieces' starting positions
         beta[AB_SIZE]  = {{1, 1}, {1, 3}, {1, 5}, {2, 2}, {2, 4}}, //Beta, Player 2's pieces' starting positions
         nFree[FREE_SIZE]; //nFree can have 35 max slots (7 * 5). free is already a built in function thus the name nFree

     //System Variables & Initialization - Bools
     int aTurn = 1, over = 0, ok = 0;
     
     // Other variables
     int prev[2] = {0, 0}, next[2] = {0, 0};

     displayBoard(alpha, beta);
     
     while(!over) {
          // If aTurn, then it is player 1's turn, else its player 2's turn
          if(aTurn) printf("(Alpha) PLAYER 1 (O): ");
          else printf("(Beta) PLAYER 2 (X): ");
          printf("\nChoose a piece to move (Column Row): ");
          scanf("%d %d", &prev[0], &prev[1]);
          printf("\nChoose where to move to?(Column Row): ");
          scanf("%d %d", &next[0], &next[1]);
          getFree(nFree, beta, alpha); // For system facts Free
          nextPlayerMove(beta, alpha, nFree, &aTurn, &ok, prev, next); 
          gameOver(beta, alpha, &over);
     }
     
     return 0;
}