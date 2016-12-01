#include <stdio.h>
#include <stdlib.h>
#include "big_number_get.h"

node* node_init () {
	node *new_node;
	new_node = (node*)malloc(sizeof(node));
	if (!new_node) {
		printf("An error occurred while malloc node\n");
		exit(0);
	}
	new_node->digit = 0;
	new_node->previous = NULL;
	new_node->next = NULL;
	return new_node;
}

big_number* BN_init () {
	big_number *new_big_number;
	new_big_number = (big_number*)malloc(sizeof(big_number));
	if (!new_big_number) {
		printf("An error occurred while malloc big number\n");
		exit(0);
	}
	new_big_number->sign = 0;
	new_big_number->head = NULL;
	new_big_number->tail = NULL;
	new_big_number->size = 0;
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
	the_big_number->size--;
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
	the_big_number->size--;
}

void BN_add_digit_in_tail (big_number *number, char the_new_digit) {
	node *new_digit;
	new_digit = node_init();
	new_digit->digit = the_new_digit;
	if (number->tail) {
		new_digit->previous = number->tail;
		number->tail->next = new_digit;
		number->tail = new_digit;
	}
	else {
	number->tail = new_digit;
	number->head = new_digit;
	}
	number->size++;
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
	number->size++;
}

big_number* BN_get (char sign, char the_first_digit) {
	big_number *number;
	number = BN_init();
	number->sign = sign;
	node *first_digit;
	first_digit = node_init();
	first_digit->digit = the_first_digit;
	number->head = first_digit;
	number->tail = first_digit;
	number->size++;
	int c;
	while (((c = getchar()) != '\n') && (c != EOF)) {
		if (!((0 + '0' <= c) && (c <= 9 + '0'))) {
			continue;
		}
		else {
			BN_add_digit_in_tail(number, c - '0');
		}
	}
	if (ferror(stdin)) {
		printf("An error occurred while reading the numbers\n");
		exit(0);
	}
	BN_del_leading_zeros(number);
	return number;
}

big_number* BN_get_zero () {
	big_number *new_number = BN_init();
	node *zero = node_init();
	new_number->size++;
	new_number->tail = zero;
	new_number->head = zero;
	return new_number;
}

void BN_swap (big_number *a, big_number *b) {
	big_number tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

big_number* BN_addition (big_number *a, big_number *b) {
	char tmp = 0;
	big_number *result;
	result = BN_init();
	result->sign = a->sign;
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

big_number* BN_subtraction (big_number *a, big_number *b) {
	big_number *result;
	result = BN_init();
	if (BN_abs_compare(a, b) == 0) {
		BN_add_digit_in_head(result, 0);
		return result;
	}
	else if (BN_abs_compare(a, b) == -1) {
		BN_swap(a, b);
	}
	result->sign = a->sign;
	while (b->tail) {
		if (a->tail->digit < b->tail->digit) {
			a->tail->digit += 10;
			a->tail->previous->digit--;
		}
		BN_add_digit_in_head(result, (a->tail->digit - b->tail->digit));
		BN_del_tail(a);
		BN_del_tail(b);
	}
	while (a->tail) {
		if (a->tail->digit < 0) {
			a->tail->digit += 10;
			a->tail->previous->digit--;
		}
		BN_add_digit_in_head(result, a->tail->digit);
		BN_del_tail(a);
	}
	free(a);
	free(b);
	BN_del_leading_zeros(result);
	return result;
}

big_number* BN_multiplication (big_number *a, big_number *b) {
	if (((a->size == 1) && (a->head->digit == 0)) ||
		((b->size == 1) && (b->head->digit == 0))) {
		big_number *result;
		result = BN_init();
		BN_add_digit_in_head(result, 0);
		BN_del(a);
		BN_del(b);
		return result;
	}
	else if ((a->size == 1) && (a->head->digit == 1)) {
		if (a->sign == 1) {
			b->sign = (b->sign + 1) % 2;
		}
		BN_del(a);
		return b;
	}
	else if ((b->size == 1) && (b->head->digit == 1)) {
		if (b->sign == 1) {
			a->sign = (a->sign + 1) % 2;
		}
		BN_del(b);
		return a;
	}
	else {
		if (a->size < b->size) {
			BN_swap (a, b);
		}
		big_number *result = BN_init();
		while(a->tail->digit == 0) {
			BN_add_digit_in_head(result, 0);
			BN_del_tail(a);
		}
		while(b->tail->digit == 0) {
			BN_add_digit_in_head(result, 0);
			BN_del_tail(b);
		}
		long int tmp = 0;
		long int sum = 0;
		node *the_current_node_a = node_init();
		node *the_current_node_b = node_init();
		node *current_digit_a = node_init();
		node *current_digit_b = node_init();
		the_current_node_a = a->tail;
		the_current_node_b = b->tail;
		while (the_current_node_b) {
			current_digit_a = the_current_node_a;
			current_digit_b = the_current_node_b;
			while (current_digit_a && current_digit_b) {
				sum += current_digit_a->digit * current_digit_b->digit;
				current_digit_a = current_digit_a->next;
				current_digit_b = current_digit_b->previous;
			}
			BN_add_digit_in_head(result, ((sum + tmp) % 10));
			tmp = (sum + tmp) / 10;
			sum = 0;
			the_current_node_a = the_current_node_a->previous;
			if (!the_current_node_a) {
				the_current_node_a = a->head;
				the_current_node_b = the_current_node_b->previous;
			}
		}
		if (tmp > 0) {
			BN_add_digit_in_head(result, tmp);
		}
		result->sign = (a->sign + b->sign) % 2;
		BN_del(a);
		BN_del(b);
		return result;
	}
}

void BN_del_leading_zeros (big_number *the_big_number) {
	while((the_big_number->head) && (the_big_number->head->digit == 0) && (the_big_number->head->next)) {
		the_big_number->head = the_big_number->head->next;
		free(the_big_number->head->previous);
		the_big_number->head->previous = NULL;
		the_big_number->size--;
	}
}

char BN_abs_compare (big_number *a, big_number *b) {
	if (a->size < b->size) {
		return -1;
	}
	else if (a->size > b->size) {
		return 1;
	}
	else {
		node *current_digit_a;
		current_digit_a = node_init();
		current_digit_a = a->head;
		node *current_digit_b;
		current_digit_b = node_init();
		current_digit_b = b->head;
		while (current_digit_a) {
			if (current_digit_a->digit < current_digit_b->digit) {
				return -1;
			}
			else if (current_digit_a->digit > current_digit_b->digit) {
				return 1;
			}
			else {
				current_digit_a = current_digit_a->next;
				current_digit_b = current_digit_b->next;
			}
		}
		return 0;
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