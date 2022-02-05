#include	<stdio.h>
#include	<stdlib.h>
#include	"memcheck.h"

typedef struct AllocRecord {
	size_t size;
	const char* file;
	int line;
	const char* func;
	void* address;
	struct AllocRecord* next;
} AllocRecord;

static void* (*real_malloc)(size_t) = malloc;
static void* (*real_calloc)(size_t, size_t) = calloc;
static void* (*real_realloc)(void*, size_t) = realloc;
static void (*real_free)(void*) = free;

static AllocRecord* records = NULL;		// A linked list, hold all allocation records

/* Add a record into records */
static AllocRecord* add_record(size_t size, const char* file, int line, const char* func, void* address) {
	AllocRecord* record = (AllocRecord*)real_malloc(sizeof(AllocRecord));
	record->size = size;
	record->file = file;
	record->line = line;
	record->func = func;
	record->address = address;
	record->next = NULL;
	record->next = records;
	records = record;
	return record;
}

void* _memcheck_malloc(size_t size, const char* file, int line, const char* func) {
	return add_record(size, file, line, func, real_malloc(size))->address;
}

void* _memcheck_calloc(size_t num, size_t size, const char* file, int line, const char* func) {
	return add_record(size * num, file, line, func, real_calloc(num, size))->address;
}

void* _memcheck_realloc(void* ptr, size_t size, const char* file, int line, const char* func) {
	// Remove the record of the original ptr in records first

	AllocRecord* cur = records;
	AllocRecord* pre = NULL;

	while(cur != NULL) {
		if(cur->address == ptr) {
			if(pre != NULL)
				pre->next = cur->next;
			else
				records = cur->next;

			real_free(cur);
			break;
		}

		pre = cur;
		cur = cur->next;
	}

	return add_record(size, file, line, func, real_realloc(ptr, size))->address;
}

void _memcheck_free(void* ptr) {
	AllocRecord* cur = records;
	AllocRecord* pre = NULL;

	while(cur != NULL) {
		if(cur->address == ptr) {
			if(pre != NULL)
				pre->next = cur->next;
			else
				records = cur->next;

			real_free(ptr);
			real_free(cur);
			break;
		}

		pre = cur;
		cur = cur->next;
	}
}

static void memcheck() {
	AllocRecord* cur = records;
	AllocRecord* pre = NULL;

	if(cur == NULL) {
		fprintf(stderr, "No memory leak");
		return;
	}

	fprintf(stderr, "Memory leaks:\n");

	while(cur != NULL) {
		fprintf(stderr, "  %llu bytes at 0x%p (allocate from %s:%d in %s)\n", cur->size, cur->address, cur->file, cur->line, cur->func);
		pre = cur;
		cur = cur->next;
		real_free(pre);
	}
}

void apply_memcheck() {
	atexit(memcheck);
}