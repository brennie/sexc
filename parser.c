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

#include "parser.h"
#include "token.h"
#include "syntaxtree.h"

Token lookahead;

//TODO: Null checking
SyntaxTree *matchExpression(FILE *input)
{
	SyntaxTree *head;

	if (lookahead.type == VALUE)
	{
		head = newSyntaxTree();
		head->token = lookahead;
	}
	else if (lookahead.type == BEGIN)
		head = matchFunction(input);

	nextToken(input);

	return head;
}

SyntaxTree *matchFunction(FILE *input)
{
	SyntaxTree *head, *expression, *children = NULL, *child;
	unsigned numChildren = 0;

	matchType(input, BEGIN);

	head = getFunction(input);

	while(lookahead.type != END)
	{
		numChildren++;

		expression = matchExpression(input);

		if (children == NULL)
		{
			children = expression;
			child = children->next;
		}

		child->next = expression;
		child = child->next;
	}

	if (children != NULL)
	{
		head->firstChild = children;
		head->numChildren = numChildren;
	}

	matchType(input, END);

	return head;
}

void matchType(FILE *input, TokenType type)
{
	if (lookahead.type != type)
		die("Expected different token.");

	nextToken(input);
}

SyntaxTree *getFunction(FILE *input)
{
	SyntaxTree *head;

	if (lookahead.type != FUNCTION)
		die("Expected function.");

	head = newSyntaxTree();
	head->token = lookahead;

	nextToken(input);

	return head;
}

void interpret(FILE *input, FILE *output)
{
	int value;
	SyntaxTree *head;
	nextToken(input); // Initialize lookahead

	head = matchExpression(input);

	value = evaluateSyntaxTree(&head);

	fprintf(output, "Result: %d\n", value);
}
