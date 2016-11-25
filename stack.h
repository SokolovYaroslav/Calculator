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

void stack_launch ();

void stack_realloc ();

void stack_push (big_number* new_big_number);

big_number* stack_pop ();

void stack_clear ();