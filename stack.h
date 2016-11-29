#pragma once
#include "typedefs.h"

void stack_launch ();

void stack_realloc ();

void stack_push (big_number *new_big_number);

big_number* stack_pop ();

big_number* stack_last ();

unsigned int stack_size ();

void stack_clear ();