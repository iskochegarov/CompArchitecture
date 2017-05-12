#ifndef MYGUI_H
#define MYGUI_H

#include "memory.h"

int flg_state (int8_t regist);
void color_reset ();
void show_accumulator ();
void show_ic ();
void show_memory();
void show_bigchars();
void show_operation();
void show_flags();
void show_keys();
void refresh();

#endif
