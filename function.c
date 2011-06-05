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

#include <string.h>

#include "function.h"

static double add(unsigned int numArgs, double *args)
{
	double sum = args[0];
	unsigned int i;

	for (i = 1; i < numArgs; i++)
		sum += args[i];

	return sum;
}

static double sub(unsigned int numArgs, double *args)
{
	return args[0] - args[1];
}

static double mul(unsigned int numArgs, double *args)
{
	double product = args[0];
	unsigned int i;

	for (i = 1; i < numArgs; i++)
		product *= args[i];

	return product;
}

//TODO: Handle division by 0 error
static double div(unsigned int numArgs, double *args)
{
	return args[0] / args[1];
}

const FunctionRecord functionTable[] = 
{
	{"+", add, ARGS_VARIADIC, 2},
	{"-", sub, ARGS_FIXED, 2},
	{"*", mul, ARGS_VARIADIC, 2},
	{"/", div, ARGS_FIXED, 2},
	{NULL, NULL, ARGS_NULL, 0}
};

int lookupFunction(const char *name)
{
	int i;
	for (i = 0; functionTable[i].name != NULL; i++)
		if (strcmp(functionTable[i].name, name) == 0)
			return i;

	return -1; // Not found
}