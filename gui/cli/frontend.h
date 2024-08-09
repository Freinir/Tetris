/// @mainpage
/// @file frontend.h
/// @author hornedsi
/// @brief Игра тетрис (интерфейс)
/// @version 1.0
/// @date 01.08.2024
///
/// @copyright Copyright (c) 2024

#ifndef FRONTEND_H
#define FRONTEND_H

#include <locale.h>
#include <ncurses.h>

#include "../../brick_game/tetris/backend.h"

/// Высота пикселя
#define PIXEL_HEIGHT 1

/// Ширина пикселя
#define PIXEL_WIDTH 2

/// Начальный отступ на экране вправо вниз
#define BOARDS_BEGIN 2

/// Задержка цикла интерфейса
#define SPEED_BEGIN 10

/// Ширина поля дополнительной информации
#define HUD_WIDTH 12

/// Высота игрового поля
#define BOARD_HEIGHT H_SIZE *PIXEL_HEIGHT

/// Ширина игрового поля
#define BOARD_WIDTH W_SIZE *PIXEL_WIDTH

/// @brief Функция отрисовки прямоугольников
/// @param top_y координата верхней границы
/// @param bottom_y координата нижней границы
/// @param left_x координата левой границы
/// @param right_x координата правой границы
void s21_print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

/// @brief Функция отрисовки интерфейсной подложки
void s21_print_overlay();

/// @brief Функция печати состояния игры
/// @param status Указатель на сруктуру текущего состояния игры
void s21_print_stats(GameInfo_t *status);

/// @brief Функция печати следующей фигуры
/// @param top_y координата верхней границы
/// @param left_x координата левой границы
/// @param status Указатель на сруктуру текущего состояния игры
void s21_print_next(int top_y, int left_x, GameInfo_t *status);

/// @brief Функция печати игрового поля
/// @param top_y координата верхней границы
/// @param left_x координата левой границы
/// @param status Указатель на сруктуру текущего состояния игры
void s21_print_game_field(int top_y, int left_x, GameInfo_t *status);

/// @brief Функция определения ввода от игрока
/// @return Возвращает действие, которое совершил игрок
UserAction_t s21_get_key();

/// @brief Функция отрисовки дополнительной информации игрового состояния
/// @param status Указатель на сруктуру текущего состояния игры
void s21_print_status(GameInfo_t *status);

/// @brief Функция сервисной печати и подсказок
/// @param top_y координата верхней границы
/// @param left_x координата левой границы
/// @param status Указатель на сруктуру текущего состояния игры
void s21_print_service(int top_y, int left_x, game_t *state);

/// @brief Основная функция с зацикливанием интерфейса игры
void game_loop();

#endif