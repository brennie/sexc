/* sexc - s-expression calculator
 * Copyright (C) 2011  Barret Rennie
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token.h"
#include "function.h"

#define IS_SIGN_PREFIX(c) (c == '+' || c == '-')
#define IS_WHITESPACE(c) (c == ' ' || c == '\t' || c == '\n')
#define IS_SPECIAL(c) (c == '(' || c == ')' || IS_WHITESPACE(c) || c == EOF)

extern Token lookahead;

void die(const char *message)
{
	fprintf(stderr, "Error: %s.\n", message);
	exit(1);
}

int fpeek(FILE *stream)
{
	int c = fgetc(stream);
	if (ungetc(c, stream) == EOF && c != EOF)
		die("Stream error");

	return c;
}

void nextToken(FILE *input)
{
	char buffer[FN_NAME_MAX + 1];
	int c;	

	for (c = fgetc(input); IS_WHITESPACE(c); c = fgetc(input));
	
	if (c == EOF)
	{
#ifdef DEBUG
		printf("Got EOF\n");
#endif
		die("EOF Reached");
	}
	
	if (c == '(')
	{	
		lookahead.type = BEGIN;

#ifdef DEBUG
		printf("Got (\n");
#endif
	}
	else if (c == ')')
	{
		lookahead.type = END;

#ifdef DEBUG
		printf("Got )\n");
#endif
	}
	else if (IS_SIGN_PREFIX(c) && isdigit(fpeek(input)) || isdigit(c))
	{
		ungetc(c, input);

		lookahead.type = VALUE;

		fscanf(input, "%lf", &lookahead.value.number);

#ifdef DEBUG
		printf("Got number %f\n", lookahead.value.number);
#endif	
	}
	else
	{
		unsigned pos = 1;
		int index;

		lookahead.type = FUNCTION;

		buffer[0] = c & 0xFF;
			
		memset(buffer + 1, '\0', FN_NAME_MAX);
						
		c = fgetc(input);
		while (!IS_SPECIAL(c) && pos < FN_NAME_MAX)
		{
			buffer[pos++] = c & 0xFF;
			
			c = fgetc(input);
		}
		
		if (pos == FN_NAME_MAX && !IS_SPECIAL(c))
			die("token too long");

		ungetc(c, input);
			
		index = lookupFunction(buffer);

		if (index < 0)
			die("function not found");
		else
			lookahead.value.fnIndex = index;

#ifdef DEBUG
		printf("Got function: %d:%s\n", lookahead.value.fnIndex, buffer);
#endif

	}

}
