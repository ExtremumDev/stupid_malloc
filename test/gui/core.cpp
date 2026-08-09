#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include "core.hpp"

extern "C"{
    void *s_malloc(int);
    void s_free(void *);
}

extern void *heap_start;

enum {
    window_w = 600,
    window_h = 300,
    heap_size = 512,
    heap_image_size = window_w - 20,
    heap_image_height = 60,
    heap_image_x_ident = 10,
    heap_image_y_ident = 10,
};

StupidMallocTest::StupidMallocTest()
{
    w = new Fl_Window(600, 300, "Stupid malloc GUI");
    heap_image = new Fl_Group(heap_image_x_ident, heap_image_y_ident, heap_image_size, heap_image_height);
    heap_image->box(FL_UP_BOX);
    heap_image->color(FL_BLACK);
    heap_image->end();
    w->end();
    heap_image->show();
    w->show();

    Fl::check();
}

void StupidMallocTest::update_window()
{
    heap_image->redraw();
    w->redraw();

    Fl::check();
    Fl::wait(0.1);
}

void *StupidMallocTest::malloc(int size)
{
    void *alloc = s_malloc(size);

    int ident = (char *)alloc - (char *)heap_start;

    double percent = ((double) ident) / heap_size;

    int block_start = heap_image_size * percent;
    int block_w = (size / (double) heap_size) * heap_image_size;

    heap_image->begin();
    Fl_Box *new_block = new Fl_Box(
        heap_image_x_ident + block_start,
        heap_image_y_ident,
        block_w,
        heap_image_height
    );
    new_block->box(FL_UP_BOX);
    new_block->color(FL_RED);
    heap_image->end();
    
    this->update_window();
    return alloc;
}