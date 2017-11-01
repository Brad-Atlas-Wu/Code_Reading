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
  for (i = 0; i < 6; i++) {//->产生出6颗食物
    add_new_food(board);
  }

  while(true) {
    clear();
    display_points(board->snake, ACS_BLOCK);//->ACS_BLOCK代表“#”;这里画出最开始的蛇，坐标只有（2，2）（2，3），所以是两个#
    display_points(board->foods, ACS_DIAMOND);//->对应地，这里画出最开始的6颗食物
    refresh();//->刷新屏幕，显示内容

/**********可以看到，在这之前我们初始化并显示了蛇和食物*****************/

    dir = get_next_move(dir);//->获得并判断用户输入的方向
    enum Status status = move_snake(board, dir);//->蛇挂掉之前一直被循环执行
    if (status == FAILURE) break;
  }
  endwin();

  return 0;
}
