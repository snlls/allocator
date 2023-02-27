#include "allocator.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>


// dump list allocator for debug purposes
struct adata;

typedef struct adata {
	void *data;
	struct adata *next;
} adata;

static adata global_adata = {0};
allocator *allocator_global = &global_adata;

void allocator_init(allocator *a)
{
	assert(a);
	memset(a, 0, sizeof(*a));
}

void allocator_uninit(allocator *a)
{
	assert(a);	
	adata *tmp = a;
	if(tmp->data) {
		free(tmp->data);
		tmp->data = 0;
	}
	tmp = tmp->next;
	
	while(tmp) {
		if(tmp->data) {
			free(tmp->data);
			tmp->data = 0;
		}
		adata *t = tmp;
		tmp = tmp->next;
		free(t);
	}
}


/** @brief print debug info to buffer */
char *allocator_str(allocator *a, char *buf, int buf_len)
{
	assert(a);
	assert(buf);
	int nodes = 0;
	int allocs = 0;
	adata *tmp = a;
	while(tmp) {
		nodes++;
		if(tmp->data)
			allocs++;
		tmp = tmp->next;
	}
	(void)snprintf(buf, buf_len, "allocator: nodes %d allocs %d", nodes, allocs);
	return buf;
}

void *allocator_alloc(allocator *a, size_t size)
{
	assert(a);
	if(!size)
		return 0;
	adata *tmp = a;
	while(tmp) {
		if(!tmp->data) {
			tmp->data = malloc(size);
			assert(tmp->data);
			return tmp->data;
		}
		if(!tmp->next) {
			tmp->next = malloc(sizeof(adata));
			assert(tmp->next);
			tmp->next->next = 0;
			tmp->next->data = malloc(size);
			assert(tmp->next->data);
			return tmp->next->data;
		}
		tmp = tmp->next;
	}
	return 0;
}

void allocator_free(allocator *a, void *p)
{
	assert(a);
	if(!p)
		return;
	adata *tmp = a;
	while(tmp) {
		if(tmp->data == p) {
			free(tmp->data);
			tmp->data = 0;
			return;
		}
		tmp = tmp->next;
	}
}
