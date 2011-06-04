#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H
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

#include "token.h"
#include "syntaxtree.h"

void interpret(FILE *input, FILE *output);
SyntaxTree *matchExpression(FILE *input);
SyntaxTree *matchFunction(FILE *input);
void matchType(FILE *input, TokenType type);
SyntaxTree *getFunction(FILE *input);
#endif