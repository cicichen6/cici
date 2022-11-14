#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <curses.h>
#include "cJSON.h"

#define GAME_WIN_HIGHT 25
#define GAME_WIN_WIDTH 75
#define MSG_WIN_HIGHT 5
#define MSG_WIN_WIDTH 16
#define LOG_WIN_HIGHT 7
#define GAME_LOG_SPACE 2
#define INFO_WIN_WIDTH 20
#define INFO_LEVEL_LABEL_BOTTOM -10
#define CURS_SET_HIDDEN 0
#define CURS_SET_VISIBLE 1
#define CURS_SET_FULL_VISIBLE 2
#define SNAKE_PADDING 3                 //初始化时蛇头距离边框的最小距离
#define SNAKE_HEAD_CHAR '@'
#define SNAKE_NODE_CHAR '#' 
#define FOOD_CHAR '$'
#define BORDER_PADDING 3
#define NANO_SLEEP_INTERVAL 10 * 1000 * 1000
//#define SNAKE_SPEED_CN  100000

typedef enum {
  UNDEFINED,
  UP,
  RIGHT,
  DOWN,
  LEFT,
} SNAKE_NODE_DIR;

struct SnakeNode {
  int y;
  int x;
  SNAKE_NODE_DIR nodeDir;
  struct SnakeNode *pPrevious;
};

struct Food {
  int x;
  int y;
};

typedef struct SnakeNode TSnakeNode;
typedef struct Food TFood;

void init(void);
TSnakeNode* createSnake(void);
TFood* createFood(TSnakeNode *);
bool snakeContainsXY(int, int, TSnakeNode *, bool);
WINDOW* createGameWin(void);
TSnakeNode* createSnakeNode(int, int, SNAKE_NODE_DIR);
void renderGameWin(WINDOW *, WINDOW*, TSnakeNode *, TFood *);
void onTimeInterval(long, long);
void offTimeInterval(void);
void sig_handler(); 
void runGame(void);
long getSnakeSpeed(void);
void snakeGrow(TSnakeNode *);
bool isAte(void);
bool checkIfDeath(TSnakeNode *);
void calNewXY(int *, int *, int, int, SNAKE_NODE_DIR, int);
void initGame(void);
WINDOW* createMsgWin(const char*);
void destroy(void);
WINDOW* createInfoWin(void);
void changeLevelStr(int, int);
void runlog(WINDOW *, char *);
WINDOW* createLogWin();
void cleanLine(WINDOW *, int, int, int);
char* getLevelExpMes();
void init_cJSON();

static WINDOW *gameWin = NULL;
static WINDOW *infoWin = NULL;
static WINDOW *logWin = NULL;
static TSnakeNode *snake = NULL;
static TFood *food = NULL;
static bool isStart = false;
static bool isGameOver = false;
static WINDOW* msgWin = NULL;
static int level = 1;
static int exp = 0;
static int jj = 0;
static long speed = 200000;
char *levelExpMes = NULL;
cJSON *cjson_text = NULL;
cJSON *cjson_level = NULL;
cJSON *cjson_level_item = NULL;
 
int main() {
  init();
  gameWin = createGameWin();
  infoWin = createInfoWin();
  logWin = createLogWin();
  runGame();
  endwin();

  return EXIT_SUCCESS;
}

void init(){
  initscr();
  raw();
  noecho();
  keypad(stdscr, true);
  curs_set(CURS_SET_HIDDEN);
  refresh();
}

TSnakeNode *createSnake() {
  srand((unsigned)time(NULL));
  int headY = rand() % (GAME_WIN_HIGHT - 2 * BORDER_PADDING)  + BORDER_PADDING;
  int headX = rand() % (GAME_WIN_WIDTH - 2 * BORDER_PADDING) + BORDER_PADDING;
  int dir = rand() % 3 + 1;       //上、右、下、左四个方向，最小取值为1，最大取值为4
  int tailY, tailX;

  calNewXY(&tailY, &tailX, headY, headX, (SNAKE_NODE_DIR)dir, -1);  //-1代表在蛇尾添加节点

  TSnakeNode *pHead = createSnakeNode(headY, headX, (SNAKE_NODE_DIR)dir);
  TSnakeNode *pTail = createSnakeNode(tailY, tailX, (SNAKE_NODE_DIR)dir);
  pTail->pPrevious = pHead;
  pHead->pPrevious = pTail;

  return pHead;
}

TSnakeNode* createSnakeNode(int y, int x, SNAKE_NODE_DIR dir) {
  TSnakeNode *node = (TSnakeNode*)malloc(sizeof(TSnakeNode));
  node->y = y;
  node->x = x;
  node->nodeDir = dir;
  return node;
}

WINDOW* createGameWin() {
  WINDOW *gameWin = newwin(GAME_WIN_HIGHT, GAME_WIN_WIDTH, 0, 0);
  box(gameWin, '*', '*');
  wrefresh(gameWin);
  return gameWin;
}

void renderGameWin(WINDOW *gameWin, WINDOW* infoWin, TSnakeNode *snake, TFood *food) {
  TSnakeNode *currentNode = snake->pPrevious;
  TSnakeNode *tail = currentNode;

  do {
    mvwaddch(gameWin, currentNode->y, currentNode->x, currentNode == snake ? SNAKE_HEAD_CHAR : SNAKE_NODE_CHAR);
    currentNode = currentNode->pPrevious;
    if(currentNode == tail) {
      break;
    }
  } while(true);

  mvwaddch(gameWin, food->y, food->x, FOOD_CHAR);
  wrefresh(infoWin);
  wrefresh(gameWin);
}

void onTimeInterval(long sec, long usec){
  isStart = true; 
  struct itimerval timerval;
 
  //it_value指定将于多少时长后启动
  timerval.it_value.tv_sec = sec;
  timerval.it_value.tv_usec = usec;
  //it_interval指定间隔多少时长后再次启动
  timerval.it_interval.tv_sec = sec;
  timerval.it_interval.tv_usec = usec;

  signal(SIGALRM, sig_handler);
  //setitimer定时结束后会产生一个SIGALRM信号
  setitimer(ITIMER_REAL, &timerval, NULL);
}

void offTimeInterval() {
  isStart = false;
  setitimer(ITIMER_REAL, NULL, NULL);
}  

void sig_handler() {
  int x, y;
  calNewXY(&y, &x, snake->y, snake->x, snake->nodeDir, 1);  //1代表在当前节点的前面添加节点
  TSnakeNode *newHead = createSnakeNode(y, x, snake->nodeDir);
  TSnakeNode *tail = snake->pPrevious;
  snake = snake->pPrevious = newHead;
  newHead->pPrevious = tail->pPrevious;
  mvwaddch(gameWin, tail->y, tail->x, ' ');
  free(tail);
  
  if(isAte()){
      snakeGrow(snake);
      food = createFood(snake);
  }
  if(checkIfDeath(snake)){
      offTimeInterval();
      msgWin = createMsgWin("Game Over!");
      isGameOver = true;
  }
  renderGameWin(gameWin, infoWin, snake, food);
}

void runGame() {
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = NANO_SLEEP_INTERVAL;

  bool doLoop = true;

  initGame();

  while(doLoop) {
    char input = getch();

    switch(input) {
      case 32:      //Space
          if(!isStart && !isGameOver) {
              onTimeInterval(0, getSnakeSpeed());
              continue;
          }
          break;

      case 'Q':
      case 'q':
          offTimeInterval(); 
          doLoop = false;
          destroy();
          continue;
          break;
          
      case 'R':
      case 'r':
          if(!isStart) {
              initGame();
          }
          continue;
          break;
      default:
          break;
     }

     if(!isStart){
         continue;
     }

     switch(input){
      case 'W':
      case 'w':
        if(snake->nodeDir != DOWN){
            offTimeInterval();
            snake->nodeDir = UP;
            onTimeInterval(0, getSnakeSpeed());
        }
        break;

      case 'D':
      case 'd':
        if(snake->nodeDir != LEFT){
            offTimeInterval();
            snake->nodeDir = RIGHT;
            onTimeInterval(0, getSnakeSpeed());
        }    
        break;

      case 'S':
      case 's':
        if(snake->nodeDir != UP){
            offTimeInterval();
            snake->nodeDir = DOWN;
            onTimeInterval(0, getSnakeSpeed());
        }
        break;

      case 'A':
      case 'a':
        if(snake->nodeDir != RIGHT){
            offTimeInterval();
            snake->nodeDir = LEFT;
            onTimeInterval(0, getSnakeSpeed());
        }
        break;
      default:
        break;
    }

    //暂停执行线程，以免用户长按键盘时程序频繁刷新
    if(doLoop) {
      nanosleep(&ts, NULL);
    }
  }
}

TFood* createFood(TSnakeNode *snake){
    srand((unsigned)time(NULL));

    TFood *food = (TFood *)malloc(sizeof(TFood));
    
    do{
        food->x = rand() % (GAME_WIN_WIDTH - 2) + 1;
        food->y = rand() % (GAME_WIN_HIGHT - 2) + 1;
    }while(snakeContainsXY(food->y, food->x, snake, true));  
    
    return food;
}

bool snakeContainsXY(int y, int x, TSnakeNode* snake, bool isCheckHead){
    TSnakeNode *currentNode = snake->pPrevious; 
    
    while(true){
        if(currentNode == snake && !isCheckHead){
            break;
        }
        if(y == currentNode->y && x == currentNode->x){
            return true;
        }
        currentNode = currentNode->pPrevious;
        if(currentNode == snake->pPrevious){
            break;
        }
    } 
    
    return false;
}

void snakeGrow(TSnakeNode *snake){
   static int length = 0;
   int x = 0;
   int y = 0;
   TSnakeNode *tail = snake->pPrevious;
   calNewXY(&y, &x, tail->y, tail->x, tail->nodeDir, -1); //-1代表在当前节点后面添加节点
   TSnakeNode *newNode = createSnakeNode(y, x, tail->nodeDir);
   newNode->pPrevious = tail;
   snake->pPrevious = newNode;
   length++;
   if(length == exp){
       level++;
       jj++;
       length = 0;
       runlog(logWin, "level up!");
       cjson_level_item = cJSON_GetArrayItem(cjson_level, jj);
       exp = cjson_level_item->valueint;
       changeLevelStr(level, exp);
    }    
   int foodStrTop = GAME_WIN_HIGHT + LOG_WIN_HIGHT + GAME_LOG_SPACE + INFO_LEVEL_LABEL_BOTTOM + 2;
   mvwprintw(infoWin, foodStrTop, exp - length + 1, " ");
   wrefresh(infoWin);
}

bool isAte(){
   bool result = snake->y == food->y && snake->x == food->x;
   if(result) {
      free(food);
   }
   return result;
}

bool checkIfDeath(TSnakeNode *snake){
    if(snakeContainsXY(snake->y, snake->x, snake, false)){
        return true;
    }  

   if(snake->y == 0 || snake->y == (GAME_WIN_HIGHT - 1) || snake->x == 0 || snake->x == (GAME_WIN_WIDTH - 1)){
        return true;
    }

    return false;
}

long getSnakeSpeed() {
    
    if(level != 0 && level%3 == 0){
        speed = speed - 5000;
    }
    return speed * 10/(10 +level);
}

//headOrTail:在头上添加为1，在尾上添加为-1;
void calNewXY(int *newY,int *newX,int oldY,int oldX, SNAKE_NODE_DIR dir,int headOrTail) {
    *newY = oldY;
    *newX = oldX;
    switch(dir) {
      case UP:  *newY = oldY - headOrTail; break;
      case RIGHT: *newX = oldX + headOrTail; break;
      case DOWN: *newY = oldY + headOrTail; break;
      case LEFT: *newX = oldX - headOrTail; break;
      default: break;
    }
}

void initGame(){
    destroy();
    snake = createSnake();
    food = createFood(snake);
    box(gameWin, '*', '*');
    levelExpMes = getLevelExpMes();
    init_cJSON(levelExpMes);
    level = 1;
    jj = 0;
    cjson_level_item = cJSON_GetArrayItem(cjson_level, 0);
    exp = cjson_level_item->valueint;
    changeLevelStr(level, exp);
    renderGameWin(gameWin, infoWin, snake, food);
}    



void changeLevelStr(int level, int exp) {
    char levelStr[INFO_WIN_WIDTH - 2] = {0};
    sprintf(levelStr, "%d", level);
    mvwprintw(infoWin, GAME_WIN_HIGHT + LOG_WIN_HIGHT + GAME_LOG_SPACE + INFO_LEVEL_LABEL_BOTTOM + 1, (INFO_WIN_WIDTH - strlen(levelStr)) / 2, levelStr);
    int upgradeFoods = exp;
    char foodStr[2] = {FOOD_CHAR, '\0'};
    attron(A_DIM);
    int foodStrTop = GAME_WIN_HIGHT + LOG_WIN_HIGHT + GAME_LOG_SPACE + INFO_LEVEL_LABEL_BOTTOM + 2;
    for(int ii = 1; ii <= upgradeFoods; ii++) {
      // mvwprintw(infoWin, foodStrTop + upgradeFoods % (INFO_WIN_WIDTH - 2), ii % (INFO_WIN_WIDTH - 2 + 1), foodStr);
      //TODO   
      mvwprintw(infoWin, foodStrTop, ii % (INFO_WIN_WIDTH - 2 + 1), foodStr);
    }
    attroff(A_DIM); 
    wrefresh(infoWin);
}

WINDOW* createMsgWin(const char* msg){
    WINDOW *msgWin = newwin(MSG_WIN_HIGHT, MSG_WIN_WIDTH, (GAME_WIN_HIGHT - MSG_WIN_HIGHT) / 2, (GAME_WIN_WIDTH - MSG_WIN_WIDTH) / 2);
    box(msgWin, '#', '#');
    mvwprintw(msgWin, MSG_WIN_HIGHT / 2, (MSG_WIN_WIDTH - strlen(msg)) / 2, msg);
    wrefresh(msgWin);
    return msgWin;
}

void destroy(){  
    if(snake != NULL){
        while(true){
            TSnakeNode *tail = snake->pPrevious;
            snake->pPrevious = tail->pPrevious;
            free(tail);
        
            if(snake->pPrevious == snake){
                free(snake);
                break;
            }
        }
        snake = NULL;
    }
    
    if(food != NULL){
        free(food);
        food = NULL;
    }
    isStart = false;
    
    if(msgWin != NULL){
        delwin(msgWin);
        msgWin = NULL;
    }
    werase(gameWin);
    mvwprintw(infoWin,GAME_WIN_HIGHT + LOG_WIN_HIGHT + GAME_LOG_SPACE - 2, 1, "  ");
    isGameOver = false;
    if(cjson_text != NULL){
        cJSON_Delete(cjson_text);
    }
}    

WINDOW* createInfoWin(){
    WINDOW *infoWin = newwin(GAME_WIN_HIGHT + LOG_WIN_HIGHT + GAME_LOG_SPACE, INFO_WIN_WIDTH, 0, GAME_WIN_WIDTH + GAME_LOG_SPACE);
    mvwprintw(infoWin, 1, 1, "   info:\n#\n#   W/w: up\n#   S/s: down\n#   A/a: left\n#   D/d: right\n#   Q/q: quit\n#   R/r: run again");
    box(infoWin, '#', '#');
    char spliter[INFO_WIN_WIDTH - 1] = {0};
    memset(spliter, '-', INFO_WIN_WIDTH - 2);
    mvwprintw(infoWin, GAME_WIN_HIGHT + LOG_WIN_HIGHT + GAME_LOG_SPACE + INFO_LEVEL_LABEL_BOTTOM - 1, 1, spliter);
    char *levelLabel = "Gamer Level";
    mvwprintw(infoWin, GAME_WIN_HIGHT + LOG_WIN_HIGHT + GAME_LOG_SPACE + INFO_LEVEL_LABEL_BOTTOM, (INFO_WIN_WIDTH - strlen(levelLabel)) / 2, levelLabel);
    wrefresh(infoWin);
    return infoWin;
}

WINDOW* createLogWin(){
    WINDOW *logWin = newwin(LOG_WIN_HIGHT, GAME_WIN_WIDTH, (GAME_WIN_HIGHT + GAME_LOG_SPACE), 0);
    box(logWin, '#', '#');
    wrefresh(logWin);
    return logWin;
}

void runlog(WINDOW* win,char* str){
    static char buf[LOG_WIN_HIGHT - 2][GAME_WIN_WIDTH - 2];
    static int index = 0;
    strcpy(buf[index], str);
    for(int ii = 0; ii < LOG_WIN_HIGHT - 2; ii++){
       cleanLine(win, ii + 1, 1, GAME_WIN_WIDTH);
       mvwprintw(win, ii + 1, 1, buf[(index+ii) % (LOG_WIN_HIGHT - 2)]);
    }  
    box(win, '#', '#');
    wrefresh(win);
    index = (index + LOG_WIN_HIGHT - 2 - 1) % (LOG_WIN_HIGHT - 2);
}

void cleanLine(WINDOW* win, int y, int x, int width){
    char empty[width - 2];
    memset(empty,' ', width - 2);
    mvwprintw(win, y, x, empty);
    box(win, '#', '#');
    wrefresh(win);
}

char* getLevelExpMes(){
    FILE *fp = fopen("levelex.json","r");
    char txt[100];
    int size;
    if(fp == NULL){
        printf("wrong");
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    levelExpMes = (char*)malloc(size);
    memset(levelExpMes, 0, sizeof(levelExpMes));
    rewind(fp);
    while((fgets(txt, 100, fp)) != NULL){
        strcat(levelExpMes,txt);
    }
    fclose(fp);
    return levelExpMes;
}

void init_cJSON(char * levelExpMes){
    cjson_text = NULL;
    cjson_level = NULL;
    cjson_level_item = NULL;
    int level_array_size = 0;

    cjson_text = cJSON_Parse(levelExpMes);
    if(cjson_text == NULL){
        printf("parse fail");
    }
    cjson_level = cJSON_GetObjectItem(cjson_text, "levelexp");
    level_array_size = cJSON_GetArraySize(cjson_level);
}
