#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef char *str;

void *mmalloc(size_t size);
str String(char *str);
