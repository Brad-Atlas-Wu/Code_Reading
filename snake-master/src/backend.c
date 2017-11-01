#include "backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
 * Removes from the list or returns false
 */
bool remove_from_list(PointList* elt, PointList** list) {
  PointList *currP, *prevP;
  prevP = NULL;
	
  for (currP = *list;
      currP != NULL;
      prevP = currP, currP = currP->next) {
    if (is_same_place(currP, elt)) {
      if (prevP == NULL) {//->被吃的食物是链表的第一个，删除
        *list = currP->next;
      } else {
        prevP->next = currP->next;//->被吃的食物是链表的某中间一环或最后一个，删除
      }
      free(currP);
      return true;//->成功删除被吃的食物
    }
  }
  return false;
}

enum Status move_snake(Board* board, enum Direction dir) {
  // Create a new beginning. Check boundaries.
  PointList* beginning = next_move(board, dir);
  if (beginning == NULL) {//->撞墙了
    return FAILURE;
}

  // If we've gone backwards, don't do anything
  if (board->snake->next && is_same_place(beginning, board->snake->next)) {
    beginning->next = NULL;//->往回走，忽视
    free(beginning);
    return SUCCESS;
  }

  // Check for collisions//->检测是否撞到自己
  if (list_contains(beginning, board->snake)) {
    return FAILURE;
  }

  // Check for food
  if (list_contains(beginning, board->foods)) {
    // Attach the beginning to the rest of the snake;
    beginning->next = board->snake;//->吃到食物，在前面加长一节
    board->snake = beginning;
    remove_from_list(beginning, &(board->foods));//->用删除链表元素的方法，删除掉刚刚被吃的食物
    add_new_food(board);

    return SUCCESS;
  }


  // Attach the beginning to the rest of the snake//-> 蛇头多一格
  beginning->next = board->snake;
  board->snake = beginning;


  // Cut off the end				//-》蛇尾消除掉一格（确切说是全部前移一格）
  PointList* end = board->snake;
  while(end->next->next) {
    end = end->next;
  }
  free(end->next);//最后一格丢掉
  end->next = NULL;

  return SUCCESS;
}

//->对比新出现的食物和蛇/其他食物是不是同一位置
bool is_same_place(PointList* cell1, PointList* cell2) {
  return cell1->x == cell2->x && cell1->y == cell2->y;
}


PointList* next_move(Board* board, enum Direction dir) {
  PointList* snake = board->snake;
  int new_x = snake->x;
  int new_y = snake->y;
  switch(dir) {
    case UP:
      new_y = snake->y - 1;
      break;
    case DOWN:
      new_y = snake->y + 1;
      break;
    case LEFT:
      new_x = snake->x - 1;
      break;
    case RIGHT:
      new_x = snake->x + 1;
      break;
  }
  if (new_x < 0 || new_y < 0 || new_x >= board->xmax || new_y >= board->ymax) {
    return NULL;
  } else {
    return create_cell(new_x, new_y);
  }
}

PointList* create_random_cell(int xmax, int ymax) {
  return create_cell(rand() % xmax, rand() % ymax);
}

/*求新事物坐标，并且与之前的食物连接起来（链表）*/
void add_new_food(Board* board) {
  PointList* new_food;
/*->为新食物产生坐标（和其他食物/蛇的坐标不重合）*/
  do {
    new_food = create_random_cell(board->xmax, board->ymax);
  } while(list_contains(new_food, board->foods) || list_contains(new_food, board->snake));//-》判断新食物坐标和其他食物/蛇不重叠

  new_food->next = board->foods;
  board->foods = new_food;//这里是利用board->foods为中间量，所以是把新食物与之前的食物连接起来
}

/*->
 * 循环判断蛇/食物指针是否指向有效位置
 * 	是：
 * 	对比新出现的食物和蛇/其他食物是否在同一位置
 *		是：结束循环，返回真
 *		否：继续循环，蛇/食物向next移动
 *	否：返回假
 * */
bool list_contains(PointList* cell, PointList* list) {
  PointList* s = list;//->定义一个临时指针，等于food/snake
  while (s) {
    if (is_same_place(s, cell)) {
      return true;
    }
    s = s->next;
  }
  return false;
}

PointList* create_cell(int x, int y) {
  PointList* cell = malloc(sizeof(*cell));
  cell->x = x;
  cell->y = y;
  cell->next = NULL;
  return cell;
}

Board* create_board(PointList* snake, PointList* foods, int xmax, int ymax) {
  Board* board = malloc(sizeof(*board));
  board->foods = foods;
  board->snake = snake;
  board->xmax = xmax;
  board->ymax = ymax;
  return board;
}

  PointList* create_snake() {
  PointList* a = create_cell(2,3);
  PointList* b = create_cell(2,2);
  a->next = b;
  return a;
}

