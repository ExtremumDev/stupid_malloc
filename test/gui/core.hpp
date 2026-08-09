#ifndef GUI_CORE_H_SENTRY
#define GUI_CORE_H_SENTRY

#include <FL/Fl_Window.H>

class StupidMallocTest{
    Fl_Window *w;
    Fl_Group *heap_image;
    
    void update_window();
public:
    StupidMallocTest();
    void *malloc(int size);
    void free(void *ptr);

};
#endif