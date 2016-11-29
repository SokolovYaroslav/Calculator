#include <stdio.h>
#include <stdlib.h>
#include "big_number_get.h"

node* node_init () {
	node *new_node;
	new_node = (node*)malloc(sizeof(node));
	new_node->digit = 0;
	new_node->previous = NULL;
	new_node->next = NULL;
	return new_node;
}

big_number* BN_init () {
	big_number *new_big_number;
	new_big_number = (big_number*)malloc(sizeof(big_number));
	new_big_number->sign = 0;
	new_big_number->head = NULL;
	new_big_number->tail = NULL;
	return new_big_number;
}

void BN_del (big_number *the_big_number) {
	while (the_big_number->head->next) {
		the_big_number->head = the_big_number->head->next;
		free(the_big_number->head->previous);
	}
	free(the_big_number->head);
	free(the_big_number);
}

void BN_del_tail (big_number *the_big_number) {
	if (the_big_number->tail->previous) {
		the_big_number->tail = the_big_number->tail->previous;
		free(the_big_number->tail->next);
		the_big_number->tail->next = NULL;
	}
	else {
		free(the_big_number->tail);
		the_big_number->tail = NULL;
		the_big_number->head = NULL;
	}
}

void BN_del_head (big_number *the_big_number) {
	if (the_big_number->head->next) {
		the_big_number->head = the_big_number->head->next;
		free(the_big_number->head->previous);
		the_big_number->head->previous = NULL;
	}
	else {
		free(the_big_number->head);
		the_big_number->head = NULL;
		the_big_number->tail = NULL;
	}
}

void BN_add_digit_in_tail (big_number *number, char the_new_digit) {
	printf("BN_add_digit_in_tail\n");
	node *new_digit;
	new_digit = node_init();
	new_digit->digit = the_new_digit;
	if (number->tail) {
		printf("number->tail\n");
		new_digit->previous = number->tail;
		number->tail->next = new_digit;
		number->tail = new_digit;
	}
	else {
	number->tail = new_digit;
	number->head = new_digit;
	}
}

void BN_add_digit_in_head (big_number *number, char the_new_digit) {
	node *new_digit;
	new_digit = node_init();
	new_digit->digit = the_new_digit;
	if (number->head) {
		new_digit->next = number->head;
		number->head->previous = new_digit;
		number->head = new_digit;
	}
	else {
	number->head = new_digit;
	number->tail = new_digit;
	}
}

big_number* BN_get (char sign, char the_first_digit) {
	printf("BN_get\n");
	big_number *number;
	number = BN_init();
	number->sign = sign;
	node *first_digit;
	first_digit = node_init();
	first_digit->digit = the_first_digit;
	number->head = first_digit;
	number->tail = first_digit;

	int c;
	while (((c = getchar()) != '\n') && (c != EOF)) {
		c -= '0';
		if (!((0 <= c) && (c <= 9))) {
			printf("Wrong symbol in number\n");
		}
		else {
			BN_add_digit_in_tail(number, c);
		}
	}
	if (ferror(stdin)) {
		printf("An error occurred while reading the numbers\n");
		exit(0);
	}
	BN_del_leading_zeros(number);
	return number;
}

big_number* BN_addition (big_number *a, big_number *b) {
	char tmp = 0;
	big_number *result;
	result = BN_init();
	while((a->tail) && (b->tail)) {
		BN_add_digit_in_head(result, ((a->tail->digit + b->tail->digit + tmp) % 10));
		tmp = (a->tail->digit + b->tail->digit) / 10;
		BN_del_tail(a);
		BN_del_tail(b);
	}
	while (a->tail) {
		BN_add_digit_in_head(result, ((a->tail->digit + tmp) % 10));
		tmp = (a->tail->digit + tmp) / 10;
		BN_del_tail(a);
	}
	while (b->tail) {
		BN_add_digit_in_head(result, ((b->tail->digit + tmp) % 10));
		tmp = (b->tail->digit + tmp) / 10;
		BN_del_tail(b);
	}
	if(tmp){
		BN_add_digit_in_head(result, tmp);
	}
	free(a);
	free(b);
	return result;
}

void BN_del_leading_zeros (big_number *the_big_number) {
	while((the_big_number->head) && (the_big_number->head->digit == 0) && (the_big_number->head->next)) {
		the_big_number->head = the_big_number->head->next;
		free(the_big_number->head->previous);
		the_big_number->head->previous = NULL;
	}
}

void BN_print (big_number *the_big_number) {
	if (the_big_number->sign) {
		printf("-");
	}
	node *next_to_print;
	next_to_print = the_big_number->head;
	while (next_to_print) {
		printf("%c", (next_to_print->digit + '0'));
		next_to_print = next_to_print->next;
	}
	printf("\n");
}