#include "frontend.h"

#include <wchar.h>

void s21_print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  /*Рисуем левый верхний угол:*/
  mvaddch(BOARDS_BEGIN + top_y, BOARDS_BEGIN + left_x, ACS_ULCORNER);
  /*Рисуем верхнюю границу:*/
  int i = left_x + 1;
  while (i < right_x) {
    mvaddch(BOARDS_BEGIN + top_y, BOARDS_BEGIN + i, ACS_HLINE);
    i++;
  }
  /*Рисуем правый верхний угол:*/
  mvaddch(BOARDS_BEGIN + top_y, BOARDS_BEGIN + i, ACS_URCORNER);

  for (int j = top_y + 1; j < bottom_y; j++) {
    /*Рисуем левую границу:*/
    mvaddch(BOARDS_BEGIN + j, BOARDS_BEGIN + left_x, ACS_VLINE);
    /*Рисуем правую границу:*/
    mvaddch(BOARDS_BEGIN + j, BOARDS_BEGIN + right_x, ACS_VLINE);
  }
  /*Рисуем левый нижний угол:*/
  mvaddch(BOARDS_BEGIN + bottom_y, BOARDS_BEGIN + left_x, ACS_LLCORNER);
  /*Рисуем нижнюю границу:*/
  i = left_x + 1;
  while (i < right_x) {
    mvaddch(BOARDS_BEGIN + bottom_y, BOARDS_BEGIN + i, ACS_HLINE);
    i++;
  }
  /*Рисуем правый нижний угол:*/
  mvaddch(BOARDS_BEGIN + bottom_y, BOARDS_BEGIN + i, ACS_LRCORNER);
}

void s21_print_overlay() {
  /*Отрисовка границ игрового поля приставки:*/
  s21_print_rectangle(0, BOARD_HEIGHT + 4, 0, BOARD_WIDTH + HUD_WIDTH + 3);
  /*Отрисовка границ основного игрового поля:*/
  s21_print_rectangle(2, BOARD_HEIGHT + 3, 1, BOARD_WIDTH + 2);

  wchar_t *game = L"Игра:";
  wchar_t *name;
  if (GAME == 1) {
    name = L"ТЕТРИС";
  } else {
    name = L"НЕИЗВЕСТНАЯ";
  }
  wchar_t *score = L"Счёт:";
  wchar_t *hi_Score = L"Рекорд:";
  wchar_t *level = L"Уровень:";
  mvprintw(BOARDS_BEGIN + 1, BOARDS_BEGIN + 2, "%ls", game);
  mvprintw(BOARDS_BEGIN + 1, BOARDS_BEGIN + 8, "%ls", name);
  mvprintw(BOARDS_BEGIN + 3, BOARDS_BEGIN + BOARD_WIDTH + 4, "%ls", score);
  mvprintw(BOARDS_BEGIN + 5, BOARDS_BEGIN + BOARD_WIDTH + 4, "%ls", hi_Score);
  mvprintw(BOARDS_BEGIN + 7, BOARDS_BEGIN + BOARD_WIDTH + 4, "%ls", level);
}

void s21_print_next(int top_y, int left_x, GameInfo_t *status) {
  for (int i = 0; i < NEXT_SIZE * PIXEL_HEIGHT; i++) {
    for (int j = 0; j < NEXT_SIZE * PIXEL_WIDTH; j++) {
      if (status->next[i / PIXEL_HEIGHT][j / PIXEL_WIDTH] != 0) {
        mvaddch(top_y + i, left_x + j, ' ' | COLOR_PAIR(1));
      } else {
        mvaddch(top_y + i, left_x + j, ' ');
      }
    }
  }
}
void s21_print_status(GameInfo_t *status) {
  /*Вывод текущего счёта:*/
  mvprintw(BOARDS_BEGIN + 4, BOARDS_BEGIN + BOARD_WIDTH + 4, "%07d",
           status->score);
  /*Вывод текущего рекорда:*/
  mvprintw(BOARDS_BEGIN + 6, BOARDS_BEGIN + BOARD_WIDTH + 4, "%07d",
           status->high_score);
  /*Вывод текущего уровня:*/
  mvprintw(BOARDS_BEGIN + 8, BOARDS_BEGIN + BOARD_WIDTH + 4, "%02d",
           status->level);

  if (status->pause) {
    wchar_t *pause = L"ПАУЗА";
    mvprintw(BOARDS_BEGIN + BOARD_HEIGHT + 2, BOARDS_BEGIN + BOARD_WIDTH + 6,
             "%ls", pause);
  } else {
    mvprintw(BOARDS_BEGIN + BOARD_HEIGHT + 2, BOARDS_BEGIN + BOARD_WIDTH + 6,
             "     ");
  }
  if (GAME == 1 && status->next) {
    wchar_t *next = L"Следующая:";
    mvprintw(BOARDS_BEGIN + 10, BOARDS_BEGIN + BOARD_WIDTH + 4, "%ls", next);
    s21_print_next(BOARDS_BEGIN + 12, BOARDS_BEGIN + BOARD_WIDTH + 4, status);
  }
}

void s21_print_game_field(int top_y, int left_x, GameInfo_t *status) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (status->field[i / PIXEL_HEIGHT][j / PIXEL_WIDTH] != 0) {
        mvaddch(top_y + i, left_x + j, ' ' | COLOR_PAIR(1));
      } else {
        mvaddch(top_y + i, left_x + j, ' ');
      }
    }
  }
}

void s21_print_service(int top_y, int left_x, game_t *state) {
  wchar_t *usage1 = L"Нажмите клавишу \"Q\" выхода из игры";
  mvprintw(BOARDS_BEGIN + 27, BOARDS_BEGIN, "%ls", usage1);
  wchar_t *usage2 = L"Нажмите клавишу \"A\" для поворота фигуры";
  mvprintw(BOARDS_BEGIN + 28, BOARDS_BEGIN, "%ls", usage2);
  wchar_t *usage3 = L"Основное управление осуществляется стрелками";
  mvprintw(BOARDS_BEGIN + 29, BOARDS_BEGIN, "%ls", usage3);

  if (state->game_state == START) {
    wchar_t *help = L"Нажмите клавишу \"Enter\" для для старта игры        ";
    mvprintw(BOARDS_BEGIN + 25, BOARDS_BEGIN, "%ls", help);
  } else if (state->game_state == GAMEOVER) {
    wchar_t *help = L"Нажмите клавишу \"Enter\" для начала новой попытки";
    mvprintw(BOARDS_BEGIN + 25, BOARDS_BEGIN, "%ls", help);
  } else if (state->game_state == PAUSE) {
    wchar_t *help = L"Нажмите клавишу \"Пробел\" для выхода из паузы       ";
    mvprintw(BOARDS_BEGIN + 25, BOARDS_BEGIN, "%ls", help);
  } else {
    wchar_t *help = L"Нажмите клавишу \"Пробел\" для паузы игры            ";
    mvprintw(BOARDS_BEGIN + 25, BOARDS_BEGIN, "%ls", help);
  }
  if (state->game_state == GAMEOVER) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
      for (int j = 0; j < BOARD_WIDTH; j++) {
        mvaddch(top_y + i, left_x + j, ' ');
      }
    }
    wchar_t *gameover = L"ИГРА ОКОНЧЕНА!";
    mvprintw(BOARDS_BEGIN + 13, BOARDS_BEGIN + 5, "%ls", gameover);
  } else if (state->game_state == START) {
    wchar_t *ready = L"ГОТОВ К СТАРТУ!";
    mvprintw(BOARDS_BEGIN + 13, BOARDS_BEGIN + 5, "%ls", ready);
  }
}

UserAction_t s21_get_key() {
  struct timespec req, rem;
  req.tv_sec = 0;
  req.tv_nsec = 100L;
  UserAction_t key = -1;
  int ch = getch();
  if (ch == 'a' || ch == 'A')
    key = Action;
  else if (ch == KEY_UP)
    key = Up;
  else if (ch == KEY_DOWN)
    key = Down;
  else if (ch == KEY_LEFT)
    key = Left;
  else if (ch == KEY_RIGHT)
    key = Right;
  else if (ch == 'q' || ch == 'Q')
    key = Terminate;
  else if (ch == ' ')
    key = Pause;
  else if (ch == 10) {  // KEY_ENTER
    key = Start;
  }
  nanosleep(&req, &rem);
  return key;
}

void game_loop() {
  GameInfo_t *status = s21_get_game_info();
  GameInfo_t buf_status;
  FILE *file;
  if ((file = fopen(SCORE_FILE, "r"))) {
    fscanf(file, "%d", &status->high_score);
    fclose(file);
  }
  game_t *state = s21_get_state();
  UserAction_t key = 20;
  state->service = 1;
  while (state->game_state != EXIT) {
    userInput(key, false);
    /*Начальная временная задержка:*/
    if (state->service == 1 && key == 0) {
      napms(1005);
      state->service = 0;
    }
    if (state->game_state == MOVING) {
      buf_status = updateCurrentState();
    }

    s21_print_status(status);
    if (state->game_state == MOVING) {
      s21_print_game_field(BOARDS_BEGIN + 3, BOARDS_BEGIN + 2, &buf_status);
    }
    s21_print_service(BOARDS_BEGIN + 3, BOARDS_BEGIN + 2, state);
    if (state->game_state == MOVING || state->game_state == PAUSE ||
        state->game_state == START || state->game_state == GAMEOVER) {
      key = s21_get_key();
    }
  }
}

int main(void) {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  wtimeout(stdscr, SPEED_BEGIN);
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_BLUE, COLOR_BLUE);
    init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(6, COLOR_WHITE, COLOR_WHITE);
    init_pair(7, COLOR_CYAN, COLOR_CYAN);
  }
  s21_print_overlay();
  game_loop();
  endwin();

  return 0;
}