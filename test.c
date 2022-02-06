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