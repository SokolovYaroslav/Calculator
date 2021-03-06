#pragma once
#include "typedefs.h"

node* node_create ();

big_number* BN_create ();

void BN_del (big_number *the_big_number);

void BN_del_tail (big_number *the_big_number);

void BN_del_head (big_number *the_big_number);

void BN_add_digit_in_tail (big_number *number, long long int the_new_digit);

void BN_add_digit_in_head (big_number *number, long long int the_new_digit);

big_number_in_array* BN_get_in_array (char the_first_digit);

big_number* BN_get (char sign, char the_first_digit);

big_number* BN_get_zero ();

void BN_swap (big_number *a, big_number *b);

big_number* BN_addition (big_number *a, big_number *b);

big_number* BN_subtraction (big_number *a, big_number *b);

void BN_subtraction_for_division (big_number *a, big_number *b);

big_number* BN_multiplication (big_number *a, big_number *b);

big_number* BN_division (big_number *a, big_number *b);

char BN_abs_compare (big_number *a, big_number *b);

void BN_del_leading_zeros (big_number *the_big_number);

void BN_print (big_number *the_big_number);