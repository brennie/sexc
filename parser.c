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
	else
		die("Expected expression");

	return head;
}

SyntaxTree *matchFunction(FILE *input)
{
	SyntaxTree *head, *expression, *children = NULL, *child;
	unsigned numChildren = 0;

	matchType(BEGIN);

	head = getFunction(input);

	nextToken(input);

	while(lookahead.type != END)
	{
		numChildren++;

		expression = matchExpression(input);

		if (children == NULL) // first child
		{
			children = expression;
			child = children;
		}
		else
		{
			child->next = expression;
			child = child->next;
		}
		
		nextToken(input);
	}

	if (children != NULL)
	{
		head->firstChild = children;
		head->numChildren = numChildren;
	}

	matchType(END);

	return head;
}

void matchType(TokenType type)
{
	if (lookahead.type != type)
		die("Expected different token.");
}

SyntaxTree *getFunction(FILE *input)
{
	SyntaxTree *head;

	nextToken(input);

	if (lookahead.type != FUNCTION)
		die("Expected function.");

	head = newSyntaxTree();
	head->token = lookahead;

	return head;
}

void interpret(FILE *input, FILE *output)
{
	while (!feof(input))
	{
		double value;
		SyntaxTree *head;
		
		nextToken(input);
	
		head = matchExpression(input);

		value = evaluateSyntaxTree(&head);
	
		fprintf(output, "Result: %f\n", value);

		freeSyntaxTree(head);
	}
}
