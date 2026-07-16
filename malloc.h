/* Header file for intern use(not a part of library) */

struct alloc {
    struct alloc *next;
    int size;
};
