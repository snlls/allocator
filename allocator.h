

#include <stdlib.h>

typedef void allocator;

void allocator_fail_fn();
void allocator_fail() __attribute__ ((weak, alias ("allocator_fail_fn")));

void allocator_init(allocator *a);
void allocator_denit(allocator *a);

/** @brief print debug info to buffer */
char *allocator_str(allocator *a, char *buf, int buf_len);

void *allocator_alloc(allocator *a, size_t size);
void allocator_free(allocator *a, void *p);

extern allocator *allocator_global;

#define allocate(size) allocator_alloc(allocator_global, (size))
#define deallocate(p) allocator_free(allocator_global, (p))
