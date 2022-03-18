#include "mem.h"
#include "mem_debug.h"
#include "mem_internals.h"
#include "util.h"

static void* map_pages(void const* addr, size_t length, int additional_flags) {
  return mmap( (void*) addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | additional_flags , -1, 0 );
}

struct block_header* find_last_block(struct block_header* h) {
    struct block_header *ret = h;
    while (ret->next) {
        ret = ret->next;
    }
    return ret;
}

void test1(struct block_header* h) {
    debug_heap(stderr, h);
    debug("Обычное успешное выделение памяти\n");
    void* d = _malloc(64);
    debug_heap(stderr, h);
    _free(d);
}

void test2(struct block_header* h) {
    debug_heap(stderr, h);
    debug("Освобождение одного блока из нескольких выделенных\n");
    void* d1 = _malloc(64);
    void* d2 = _malloc(128);
    debug_heap(stderr, h);
    _free(d1);
    debug_heap(stderr, h);
    _free(d2);
}

void test3(struct block_header* h) {
    debug_heap(stderr, h);
    debug("Освобождение двух блоков из нескольких выделенных\n");
    void* d1 = _malloc(64);
    void* d2 = _malloc(128);
    void* d3 = _malloc(64);
    debug_heap(stderr, h);
    _free(d2);
    _free(d3);
    debug_heap(stderr, h);
    _free(d1);
}

void test4(struct block_header* h) {
    debug_heap(stderr, h);
    debug("Память закончилась, новый регион памяти расширяет старый\n");
    void* d1 = _malloc(100000);
    void* d2 = _malloc(20000);
    debug_heap(stderr, h);
    _free(d1);
    _free(d2);
}

void test5(struct block_header* h) {
    debug_heap(stderr, h);
    debug("Память закончилась, старый регион памяти не расширить из-за другого выделенного диапазона адресов, новый регион выделяется в другом месте\n");
    map_pages(find_last_block(h), 5000, 0);
    void* d1 = _malloc(100000);
    void* d2 = _malloc(20000);
    debug_heap(stderr, h);
    _free(d1);
    _free(d2);
}

int main() {
    struct block_header* h = (struct block_header*) heap_init(100000);
    test1(h);
    test2(h);
    test3(h);
    test4(h);
    test5(h);
    debug_heap(stderr, h);
}