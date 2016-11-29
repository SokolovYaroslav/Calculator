#include <stdio.h>
#include <stdlib.h>
#include "big_number_get.h"
#include "stack.h"

int main () {
	int c;
	stack_launch();
	while((c = getchar()) != EOF) {
		if (c == '+') {
			if (stack_size() < 2) {
				printf("Not enough number to add\n");
				continue;
			}
			else {
				stack_push(BN_addition(stack_pop(), stack_pop()));
			}
			while (getchar() != '\n') {
				if (ferror(stdin)) {
					printf("An error occurred while reading the symbols\n");
					exit(0);
				}
			}
			continue;
		}
		else if (c == '*') {
			//BN_multiple
			while (getchar() != '\n') {
				if (ferror(stdin)) {
					printf("An error occurred while reading the symbols\n");
					exit(0);
				}
			}
			continue;
		}
		else if (c == '/') {
			//BN_division
			while (getchar() != '\n') {
				if (ferror(stdin)) {
					printf("An error occurred while reading the symbols\n");
					exit(0);
				}
			}
			continue;
		}
		else if (c == '-') {
			c = getchar();
			if (ferror(stdin)) {
				printf("An error occurred while reading the numbers\n");
				exit(0);
			}
			if (c == '\n') {
				//BN_subtraction
				continue;
			}
			else if (((0 + '0') <= c) && (c <= (9 + '0'))) {
				stack_push(BN_get(1, c - '0'));
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
		exit(0);
	}
	return 1;
}