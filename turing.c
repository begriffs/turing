#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAPELEN (1L << 20)

bool tape[TAPELEN];

struct rule
{
	bool write;
	bool halt;
	short movehead; /* -1, 1 */
	short nextstate;
} program[2]['Z'-'A'];

bool parse_rules(void)
{
	char clump[4];
	int onvalue = 0;
	short curstate = 0;

	/* rule H is special */
	program[0]['H'-'A'] = program[1]['H'-'A'] =
		(struct rule){.halt = true};
	while (fgets(clump, sizeof clump, stdin))
	{
		if (clump[0] != '0' && clump[0] != '1')
		{
			fprintf(stderr,
			        "Invalid write value: %c\n", clump[0]);
			return false;
		}
		if (clump[1] != 'L' && clump[1] != 'R')
		{
			fprintf(stderr,
			        "Invalid direction: %c\n", clump[1]);
			return false;
		}
		if (clump[2] < 'A' || clump[2] > 'Z')
		{
			fprintf(stderr,
			        "Invalid next state: %c\n", clump[2]);
			return false;
		}
		program[onvalue][curstate] =
			(struct rule)
			{
				.write     = clump[0] == '1',
				.movehead  = clump[1] == 'R' ? 1 : -1,
				.nextstate = clump[2] - 'A'
			};
		if (onvalue++ == 1)
		{
			onvalue = 0;
			if (++curstate == 'H'-'A')
				curstate++; /* skip state H */
		}
		getchar(); /* consume space */
	}
	if (onvalue == 1)
	{
		fputs("Error: odd number of rules\n", stderr);
		return false;
	}
	return true;
}

int main(void)
{
	size_t head = TAPELEN/2;
	short state = 0;
	struct rule *r;
	unsigned long steps = 0;
	if (!parse_rules())
		return EXIT_FAILURE;
	while (true)
	{
		r = &program[tape[head]][state];
		if (r->halt)
			break;
		tape[head] = r->write;
		head += r->movehead;
		if (head > TAPELEN) /* also catches <0 wraparound */
		{
			fprintf(stderr, "Tape out of bounds\n");
			return EXIT_FAILURE;
		}
		state = r->nextstate;
		steps++;
	}
	printf("Finished in %lu steps\n", steps);
	return 0;
}
