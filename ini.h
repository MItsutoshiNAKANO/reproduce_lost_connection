#ifndef INI_H
#define INI_H
/// @file
/// @brief INI file parser

#include <stdio.h>
#include <string.h>

#include "list.h"

#define INI_LINE_MAX 1024

struct key_value {
    char *key;
    char *value;
};

void destroy_ini(struct list *list);
struct list *load_ini(FILE *fp);
char *get_value(struct list *head, const char *key);

#endif // INI_H
