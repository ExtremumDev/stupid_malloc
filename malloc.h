/* Header file for intern use(not a part of library) */

struct alloc {
    struct alloc *next, *prev;
    int size;
};

#define META_SIZE sizeof(struct alloc)
