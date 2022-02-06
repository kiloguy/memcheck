## memcheck

A simple memory leak check tool for C

**Note**: This is a really simple tool, so it won't check for double free, allocate fail... . You need to check yourself and use it like normal `malloc`, `free` ...

### Usage

memcheck use macro to substitute `malloc`, `calloc`, `realloc`, and `free`

1. Include `memcheck.h` in the source files (.c) that needs to check memory leak
2. Call `apply_memcheck()` once in `main`, this is the only function in this tool
3. Compile `memcheck.c` with the source files

Then, memory leak check results will print to stderr when the program exits ([atexit](https://www.cplusplus.com/reference/cstdlib/atexit/) behavior)

### Example

`test.c`:

```c
#include	<stdio.h>
#include	"memcheck.h"

void foo() {
	char* c = (char*)malloc(10);
	char* d = (char*)realloc(c, 15);
}

int main() {
	apply_memcheck();

	int* a = (int*)malloc(sizeof(int));
	float* b = (float*)calloc(5, sizeof(float));
	foo();
	free(a);

	return 0;
}
```

Compile and run:

```bash
gcc -o test test.c memcheck.c
./test
```

Output:

```plaintext
Memory leaks:
  15 bytes at 0x0000000000A914E0 (allocate from test.c:6 in foo)
  20 bytes at 0x0000000000A91480 (allocate from test.c:13 in main)
```