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

#include <stdlib.h>

#include "syntaxtree.h"
#include "function.h"

extern const FunctionRecord functionTable[];

SyntaxTree *newSyntaxTree()
{
	SyntaxTree *tree = malloc(sizeof(SyntaxTree));
	
	if (tree != NULL)
	{
		tree->next = tree->firstChild = NULL;
		tree->numChildren = 0;
	}

	return tree;
}

void freeSyntaxTree(SyntaxTree *root)
{
	if (root == NULL) return;
	
	freeSyntaxTree(root->next);
	freeSyntaxTree(root->firstChild);
	
	free(root);
}

int evaluateSyntaxTree(SyntaxTree **head)
{
	Token t = (*head)->token;
	
	if (t.type == VALUE)
	{
		free (*head);
		*head = NULL;
		return t.value.number;
	}
	
	else if (t.type == FUNCTION)
	{
		SyntaxTree *child;
		unsigned i;
		int value;
		int *argList;
		unsigned int numChildren = (*head)->numChildren;
		const FunctionRecord fn = functionTable[t.value.fnIndex];

		if ((fn.argType == ARGS_FIXED && numChildren != fn.numArgs) || (fn.argType == ARGS_VARIADIC && numChildren < fn.numArgs))
			die("Invalid number of arguments");
		
		argList = calloc(numChildren, sizeof(int));

		for (i = 0, child = (*head)->firstChild; child != NULL; i++, child = child->next)
			argList[i] = evaluateSyntaxTree(&child);

		value = fn.function(numChildren, argList);

		free (*head);
		*head = NULL;

		return value;
	}

	die("Malformed syntax tree.");
}