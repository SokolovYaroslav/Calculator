#include <stdio.h>
#include <stdlib.h>
#include "big_number_get.h"

node* node_create () {
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

big_number* BN_create () {
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

void BN_add_digit_in_tail (big_number *number, long long int the_new_digit) {
	node *new_digit;
	new_digit = node_create();
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

void BN_add_digit_in_head (big_number *number, long long int the_new_digit) {
	node *new_digit;
	new_digit = node_create();
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

big_number_in_array* BN_get_in_array (char the_first_digit) {
	big_number_in_array *number;
	number = (big_number_in_array*)malloc(sizeof(big_number_in_array));
	int capacity = 1024;
	number->array = (char*)malloc(sizeof(char) * capacity);
	number->array[0] = the_first_digit;
	number->size = 1;
	int c;
	while (((c = getchar()) != '\n') && (c != EOF)) {
		if (number->size >= capacity) {
			capacity *= 2;
			number->array = (char*)realloc(number->array, sizeof(char) * capacity);
		}
		if (!((0 + '0' <= c) && (c <= 9 + '0'))) {
			continue;
		}
		else {
			number->array[number->size++] = c;
		}
	}
	if (ferror(stdin)) {
		printf("An error occurred while reading the numbers\n");
		exit(0);
	}
	return number;
}

big_number* BN_get (char sign, char the_first_digit) {
	big_number *number;
	number = BN_create();
	number->sign = sign;
	big_number_in_array *number_in_array = BN_get_in_array(the_first_digit);
	char digit[10];
	digit[9] = '\n';
	for (int i = (number_in_array->size - 1); i >= 0; i -= 9) {
		int j = 0;
		for (j = 0; ((j < 9) && ((i - j) >= 0)); j++) {
			digit[8 - j] = number_in_array->array[i - j];
		}
		BN_add_digit_in_head(number, strtoll(digit + (9 - j), NULL, 10));
	}
	free(number_in_array->array);
	free(number_in_array);
	return number;
}

big_number* BN_get_zero () {
	big_number *new_number = BN_create();
	node *zero = node_create();
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
	long long int tmp = 0;
	big_number *result;
	result = BN_create();
	result->sign = a->sign;
	while((a->tail) && (b->tail)) {
		BN_add_digit_in_head(result, ((a->tail->digit + b->tail->digit + tmp) % 1000000000));
		tmp = (a->tail->digit + b->tail->digit + tmp) / 1000000000;
		BN_del_tail(a);
		BN_del_tail(b);
	}
	while (a->tail) {
		BN_add_digit_in_head(result, ((a->tail->digit + tmp) % 1000000000));
		tmp = (a->tail->digit + tmp) / 1000000000;
		BN_del_tail(a);
	}
	while (b->tail) {
		BN_add_digit_in_head(result, ((b->tail->digit + tmp) % 1000000000));
		tmp = (b->tail->digit + tmp) / 1000000000;
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
	result = BN_create();
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
			a->tail->digit += 1000000000;
			a->tail->previous->digit--;
		}
		BN_add_digit_in_head(result, (a->tail->digit - b->tail->digit));
		BN_del_tail(a);
		BN_del_tail(b);
	}
	while (a->tail) {
		if (a->tail->digit < 0) {
			a->tail->digit += 1000000000;
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
	if (BN_abs_compare(a, b) == 0) {
		while (a->head) {
			BN_del_tail(a);
		}
		return;
	}
	node *current_node_a = a->tail;
	node *current_node_b = b->tail;
	while (current_node_b) {
		if (current_node_a->digit < current_node_b->digit) {
			current_node_a->digit += 1000000000;
			current_node_a->previous->digit--;
		}
		current_node_a->digit -= current_node_b->digit;
		current_node_a = current_node_a->previous;
		current_node_b = current_node_b->previous;
	}
	while (current_node_a) {
		if (current_node_a->digit < 0) {
			current_node_a->digit += 1000000000;
			current_node_a->previous->digit--;
		}
		current_node_a = current_node_a->previous;
	}
	BN_del_leading_zeros(a);
}

big_number* BN_multiplication (big_number *a, big_number *b) {
	if (((a->size == 1) && (a->head->digit == 0)) ||
		((b->size == 1) && (b->head->digit == 0))) {
		big_number *result;
		result = BN_create();
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
		big_number *result = BN_create();
		for (i = 0; i < (a->size + b->size); i++) {
			BN_add_digit_in_head(result, 0);
		}
		node *current_node_a = a->tail;
		node *current_node_b = b->tail;
		node *current_node_result = result->tail;
		node *starting_node_result = result->tail;
		long long int old_current_digit_result;
		while (current_node_b) {
			while (current_node_a) {
				old_current_digit_result = current_node_result->digit;
				current_node_result->digit = ((current_node_a->digit * current_node_b->digit) + current_node_result->digit + tmp) % 1000000000;
				tmp = ((current_node_a->digit * current_node_b->digit) + old_current_digit_result + tmp) / 1000000000;
				current_node_a = current_node_a->previous;
				current_node_result = current_node_result->previous;
			}
			while (tmp) {
				old_current_digit_result = current_node_result->digit;
				current_node_result->digit = (current_node_result->digit + tmp) % 1000000000;
				tmp = (old_current_digit_result + tmp) / 1000000000;
				current_node_result = current_node_result->previous;
			}
			current_node_a = a->tail;
			starting_node_result = starting_node_result->previous;
			current_node_result = starting_node_result;
			current_node_b = current_node_b->previous;
			BN_del_tail(b);
		}
		while (amount_of_zero > 0) {
			BN_add_digit_in_tail(result, 0);
			amount_of_zero--;
		}
		result->sign = (a->sign + b->sign) % 2;
		BN_del_leading_zeros(result);
		BN_del(a);
		free(b);
		return result;
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
	big_number *result = BN_create();
	if (BN_abs_compare(a, b) == -1) {
		BN_add_digit_in_head(result, 0);
		result->sign = (a->sign + b->sign) % 2;
		if (result->sign) {
			BN_del(result);
			big_number *new_result = BN_create();
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
		printf("into division\n");
		big_number *prefix = BN_create();
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
			long long int result_digit = 0;
			long long int power = 1000000000;
			if (BN_abs_compare(prefix, b) >= 0) {
				printf("q\n");
				BN_add_digit_in_tail(b, 0);
				for (int i = 8; i >= 0; i--) {
					printf("qqq\n");
					node *current_node = prefix->head;
					char tmp = 0;
					long long int new_prefix_digit = 0;
					while (current_node) {
						new_prefix_digit = (current_node->digit / 10) + (tmp * 1000000000);
						tmp = current_node->digit % 10;
						current_node->digit = new_prefix_digit;
						current_node = current_node->next; 
					}
					BN_del_leading_zeros(prefix);
					power /= 10;
					while (BN_abs_compare(prefix, b) >= 0) {
						BN_subtraction_for_division(prefix, b);
						result_digit += power;
					}
				}
			}
			if ((prefix->size == 1) && (prefix->head->digit == 0)) {
				BN_del_head(prefix);
			}
			BN_add_digit_in_tail(result, result_digit);
		}
		if (prefix->head) {
			if (result->sign) {
				node *current_node_result = result->tail;
				result->tail->digit += 1;
				while (current_node_result) {
					if (current_node_result->digit > (1000000000 - 1)) {
						current_node_result->digit -= 1000000000;
						current_node_result->previous->digit++;
					}
					current_node_result = current_node_result->previous;
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
		node *current_node_a = a->head;
		node *current_node_b = b->head;
		while (current_node_a) {
			if (current_node_a->digit < current_node_b->digit) {
				return -1;
			}
			else if (current_node_a->digit > current_node_b->digit) {
				return 1;
			}
			else {
				current_node_a = current_node_a->next;
				current_node_b = current_node_b->next;
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
	printf("%lld", next_to_print->digit);
	next_to_print = next_to_print->next;
	while (next_to_print) {
		printf("%.9lld", next_to_print->digit);
		next_to_print = next_to_print->next;
	}
	printf("\n");
}
