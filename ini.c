/// @file
/// @brief
/// @copyright
///   2025 Mitsutoshi Nakano
///   SPDX-License-Identifier: AGPL-3.0
/// @author Mitsutoshi Nakano <ItSANgo@gmail.com>
/// @version 0.1.0
/// @date 2025-04-02

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ini.h"
#include "list.h"

static size_t
chomp(char *str)
{
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
    return len;
}

void
destroy_ini(struct list *list)
{
    if (list == NULL) {
        return;
    }
    struct list *next;
    for (struct list *l = list->next; l != list; l = next) {
        next = l->next;
        struct key_value *kv = (struct key_value *)l->data;
        free(kv->key);
        free(kv->value);
        free(kv);
        free(l);
    }
    free(list);
}

struct list *
load_ini(FILE *fp)
{
    struct list *list = create_list();
    if (!list) {
        return NULL;
    }

    char line[INI_LINE_MAX];
    while (fgets(line, sizeof line, fp)) {
        chomp(line);
        char *p = strchr(line, '=');
        if (p == NULL) {
            continue; // Skip lines without '='
        }
        *p++ = '\0'; // Split the line into key and value
        struct key_value *kv = malloc(sizeof *kv);
        if (!kv) {
            destroy_ini(list);
            return NULL;
        }
        if ((kv->key = strdup(line)) == NULL) {
            free(kv);
            destroy_ini(list);
            return NULL;
        }
        if ((kv->value = strdup(p)) == NULL) {
            free(kv->key);
            free(kv);
            destroy_ini(list);
            return NULL;
        }
        push_to_list(list, kv);
    }
    return list;
}

char *
get_value(struct list *head, const char *key)
{
    for (struct list *list = head->next; list != head; list = list->next) {
        if (strcmp(((struct key_value *)list->data)->key, key) == 0) {
            return ((struct key_value *)list->data)->value;
        }
    }
    return NULL;
}
