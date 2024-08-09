#include "backend.h"

void s21_array_zero(int **array, int row, int column) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {
      array[i][j] = 0;
    }
  }
}

void s21_init_shape(int **next) {
  srand(time(NULL));
  int seed = rand() % BRICK_COUNT;
  s21_array_zero(next, NEXT_SIZE, NEXT_SIZE);
  if (seed == 0) {  // z
    next[1][1] = seed + 1;
    next[1][2] = seed + 1;
    next[2][2] = seed + 1;
    next[2][3] = seed + 1;
  } else if (seed == 1) {  // s
    next[1][2] = seed + 1;
    next[1][3] = seed + 1;
    next[2][2] = seed + 1;
    next[2][1] = seed + 1;
  } else if (seed == 2) {  // o
    next[1][1] = seed + 1;
    next[1][2] = seed + 1;
    next[2][2] = seed + 1;
    next[2][1] = seed + 1;
  } else if (seed == 3) {  // |
    next[0][2] = seed + 1;
    next[1][2] = seed + 1;
    next[2][2] = seed + 1;
    next[3][2] = seed + 1;
  } else if (seed == 4) {  // T
    next[1][2] = seed + 1;
    next[2][1] = seed + 1;
    next[2][2] = seed + 1;
    next[2][3] = seed + 1;
  } else if (seed == 5) {  // L
    next[1][3] = seed + 1;
    next[2][1] = seed + 1;
    next[2][2] = seed + 1;
    next[2][3] = seed + 1;
  } else {  // J
    next[1][1] = seed + 1;
    next[2][1] = seed + 1;
    next[2][2] = seed + 1;
    next[2][3] = seed + 1;
  }
}

game_t *s21_get_state() {
  static game_t state = {START, true};
  return &state;
}

GameInfo_t *s21_get_game_info() {
  static GameInfo_t game = {NULL, NULL, 0, 0, 0, 0, 0};
  return &game;
}

curr_brick_t *s21_get_brick() {
  static curr_brick_t brick = {NULL, 0, 0};
  return &brick;
}
curr_brick_t *s21_get_next() {
  static curr_brick_t next = {NULL, 0, 0};
  return &next;
}

int s21_check_colission_with_field(int pos_x, int pos_y, int cel, int **brick) {
  int res = 0;
  for (int i = 0; i < NEXT_SIZE && !res; i++) {
    for (int j = 0; j < NEXT_SIZE && !res; j++) {
      if (cel && brick[i][j] && pos_x == j && pos_y == i) {
        res = 1;
      }
    }
  }
  return res;
}
int s21_check_colission(int **brick, int pos_x, int pos_y) {
  int res = 0;
  GameInfo_t *status = s21_get_game_info();
  for (int i = 0; i < NEXT_SIZE && !res; i++) {
    for (int j = 0; j < NEXT_SIZE && !res; j++) {
      if (brick[i][j] && i + pos_y >= H_SIZE) {
        res = 1;
      }
    }
  }
  for (int i = 0; i < H_SIZE && !res; i++) {
    for (int j = 0; j < W_SIZE && !res; j++) {
      res = s21_check_colission_with_field(j - pos_x, i - pos_y,
                                           status->field[i][j], brick);
    }
  }
  return res;
}

int s21_check_brick_pos(int wall) {
  curr_brick_t *brick = s21_get_brick();
  int res = -1;
  int start;
  int step;
  if (wall == Left) {
    start = 0;
    step = 1;
  } else {
    start = 4;
    step = -1;
  }
  for (int j = start; res < 0 && j * step < NEXT_SIZE - start; j += step) {
    for (int i = 0; res < 0 && i < NEXT_SIZE; i++) {
      if (brick->brick[i][j]) res = j;
    }
  }
  return res;
}

void s21_move(int dir) {
  curr_brick_t *brick = s21_get_brick();
  if (dir == Left) {
    brick->pos_x -= 1;
    int pos = s21_check_brick_pos(dir);
    if (brick->pos_x + pos < 0 ||
        s21_check_colission(brick->brick, brick->pos_x, brick->pos_y)) {
      brick->pos_x += 1;
    }
  } else if (dir == Right) {
    brick->pos_x += 1;
    int pos = s21_check_brick_pos(dir);
    if (brick->pos_x + pos >= W_SIZE ||
        s21_check_colission(brick->brick, brick->pos_x, brick->pos_y)) {
      brick->pos_x -= 1;
    }
  }
}

void s21_transform(int *buff, int **matr, int row, int col) {
  for (int i = 0; i < row; i++) {
    matr[i] = buff + i * col;
  }
}

void s21_copy_matrix(int **dir, int **source, int row, int col) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      dir[i][j] = source[i][j];
    }
  }
}

void s21_copy_figure(int **orig, int **next, int x, int y) {
  for (int i = 0; i < NEXT_SIZE; i++) {
    for (int j = 0; j < NEXT_SIZE; j++) {
      if (i + y >= 0 && j + x >= 0 && next[i][j])
        orig[i + y][j + x] = next[i][j];
    }
  }
}

void s21_action() {
  curr_brick_t *brick = s21_get_brick();
  int buf_brick[NEXT_SIZE * NEXT_SIZE] = {0};
  int *buff[NEXT_SIZE];
  s21_transform(buf_brick, buff, NEXT_SIZE, NEXT_SIZE);
  s21_copy_matrix(buff, brick->brick, NEXT_SIZE, NEXT_SIZE);
  for (int i = 0; i < NEXT_SIZE; i++) {
    for (int j = i; j < NEXT_SIZE; j++) {
      int temp = buff[i][j];
      buff[i][j] = buff[j][i];
      buff[j][i] = temp;
    }
  }

  /*Квадрат не вращаем:*/
  for (int i = 0; buff[2][2] != 3 && i < NEXT_SIZE; i++) {
    for (int j = 0; j < NEXT_SIZE / 2; j++) {
      int temp = buff[i][j];
      buff[i][j] = buff[i][NEXT_SIZE - 1 - j];
      buff[i][NEXT_SIZE - 1 - j] = temp;
    }
  }

  /*Палку двигаем по х на -1:*/
  for (int i = 0; i < NEXT_SIZE; i++) {
    for (int j = 1; j < NEXT_SIZE; j++) {
      if (buff[i][j] == 4) {
        buff[i][j - 1] = buff[i][j];
        buff[i][j] = 0;
      }
    }
  }

  if (!s21_check_colission(buff, brick->pos_x, brick->pos_y)) {
    s21_array_zero(brick->brick, NEXT_SIZE, NEXT_SIZE);
    s21_copy_figure(brick->brick, buff, 0, 0);
    int pos_left = s21_check_brick_pos(Left);
    int pos_right = s21_check_brick_pos(Right);
    while (brick->pos_x + pos_left < 0) {
      brick->pos_x += 1;
    }
    while (brick->pos_x + pos_right >= W_SIZE) {
      brick->pos_x -= 1;
    }
  }
}

void s21_start_state(UserAction_t sig, game_state_t *state) {
  switch (sig) {
    case Start:
      s21_init_status();
      *state = SPAWN;
      break;
    case Terminate:
      *state = EXIT;
      break;
    default:
      *state = START;
      break;
  }
}

void s21_spawn_state(game_state_t *state, GameInfo_t *status) {
  curr_brick_t *brick = s21_get_brick();
  if (!brick->brick) {
    static int buf_brick[NEXT_SIZE * NEXT_SIZE] = {0};
    static int *ptr_brick[NEXT_SIZE];
    s21_transform(buf_brick, ptr_brick, NEXT_SIZE, NEXT_SIZE);
    brick->brick = ptr_brick;
  }
  s21_copy_matrix(brick->brick, status->next, NEXT_SIZE, NEXT_SIZE);

  /*Задаём позицию появления фигуры:*/
  brick->pos_x = 3;
  if (brick->brick[0][2]) {
    brick->pos_y = 0;
  } else {
    brick->pos_y = -1;
  }

  /*Генерируем новую фигуру:*/
  s21_init_shape(status->next);
  if (!s21_check_colission(brick->brick, brick->pos_x, brick->pos_y)) {
    *state = MOVING;
  } else {
    *state = GAMEOVER;
  }
}

void s21_moving_state(UserAction_t sig, game_state_t *state,
                      GameInfo_t *status) {
  switch (sig) {
    case Left:
      s21_move(Left);
      break;
    case Right:
      s21_move(Right);
      break;
    case Down:
      while (*state != ATTACHING) {
        if (!s21_step_down()) {
          *state = ATTACHING;
        }
      }
      break;
    case Action:
      s21_action();
      break;
    case Pause:
      *state = PAUSE;
      break;
    case Terminate:
      *state = EXIT;
    default:
      break;
  }
  static int cur_step = 0;
  cur_step++;
  if (cur_step > (110 - 10 * status->speed)) {
    cur_step = 0;
    *state = SHIFTING;
  }
}

void s21_shifting_state(game_state_t *state) {
  if (s21_step_down()) {
    *state = MOVING;
  } else {
    *state = ATTACHING;
  }
}

void s21_pause_state(UserAction_t sig, game_state_t *state,
                     GameInfo_t *status) {
  switch (sig) {
    case Pause:
      *state = MOVING;
      status->pause = 0;
      break;
    case Terminate:
      *state = EXIT;
      break;
    default:
      status->pause = 1;
      break;
  }
}

void s21_attaching_state(game_state_t *state, GameInfo_t *status) {
  curr_brick_t *brick = s21_get_brick();
  s21_copy_figure(status->field, brick->brick, brick->pos_x, brick->pos_y);
  /*Проверка заполнений линий:*/
  s21_check_lines();
  /*Проверка достижения уровня:*/
  s21_check_level();
  /*Перезапись рекорда:*/
  s21_record_score();
  *state = SPAWN;
}

void s21_gameover_state(UserAction_t sig, game_state_t *state,
                        GameInfo_t *status) {
  switch (sig) {
    case Start:
      s21_array_zero(status->field, H_SIZE, W_SIZE);
      *state = START;
      break;
    case Terminate:
      *state = EXIT;
      break;
    default:
      break;
  }
}

void userInput(UserAction_t action, bool hold) {
  (void)(hold);  // временно не использую
  game_t *state = s21_get_state();
  GameInfo_t *status = s21_get_game_info();
  switch (state->game_state) {
    case START:
      s21_start_state(action, &state->game_state);
      break;
    case SPAWN:
      s21_spawn_state(&state->game_state, status);
      break;
    case MOVING:
      s21_moving_state(action, &state->game_state, status);
      break;
    case SHIFTING:
      s21_shifting_state(&state->game_state);
      break;
    case PAUSE:
      s21_pause_state(action, &state->game_state, status);
      break;
    case ATTACHING:
      s21_attaching_state(&state->game_state, status);
      break;
    case GAMEOVER:
      s21_gameover_state(action, &state->game_state, status);
      break;
    default:
      state->game_state = EXIT;
      break;
  }
}

void s21_copy_game(int **field) {
  GameInfo_t *status = s21_get_game_info();
  curr_brick_t *brick = s21_get_brick();
  s21_copy_matrix(field, status->field, H_SIZE, W_SIZE);
  s21_copy_figure(field, brick->brick, brick->pos_x, brick->pos_y);
}

GameInfo_t updateCurrentState() {
  GameInfo_t *status = s21_get_game_info();
  GameInfo_t buff = {NULL, NULL, 0, 0, 0, 0, 0};
  buff.next = status->next;
  buff.level = status->level;
  buff.speed = status->speed;
  buff.pause = status->pause;
  buff.high_score = status->high_score;
  buff.score = status->score;
  static int field[W_SIZE * H_SIZE] = {0};
  static int *ptr_field[H_SIZE];
  s21_transform(field, ptr_field, H_SIZE, W_SIZE);
  buff.field = ptr_field;
  s21_copy_game(buff.field);
  return buff;
}

void s21_init_status() {
  GameInfo_t *status = s21_get_game_info();
  if (!status->field) {
    static int buf_field[W_SIZE * H_SIZE] = {0};
    static int *ptr_buf_field[H_SIZE];
    s21_transform(buf_field, ptr_buf_field, H_SIZE, W_SIZE);
    status->field = ptr_buf_field;
  }

  curr_brick_t *next = s21_get_next();
  if (!next->brick) {
    static int buf_brick[NEXT_SIZE * NEXT_SIZE] = {0};
    static int *ptr_brick[NEXT_SIZE];
    s21_transform(buf_brick, ptr_brick, NEXT_SIZE, NEXT_SIZE);
    next->brick = ptr_brick;
    s21_init_shape(next->brick);
    next->pos_x = 3;
    if (next->brick[0][2]) {
      next->pos_y = 0;
    } else {
      next->pos_y = -1;
    }
  }
  status->next = next->brick;
  status->score = 0;
  status->level = 1;
  status->speed = 1;
  status->pause = 0;
}

int s21_step_down() {
  int res_move = 1;
  curr_brick_t *brick = s21_get_brick();
  brick->pos_y += 1;
  if (s21_check_colission(brick->brick, brick->pos_x, brick->pos_y)) {
    brick->pos_y -= 1;
    res_move = 0;
  }
  return res_move;
}

void s21_move_field_down(int row) {
  GameInfo_t *status = s21_get_game_info();
  for (int i = 0; i < W_SIZE; i++) {
    for (int j = row; j >= 0; j--) {
      if (j == 0) {
        status->field[j][i] = 0;
      } else {
        status->field[j][i] = status->field[j - 1][i];
      }
    }
  }
}

void s21_check_lines() {
  GameInfo_t *status = s21_get_game_info();
  int num_rows = 0;
  for (int i = 0; i < H_SIZE; i++) {
    int row_full = 1;
    for (int j = 0; row_full && j < W_SIZE; j++) {
      row_full = status->field[i][j];
    }
    if (row_full) {
      s21_move_field_down(i);
      num_rows++;
    }
  }

  /*Обновляю счёт:*/
  while (num_rows) {
    num_rows--;
    status->score += pow(2, num_rows) * 100;
  }

  /*Обновляю рекорд:*/
  if (status->score > status->high_score) {
    status->high_score = status->score;
  }
}

void s21_check_level() {
  GameInfo_t *status = s21_get_game_info();
  if (status->score >= 600) {
    status->level = status->score / 600;
  }
  if (status->level > 10) {
    status->level = 10;
  }
  status->speed = status->level;
}

void s21_record_score() {
  GameInfo_t *status = s21_get_game_info();
  FILE *file;
  file = fopen(SCORE_FILE, "w+");
  fprintf(file, "%d", status->high_score);
  fclose(file);
}
