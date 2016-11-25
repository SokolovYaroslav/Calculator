#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

static unsigned int stack_pointer = 0;
static big_number **stack;
static unsigned int stack_size = 1024;

void stack_launch () {
	stack = (big_number**)malloc(sizeof(big_number*) * 1024);
}

void stack_realloc () {
	stack_size *= 2;
	stack = (big_number**)realloc(stack, sizeof(big_number*) * stack_size);
}

void stack_push (big_number* new_big_number) {
	if (!(stack_size - stack_pointer)) {
		stack_realloc();
	}
	stack_pointer++;
	stack[stack_pointer] = new_big_number;
}

big_number* stack_pop () {
	return stack[stack_pointer--];
}

void stack_clear () {
	stack_pointer = 0;
	stack_size = 1024;
	free(stack);
}