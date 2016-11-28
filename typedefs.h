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