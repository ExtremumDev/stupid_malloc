#include <unistd.h>

#include "malloc.h"

extern int p_size;
extern int current_page_number;
extern void *heap_start;
extern struct alloc *first_alloc;

void s_free(void *pointer)
{
    struct alloc *whole_all = pointer - META_SIZE;
    
    if(whole_all->prev != NULL){
       whole_all->prev->next = whole_all->next;
    }

    if(whole_all->next != NULL){
        whole_all->next->prev = whole_all->prev;
    } else{
        /* We are at the end of data in heap -> check if we can decrease the break */
        if(whole_all->prev != NULL){
            int diff;
            int byte_position = ((void *) whole_all->prev + META_SIZE + whole_all->prev->size) - heap_start;

            diff = current_page_number * p_size - byte_position;

            if(diff >= p_size){ /* One or more page are emplty and have not data -> reduce heap size */
               int page_needed = byte_position / p_size + (byte_position % p_size != 0 ? 1 : 0); 
               sbrk((page_needed - current_page_number) * p_size);
               current_page_number = page_needed;
            }

        }else if(current_page_number > 1){
            /* There are no more allocatiob in heap, so we need only one page, other are meaningless */
            sbrk(p_size - current_page_number * p_size);
        }
    }

    if(whole_all->prev == NULL){
        first_alloc = whole_all->next;
    }
}

         

