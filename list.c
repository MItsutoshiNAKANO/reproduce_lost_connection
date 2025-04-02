/// @file
/// @brief
/// @copyright
///   2025 Mitsutoshi Nakano
///   SPDX-License-Identifier: AGPL-3.0
/// @author Mitsutoshi Nakano <ItSANgo@gmail.com>
/// @version 0.1.0
/// @date 2025-04-02

#include <stdlib.h>

#include "list.h"

struct list *
create_list(void)
{
    struct list *l = malloc(sizeof *l);
    if (l == NULL) {
        return NULL;
    }
    l->next = l;
    l->prev = l;
    l->data = NULL;
    return l;
}

struct list *
push_to_list(struct list *head, void *data)
{
    struct list *last = malloc(sizeof *last);
    if (last == NULL) {
        return NULL;
    }
    last->next = head;
    last->prev = head->prev;
    head->prev->next = last;
    head->prev = last;
    last->data = data;
    return last;
}
