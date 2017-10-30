#include "backend.h"
#include "frontend.h"
#include <ncurses.h>

int main() {
  initscr();//->初始化curses
  cbreak();//->设置为无缓冲
  noecho();//->设置为无回显
  keypad(stdscr, TRUE); //->make keys work//->允许键盘操作（准确说是中断），参数1是WINDOW *类型，stdscr代表默认屏幕大小，TRUE代表允许
  curs_set(0); //-> hide cursor//-》设置光标为不可见
  timeout(100);//->延迟100毫秒读取（不太明白）

  int xmax;//->这里xmax,ymax都未初始化，值是什么？
  int ymax;
  getmaxyx(stdscr, ymax, xmax);//->定位光标位置
  enum Direction dir = RIGHT;//->方向为右

  Board* board = create_board(create_snake(), NULL, xmax, ymax);//-》实现了对board的初始化（包含两个pointlist类型的指针和两个int类型数据）
  int i;
  for (i = 0; i < 6; i++) {
    add_new_food(board);
  }

  while(true) {
    clear();
    display_points(board->snake, ACS_BLOCK);
    display_points(board->foods, ACS_DIAMOND);
    refresh();
    dir = get_next_move(dir);
    enum Status status = move_snake(board, dir);
    if (status == FAILURE) break;
  }
  endwin();

  return 0;
}
