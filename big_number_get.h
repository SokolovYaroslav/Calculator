#pragma once
#include "typedefs.h"

big_number* BN_get (char sign, unsigned char first_digit);

void BN_add_digit (big_number *number, unsigned char the_new_digit);

void BN_del (big_number *the_big_number);