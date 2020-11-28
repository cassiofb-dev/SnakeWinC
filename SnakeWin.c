/*
*   Author: cassiofb-dev
*   Github: https://github.com/cassiofb-dev
*   Date:   November 28 2020
*
*     Hello, I made this program for fun to show
*     that anyone with basic knowledge in c or
*     any other language can do cool things.
*
*     I hope everything is well commented so that
*     everyone can understand it without having
*     to google it or a havin headache. Have fun!
*/


//---------------------------------- IMPORTS

#include <time.h>   //  for: nanosleep(), time()
#include <stdio.h>  //  for: printf()
#include <conio.h>  //  for: _kbhit(), getch()
#include <stdlib.h> //  for: malloc(), rand(), srand()
#include <string.h> //  for: strcat(), memset()


//---------------------------------- UTILITY FUNCTIONS

//  Swap two integers
void swap(int *a, int *b) { int c = *a; *a = *b; *b = c; }

//  Pause the program for some time (nanoseconds)
int sleepNS(long seconds, long nanoseconds) {
  struct timespec requested, remaining;

  requested.tv_sec = seconds;
  requested.tv_nsec = nanoseconds;

  return nanosleep(&requested, &remaining);
}

//  Pause the program for some time (milliseconds)
int sleepMS(long seconds, long milliseconds) {
  return sleepNS(seconds, milliseconds*1000000);
}


//---------------------------------- SNAKE

//  the snake will be a linked list
//  It will start with the head and end with the tail
//  Example: [1,2] -> [1,3] -> [1,4] -> NULL

//  Snake node with the postition and the next
typedef struct snake_node {
  int x;                      //  x position
  int y;                      //  y position
  struct snake_node *next;    //  the next position
} *SnakeNodePointer;          //  pointer for better memory control

//  Snake with pointers to the head and tail
typedef struct snake {
  int size;                 //  snake size
  SnakeNodePointer head;    //  snake start node
  SnakeNodePointer tail;    //  snake end node
} *SnakePointer;            //  pointer for better memory control

//  Create the snake
SnakePointer createSnake(int startX, int startY) {

  //  Get ram memory for the snake and snake node
  SnakePointer snakePTR = (SnakePointer) malloc(sizeof(struct snake));
  SnakeNodePointer snakeNodePTR = (SnakeNodePointer) malloc(sizeof(struct snake_node));

  snakeNodePTR->x = startX;   //  x start position
  snakeNodePTR->y = startY;   //  y start position
  snakeNodePTR->next = NULL;  //  NULL always marks the end

  // With one node, the head and the tail is the same
  snakePTR->head = snakeNodePTR;
  snakePTR->tail = snakeNodePTR;

  snakePTR->size = 1;

  return snakePTR;
}

//  To delete the snake from ram
void deleteSnake(SnakePointer snakePTR) {
  SnakeNodePointer snakeNodePTR = snakePTR->head->next;
  SnakeNodePointer prevSnakeNodePTR = snakePTR->head;

  while(snakeNodePTR) {
    free(prevSnakeNodePTR);
    prevSnakeNodePTR = snakeNodePTR;
    snakeNodePTR = snakeNodePTR->next;
  }

  free(prevSnakeNodePTR);
  free(snakePTR);
}

//  Update the snake path
void updateSnakePath(SnakePointer snakePTR, int x, int y) {
  SnakeNodePointer snakeNodePTR = snakePTR->head;

  while(snakeNodePTR) {
    swap(&snakeNodePTR->x, &x);
    swap(&snakeNodePTR->y, &y);
    snakeNodePTR = snakeNodePTR->next;
  }
}

//  Move the snake up
void moveSnakeUp(SnakePointer snakePTR) {
  updateSnakePath(snakePTR, snakePTR->head->x, snakePTR->head->y-1);
}

//  Move the snake down
void moveSnakeDown(SnakePointer snakePTR) {
  updateSnakePath(snakePTR, snakePTR->head->x, snakePTR->head->y+1);
}

//  Move the snake left
void moveSnakeLeft(SnakePointer snakePTR) {
  updateSnakePath(snakePTR, snakePTR->head->x-1, snakePTR->head->y);
}

//  Move the snake right
void moveSnakeRight(SnakePointer snakePTR) {
  updateSnakePath(snakePTR, snakePTR->head->x+1, snakePTR->head->y);
}

//  Insert node at snake tail
void increaseSnakePath(SnakePointer snakePTR) {
  SnakeNodePointer snakeNodePTR = (SnakeNodePointer) malloc(sizeof(struct snake_node));

  //  Start position is the same as the tail
  snakeNodePTR->x = snakePTR->tail->x;
  snakeNodePTR->y = snakePTR->tail->y;
  snakeNodePTR->next = NULL;

  //  Makes the node after the tail the tail
  snakePTR->tail->next = snakeNodePTR;
  snakePTR->tail = snakeNodePTR;
  snakePTR->size++;
}

//  check if the snake bite the tail
int checkSnakeBiteTail(SnakePointer snakePTR) {
  if(snakePTR->size < 2) return 0;

  SnakeNodePointer snakeNodePTR = snakePTR->head->next->next;

  while(snakeNodePTR) {
    if(snakePTR->head->x == snakeNodePTR->x && snakePTR->head->y == snakeNodePTR->y) return 1;
    snakeNodePTR = snakeNodePTR->next;
  }

  return 0;
}

// used to print the game
int checkSnakePath(SnakePointer snakePTR, int x, int y) {
  SnakeNodePointer snakeNodePTR = snakePTR->head;

  while(snakeNodePTR) {
    if(x == snakeNodePTR->x && y == snakeNodePTR->y) return 1;
    snakeNodePTR = snakeNodePTR->next;
  }

  return 0;
}

//  print the snake (only used for testing)
void printSnake(SnakePointer snakePTR) {
  SnakeNodePointer snakeNodePTR = snakePTR->head;

  printf("snake size: %d\nsnake path:{", snakePTR->size);

  while(snakeNodePTR->next) {
    printf("(%d, %d),", snakeNodePTR->x, snakeNodePTR->y);
    snakeNodePTR = snakeNodePTR->next;
  }

  printf("(%d, %d)}\n", snakeNodePTR->x, snakeNodePTR->y);
}


//---------------------------------- Food

// the food will be a simple struct
// containing the (x,y) position

typedef struct food{
  int x;
  int y;
} *FoodPointer;

//  create the food within a range
FoodPointer createFood(int xMin, int xMax, int yMin, int yMax) {
  FoodPointer foodPTR = (FoodPointer) malloc(sizeof(struct food));

  foodPTR->x = rand() % (xMax - xMin + 1) + xMin;
  foodPTR->y = rand() % (yMax - yMin + 1) + yMin;

  return foodPTR;
}

//  to delete food from ram
void deleteFood(FoodPointer foodPTR) { free(foodPTR); }

//  change the food position
void changeFoodPosition(FoodPointer foodPTR, int xMin, int xMax, int yMin, int yMax) {
  foodPTR->x = rand() % (xMax - xMin + 1) + xMin;
  foodPTR->y = rand() % (yMax - yMin + 1) + yMin;
}


//---------------------------------- Snake Game

// The snake game will be struct containing
// one snake, one food, the map size
// a flag to check the game over
// and the last input

typedef struct snake_game{
  SnakePointer snakePTR;    //  Snake Pointer
  FoodPointer foodPTR;      //  Food Pointer
  int width;
  int height;
  int gameover;
  char lastInput;
} *SnakeGamePointer;        //  pointer for better memory control

//  create a new game
SnakeGamePointer createSnakeGame(int width, int height) {
  SnakeGamePointer snakeGamePTR = (SnakeGamePointer) malloc(sizeof(struct snake_game));

  snakeGamePTR->width = width;
  snakeGamePTR->height = height;
  snakeGamePTR->gameover = 0;
  snakeGamePTR->snakePTR = createSnake(0, 0);
  snakeGamePTR->foodPTR = createFood(0, width-1, 0, height-1);

  return snakeGamePTR;
}

//  delete game from ram
void deleteSnakeGame(SnakeGamePointer snakeGamePTR) {
  deleteSnake(snakeGamePTR->snakePTR);
  deleteFood(snakeGamePTR->foodPTR);
  free(snakeGamePTR);
}

//  get char input
void inputSnakeGame(SnakeGamePointer snakeGamePTR, char c) {
  switch(c) {
    case 'w':
      moveSnakeUp(snakeGamePTR->snakePTR);
    break;

    case 'a':
      moveSnakeLeft(snakeGamePTR->snakePTR);
    break;

    case 's':
      moveSnakeDown(snakeGamePTR->snakePTR);
    break;

    case 'd':
      moveSnakeRight(snakeGamePTR->snakePTR);
    break;
  }

  snakeGamePTR->lastInput = c;
}

//  get player input
void getPlayerInput(SnakeGamePointer snakeGamePTR) {
  if(_kbhit()) {
    inputSnakeGame(snakeGamePTR, getch());
    return;
  };

  inputSnakeGame(snakeGamePTR, snakeGamePTR->lastInput);
}

//  when the snake eat the food
void snakeGetFood(SnakeGamePointer snakeGamePTR) {
  int snakeX = snakeGamePTR->snakePTR->head->x;
  int snakeY = snakeGamePTR->snakePTR->head->y;

  int foodX = snakeGamePTR->foodPTR->x;
  int foodY = snakeGamePTR->foodPTR->y;

  if(snakeX == foodX && snakeY == foodY) {
    increaseSnakePath(snakeGamePTR->snakePTR);
    changeFoodPosition(snakeGamePTR->foodPTR, 0, snakeGamePTR->width-1, 0, snakeGamePTR->height-1);
  }
}

//  check if the snake is out of the map
int checkSnakeOutOfMap(SnakeGamePointer snakeGamePTR) {
  int snakeX = snakeGamePTR->snakePTR->head->x;
  int snakeY = snakeGamePTR->snakePTR->head->y;

  if(snakeX < 0 || snakeX >= snakeGamePTR->width) return 1;
  if(snakeY < 0 || snakeY >= snakeGamePTR->height) return 1;

  return 0;
}

//  check if the game is over
void checkGameLogic(SnakeGamePointer snakeGamePTR) {
  snakeGetFood(snakeGamePTR);
  snakeGamePTR->gameover += checkSnakeOutOfMap(snakeGamePTR);
  snakeGamePTR->gameover += checkSnakeBiteTail(snakeGamePTR->snakePTR);
}

//  print the game screen
void printGame(SnakeGamePointer snakeGamePTR) {
  int width = snakeGamePTR->width, height = snakeGamePTR->height;
  int screenBufferSize = height * (width * 3 + 1) + 1;
  char screen[screenBufferSize];

  //  clear screen buffer
  memset(&screen, 0, sizeof(screen));

  int foodX = snakeGamePTR->foodPTR->x;
  int foodY = snakeGamePTR->foodPTR->y;

  int i, j;
  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      if(checkSnakePath(snakeGamePTR->snakePTR, j, i)) {
        strcat(screen, "[X]");
      } else if (foodX == j && foodY == i) {
        strcat(screen, "[O]");
      } else {
        strcat(screen, "[ ]");
      }
    }
    strcat(screen, "\n");
  }
  printf("%s", screen);
}

// used to testing
void printGameStatus(SnakeGamePointer snakeGamePTR) {
  printSnake(snakeGamePTR->snakePTR);
  printf("game food: (%d, %d)\n", snakeGamePTR->foodPTR->x, snakeGamePTR->foodPTR->y);
  printf("game over: %d\n",snakeGamePTR->gameover);
  printf("game size: %d x %d\n",snakeGamePTR->width, snakeGamePTR->height);
}

//  function to play the snake game
void playSnakeGame(int mapSize) {
  srand(time(NULL));  // starts random seed

  SnakeGamePointer snakeGamePTR = createSnakeGame(mapSize, mapSize);

  if(mapSize < 31) {
    while(!snakeGamePTR->gameover) {
      system("cls");
      getPlayerInput(snakeGamePTR);
      checkGameLogic(snakeGamePTR);
      printGame(snakeGamePTR);
      sleepMS(0, 200 - 10*snakeGamePTR->snakePTR->size);
    }
  } else {
    while(!snakeGamePTR->gameover) {
      system("cls");
      getPlayerInput(snakeGamePTR);
      checkGameLogic(snakeGamePTR);
      printGameStatus(snakeGamePTR);
      sleepMS(0, 200 - 10*snakeGamePTR->snakePTR->size);
    }
  }

  printf("Game over: %d points\n", snakeGamePTR->snakePTR->size);
  deleteSnakeGame(snakeGamePTR);
  system("pause");
}

//---------------------------------- MAIN FUNCTION

int main() {

  int mapSize;
  printf("Map size: ");
  scanf("%d",&mapSize);

  playSnakeGame(mapSize);

  return 0;
}