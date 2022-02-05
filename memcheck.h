#ifndef		MEMCHECK_H
#define		MEMCHECK_H

#include	<stddef.h>

#define		malloc(size)		_memcheck_malloc(size, __FILE__, __LINE__, __FUNCTION__)
#define		calloc(num, size)	_memcheck_calloc(num, size, __FILE__, __LINE__, __FUNCTION__)
#define		realloc(ptr, size)	_memcheck_realloc(ptr, size, __FILE__, __LINE__, __FUNCTION__)
#define		free(ptr)			_memcheck_free(ptr)

// private, don't use them directly
void* _memcheck_malloc(size_t size, const char* file, int line, const char* func);
void* _memcheck_calloc(size_t num, size_t size, const char* file, int line, const char* func);
void* _memcheck_realloc(void* ptr, size_t size, const char* file, int line, const char* func);
void _memcheck_free(void* ptr);

// public
void apply_memcheck();

#endif