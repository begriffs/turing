#include <stdbool.h>
#include <stdlib.h>

#define TAPELEN (1L << 20)
bool tape[TAPELEN];

int main(void)
{
	bool *head = tape + (TAPELEN/2);

A: /* 1RB 1LC */
	if (!*head) {
		*head++ = true;
		goto B;
	} else {
		*head-- = true;
		goto C;
	}
B: /* 1RC 1RB */
	if (!*head) {
		*head++ = true;
		goto C;
	} else {
		*head++ = true;
		goto B;
	}
C: /* 1RD 0LE */
	if (!*head) {
		*head++ = true;
		goto D;
	} else {
		*head-- = false;
		goto E;
	}
D: /* 1LA 1LD */
	if (!*head) {
		*head-- = true;
		goto A;
	} else {
		*head-- = true;
		goto D;
	}
E: /* 1RH 0LA */
	if (!*head) {
		*head++ = true;
		return EXIT_SUCCESS;
	} else {
		*head-- = false;
		goto A;
	}

	return EXIT_FAILURE; /* should not reach this */
}
