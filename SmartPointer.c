#include <stdio.h>
#include <stdlib.h>

#define autofree __attribute__((cleanup(free_stack)))

__attribute__ ((always_inline))
inline void free_stack(void *p)
{
	free(*(void **) p);
}

int main()
{
	autofree int *i = malloc(sizeof(int) * 10);
	i[0] = 0;
	i[1] = 1;
	printf("SmartPointer Easy Test:%d\n", i[1]);
	return 0;
}
