#include <stdio.h>
#include <stdlib.h>
#include "big_number_get.h"
#include "stack.h"

int main () {
	int c;
	char natural_was_here = 0;
	while((c = getchar()) != EOF) {
		if (c == '+') {
			//BN_addition
			while ((c = getchar()) != '\n') {
				if (ferror(stdin)) {
					printf("An error occurred while reading the symbols\n");
					exit(0);
				}
			}
			continue;
		}
		else if (c == '*') {
			//BN_multiple
			while ((c = getchar()) != '\n') {
				if (ferror(stdin)) {
					printf("An error occurred while reading the symbols\n");
					exit(0);
				}
			}
			continue;
		}
		else if (c == '/') {
			//BN_division
			while ((c = getchar()) != '\n') {
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
				if (!natural_was_here) {
					if (c != (0 + '0')) {
						natural_was_here = 1;
					}
					else {
						continue;
					}
				}
				else {
					BN_get(1, c);
				}
			}
		}
		else if (((0 + '0') <= c) && (c <= (9 + '0'))) {
			if (!natural_was_here) {
				if (c != (0 + '0')) {
					natural_was_here = 1;
				}
				else {
					continue;
				}
			}
			else {
				BN_get(0, c);
			}
		}
		else if (c = '=') {
			//print last
		}
		natural_was_here = 0;
	}
	if (ferror(stdin)) {
		printf("An error occurred while reading the numbers\n");
		exit(0);
	}
	return 1;
}