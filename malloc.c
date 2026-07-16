#include <unistd.h>

struct alloc {
    struct alloc *next;
    int size;
};


int p_size = 0;
int current_page_number = 0;
int meta_size = sizeof(struct alloc);

void *heap_start = NULL;

struct alloc *first_alloc = NULL;


static void setup_constants()
{
    if(p_size == 0){
        p_size = getpagesize();
    }

    if(heap_start == NULL){
        heap_start = sbrk(0);
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
        prev_end = (void *) *next_p + meta_size + (*next_p)->size;
        *prev_p = *next_p;
        *next_p = (*next_p)->next;
    }

    return prev_end; /* return the end of actual data part */
}


void check_heap_extend(void *section_end)
{
    int diff = (section_end - heap_start) - current_page_number * p_size;

    if(diff > 0){
        int page_need = diff / p_size + ( diff % p_size != 0 ? 1 : 0);

        sbrk((current_page_number + page_need) * p_size);

        current_page_number += page_need;
    }
}

void *s_malloc(int size)
{
    int allocation_size;
    struct alloc *new_section, *prev_section, *next_section;

    setup_constants();

    allocation_size = meta_size + size;

    new_section = find_free_memory(allocation_size, &prev_section, &next_section);

    check_heap_extend((void *)new_section + allocation_size);

    new_section->size = size;
    new_section->next = next_section;

    if(prev_section != NULL){
        prev_section->next = new_section;
    }

    if(prev_section == NULL)
        first_alloc = new_section;
    
    return (void *) new_section + meta_size;

}
