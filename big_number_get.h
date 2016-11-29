#pragma once
#include "typedefs.h"

node* node_init ();

big_number* BN_init ();

void BN_del (big_number *the_big_number);

void BN_del_tail (big_number *the_big_number);

void BN_del_head (big_number *the_big_number);

void BN_add_digit_in_tail (big_number *number, char the_new_digit);

void BN_add_digit_in_head (big_number *number, char the_new_digit);

big_number* BN_get (char sign, char the_first_digit);

big_number* BN_addition (big_number *a, big_number *b);

void BN_del_leading_zeros (big_number *the_big_number);

void BN_print (big_number *the_big_number);