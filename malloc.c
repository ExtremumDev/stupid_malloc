#include <unistd.h>

#ifdef BUFFER_VERSION
#include <stdlib.h>
#endif

#include "malloc.h"

int p_size = 0;
int current_page_number = 0;

void *heap_start = NULL;

struct alloc *first_alloc = NULL;

#ifdef BUFFER_VERSION
char *memory;
#endif


static void setup_constants()
{
    if(p_size == 0){
        p_size = getpagesize();
    }
#ifdef BUFFER_VERSION
    memory = malloc(p_size);
#endif

    if(heap_start == NULL){
#ifdef BUFFER_VERSION
        heap_start = memory;
#else
        heap_start = sbrk(0);
#endif
    }
}

static void *find_free_memory(int size, struct alloc **prev_p, struct alloc **next_p)
{
    void *prev_end = heap_start;                        
    *prev_p = NULL;
    *next_p = first_alloc;
     

    while(*next_p != NULL){
        if((void *) *next_p - prev_end >= size){             /* Memory area from first free byte after previous allocation */
                                                                /* to the last free byte before next allocation               */
            return prev_end;
        }
        prev_end = (void *) *next_p + META_SIZE + (*next_p)->size;
        *prev_p = *next_p;
        *next_p = (*next_p)->next;
    }

    return prev_end; /* return the end of actual data part */
}


static void check_heap_extend(void *section_end)
{
    int diff = (section_end - heap_start) - current_page_number * p_size;

    if(diff > 0){
        int page_need = diff / p_size + ( diff % p_size != 0 ? 1 : 0);

#ifdef BUFFER_VERSION
        memory = realloc((current_page_number + page_need) * p_size);
#else
        sbrk(page_need * p_size);
#endif
        current_page_number += page_need;
    }
}

void *s_malloc(int size)
{
    int allocation_size;
    struct alloc *new_section, *prev_section, *next_section;

    setup_constants();

    allocation_size = META_SIZE + size;

    new_section = find_free_memory(allocation_size, &prev_section, &next_section);

    check_heap_extend((void *)new_section + allocation_size);

    new_section->size = size;
    new_section->next = next_section;
    new_section->prev = prev_section;

    if(prev_section != NULL){
        prev_section->next = new_section;
    }else
        first_alloc = new_section;
    if(new_section->next != NULL){
        next_section->prev = new_section;
    }
    
    return (void *) new_section + META_SIZE;

}
