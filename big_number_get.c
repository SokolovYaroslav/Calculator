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
	if (!the_big_number->head) {
		free(the_big_number);
		return;
	}
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
		tmp = (a->tail->digit + b->tail->digit + tmp) / 10;
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
	BN_del(a);
	BN_del(b);
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
	BN_del(a);
	BN_del(b);
	BN_del_leading_zeros(result);
	return result;
}

void BN_subtraction_for_division (big_number *a, big_number *b) {
	BN_print(a);
	BN_print(b);
	if (BN_abs_compare(a, b) == 0) {
		while (a->head) {
			BN_del_tail(a);
		}
		return;
	}
	node *current_digit_a;
	node *current_digit_b;
	current_digit_a = a->tail;
	current_digit_b = b->tail;
	while (current_digit_b) {
		if (current_digit_a->digit < current_digit_b->digit) {
			current_digit_a->digit += 10;
			current_digit_a->previous->digit--;
		}
		current_digit_a->digit -= current_digit_b->digit;
		current_digit_a = current_digit_a->previous;
		current_digit_b = current_digit_b->previous;
	}
	while (current_digit_a) {
		if (current_digit_a->digit < 0) {
			current_digit_a->digit += 10;
			current_digit_a->previous->digit--;
		}
		current_digit_a = current_digit_a->previous;
	}
	BN_del_leading_zeros(a);
	BN_print(a);
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
		int amount_of_zero = 0;
		while (a->tail->digit == 0) {
			amount_of_zero++;
			BN_del_tail(a);
		}
		while (b->tail->digit == 0) {
			amount_of_zero++;
			BN_del_tail(b);
		}
		int i = 0;
		int tmp = 0;
		big_number *result = BN_init();
		for (i = 0; i < (a->size + b->size); i++) {
			BN_add_digit_in_head(result, 0);
		}
		node *current_digit_a;
		current_digit_a = a->tail;
		node *current_digit_b;
		current_digit_b = b->tail;
		node *current_digit_result;
		current_digit_result = result->tail;
		node *the_current_node_result;
		the_current_node_result = result->tail;
		char old_current_digit_result;
		while (current_digit_b) {
			while (current_digit_a) {
				old_current_digit_result = current_digit_result->digit;
				current_digit_result->digit = ((current_digit_a->digit * current_digit_b->digit) + current_digit_result->digit + tmp) % 10;
				tmp = ((current_digit_a->digit * current_digit_b->digit) + old_current_digit_result + tmp) / 10;
				current_digit_a = current_digit_a->previous;
				current_digit_result = current_digit_result->previous;
			}
			while (tmp) {
				old_current_digit_result = current_digit_result->digit;
				current_digit_result->digit = (current_digit_result->digit + tmp) % 10;
				tmp = (old_current_digit_result + tmp) / 10;
				current_digit_result = current_digit_result->previous;
			}
			current_digit_a = a->tail;
			the_current_node_result = the_current_node_result->previous;
			current_digit_result = the_current_node_result;
			current_digit_b = current_digit_b->previous;
		}
		while (amount_of_zero > 0) {
			BN_add_digit_in_tail(result, 0);
			amount_of_zero--;
		}
		result->sign = (a->sign + b->sign) % 2;
		BN_del_leading_zeros(result);
		BN_del(a);
		BN_del(b);
		return result;
	/*	if (a->size < b->size) {
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
		return result;*/
	}
}

big_number* BN_division (big_number *a, big_number *b) {
	if (a->head->digit == 0) {
		BN_del(b);
		return a;
	}
	if ((b->size == 1) && (b->tail->digit == 1)) {
		if (b->sign) {
			a->sign = (a->sign + 1) % 2;
		}
		BN_del(b);
		return a;
	}
	big_number *result = BN_init();
	if (BN_abs_compare(a, b) == -1) {
		BN_add_digit_in_head(result, 0);
		result->sign = (a->sign + b->sign) % 2;
		if (result->sign) {
			BN_del(result);
			big_number *new_result = BN_init();
			new_result->sign = 1;
			BN_add_digit_in_head(new_result, 1);
			BN_del(a);
			BN_del(b);
			return new_result;
		}
		else {
			BN_del(a);
			BN_del(b);
			return result;
		}
	}
	else {
		big_number *prefix = BN_init();
		while (a->head) {
			BN_add_digit_in_tail(prefix, a->head->digit);
			BN_del_head(a);
			while ((BN_abs_compare(prefix, b) == -1) && a->head) {
				BN_add_digit_in_tail(prefix, a->head->digit);
				BN_del_head(a);
				BN_add_digit_in_tail(result, 0);
				if (!a->head) {
					break;
				}
			}
			BN_del_leading_zeros(prefix);
			char count = 0;
			while (BN_abs_compare(prefix, b) >= 0) {
				BN_subtraction_for_division(prefix, b);
				count++;
			}
			if ((prefix->size == 1) && (prefix->head->digit == 0)) {
				BN_del_head(prefix);
			}
			BN_add_digit_in_tail(result, count);
		}
		if (prefix->head) {
			if (result->sign) {
				node *current_digit_result;
				current_digit_result = result->tail;
				result->tail->digit += 1;
				while (current_digit_result) {
					if (current_digit_result->digit > 9) {
						current_digit_result->digit -= 10;
						current_digit_result->previous->digit++;
					}
					current_digit_result = current_digit_result->previous;
				}
			}
		}
		BN_del_leading_zeros(result);
		BN_del(a);
		BN_del(b);
		BN_del(prefix);
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
		current_digit_a = a->head;
		node *current_digit_b;
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