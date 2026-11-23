#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* items;
    size_t len;
    size_t cap;
} Vec;

Vec* init();
int push(Vec* v, int val);
void drop(Vec* v);
void print(Vec* v);

int main() {
    Vec* v = init();
    if (v == NULL) {
        fprintf(stderr, "failed to allocate memory\n");
        return 1;
    }
    for (int i = 0; i < 10; i++) {
        push(v, i);
    }
    print(v);
    drop(v);

    return 0;
}

Vec* init() {
    Vec* v = malloc(sizeof *v);
    if (v == NULL) {
        return NULL;
    }

    v->cap = 8;
    v->len = 0;
    v->items = malloc(v->cap * sizeof *v->items);

    if (v == NULL) {
        free(v);
        return NULL;
    }

    return v;
}

int push(Vec* v, int val) {
    if (v == NULL)
        return 0;

    if (v->len == v->cap) {
        size_t new_cap = v->cap == 0 ? 8 : v->cap * 2;
        int* new_items = realloc(v->items, new_cap * sizeof *v->items);
        if (new_items == NULL)
            return 0;

        v->items = new_items;
        v->cap = new_cap;
    }

    v->items[v->len++] = val;
    return 1;
}

void drop(Vec* v) {
    if (v == NULL)
        return;
    free(v->items);
    free(v);
}

void print(Vec* v) {
    if (v == NULL) {
        return;
    }
    printf("[");
    for (size_t i = 0; i < v->len; i++) {
        if (i != 0)
            printf(", ");
        printf("%d", v->items[i]);
    }
    printf("]\n");
}
