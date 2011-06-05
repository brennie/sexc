#ifndef INCLUDE_FUNCTION_H
#define INCLUDE_FUNCTION_H
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

#define FN_NAME_MAX 32

typedef double Function(unsigned int, double *);

typedef enum {
		ARGS_NULL,
		ARGS_FIXED,
		ARGS_VARIADIC
}
ArgType;

 typedef struct
{
	const char *name;
	Function *function;

	ArgType argType;

	unsigned int numArgs;
}
FunctionRecord;

int lookupFunction(const char *name);

#endif