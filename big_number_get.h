#pragma once

typedef struct node{
	unsigned char digit;
	struct node *next;
	struct node *previous;
} node;

typedef struct big_number{
	char sign;
	struct node *head;
	struct node *tail;
} big_number;

big_number* BN_get (char sign, unsigned char first_digit);

void BN_add_digit (big_number *number, unsigned char the_new_digit);