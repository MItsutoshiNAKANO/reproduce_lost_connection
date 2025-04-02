#ifndef LIST_H
#define LIST_H

/// @file
/// @brief List data structure

struct list {
    struct list *next;
    struct list *prev;
    char *data;
};

struct list *create_list(void);
struct list *push_to_list(struct list *head, void *data);

#endif // LIST_H
