#include <stdio.h>
#include <stdlib.h>
#include "big_number_get.h"

void BN_add_digit (big_number *number, unsigned char the_new_digit) {
	node *new_digit;
	new_digit = (node*)malloc(sizeof(node*));
	new_digit->digit = the_new_digit;
	new_digit->previous = number->tail;
	new_digit->next = NULL;
	number->tail->next = new_digit;
	number->tail = new_digit;
}

big_number* BN_get (char sign, unsigned char the_first_digit) {
	big_number *number;
	number = (big_number*)malloc(sizeof(big_number*));
	number->sign = sign;
	node *first_digit;
	first_digit = (node*)malloc(sizeof(node*));
	first_digit->digit = the_first_digit;
	number->head = first_digit;
	first_digit->previous = NULL;
	int c;
	while ((c = getchar()) != '\n') {
		if (((0 + '0') <= c) && (c <= (9 + '0'))) {
			printf("Wrong symbol in number\n");
			continue;
		}
		BN_add_digit(number, c);
	}
	if (ferror(stdin)) {
		printf("An error occurred while reading the numbers\n");
		exit(0);
	}
	return number;
}