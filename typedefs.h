#pragma once

typedef struct big_number_in_array{
	unsigned long long int size;
	char* array;	
} big_number_in_array;

typedef struct node{
	long long int digit;
	struct node *next;
	struct node *previous;
} node;

typedef struct big_number{
	char sign : 1;
	unsigned int size : 7;
	struct node *head;
	struct node *tail;
} big_number;