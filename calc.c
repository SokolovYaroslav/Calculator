#include <stdio.h>
#include <stdlib.h>
#include "big_number_get.h"
#include "stack.h"

int main () {
	int c;
	big_number *a;
	big_number *b;
	stack_launch();
	while((c = getchar()) != EOF) {
		if (c == '+') {
			if (getchar() != '\n') {
				printf("Incorrect input, try again\n");
				while (getchar() != '\n');
				continue;
			}
			else {
				if (stack_size() < 2) {
					printf("Not enough numbers to add\n");
				}
				else {
					a = stack_pop();
					b = stack_pop();
					if (a->sign != b->sign) {
						stack_push(BN_subtraction(a, b));
						continue;
					}
					else {
						stack_push(BN_addition(a, b));
						continue;
					}
				}
			}
			if (ferror(stdin)) {
				printf("An error occurred while reading the numbers\n");
				return 0;
			}
		}
		else if (c == '-') {
			if ((c = getchar()) != '\n') {
				if (((0 + '0') <= c) && (c <= (9 + '0'))) {
					stack_push(BN_get(1, c - '0'));
				}
				else {
					printf("Incorrect input, try again\n");
					while (getchar() != '\n');
					continue;
				}
			}
			else if (stack_size() < 2) {
				printf("Not enough numbers to subtract\n");
			}
			else {
				b = stack_pop();
				a = stack_pop();
				b->sign = (b->sign + 1) % 2;
				if (a->sign != b->sign) {
					stack_push(BN_subtraction(a, b));
					continue;
				}
				else {
					stack_push(BN_addition(a, b));
					continue;
				}
			}
		}
		else if (c == '*') {
			if (getchar() != '\n') {
				printf("Incorrect input, try again\n");
				while (getchar() != '\n');
				continue;
			}
			else {
				if (stack_size() < 2) {
					printf("Not enough numbers to add\n");
				}
				else {
					stack_push(BN_multiplication(stack_pop(), stack_pop()));
					continue;
				}
			}
			if (ferror(stdin)) {
				printf("An error occurred while reading the numbers\n");
				return 0;
			}
		}
		else if (c == '/') {
			if (getchar() != '\n') {
				printf("Incorrect input, try again\n");
				while (getchar() != '\n');
				continue;
			}
			else {
				if (stack_size() < 2) {
					printf("Not enough numbers to add\n");
				}
				else {
					b = stack_pop();
					if (b->head->digit == 0) {
						printf("Divison by zero\n");
						stack_push(BN_get_zero());
						continue;
					}
					else {
						stack_push(BN_division(stack_pop(), b));
						continue;
					}
				}
			}
			if (ferror(stdin)) {
				printf("An error occurred while reading the numbers\n");
				return 0;
			}
		}
		else if (((0 + '0') <= c) && (c <= (9 + '0'))) {
			stack_push(BN_get(0, c - '0'));
		}
		else if (c == '=') {
			BN_print(stack_last());
		}
	}
	if (ferror(stdin)) {
		printf("An error occurred while reading the numbers\n");
		return 0;
	}
	while (stack_size() > 0) {
		BN_del(stack_pop());
	}
	stack_clear();
	return 1;
}