#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define autofree __attribute__((cleanup(free_stack)))

void sfree(void *ptr);

__attribute__ ((always_inline))
inline void free_stack(void *p)
{
	sfree(*(void **) p);
}

struct meta{
	void (*dtor)(void *);
	void *ptr;
};

__attribute__ ((malloc))
void *smalloc(size_t size, void(*dtor)(void *))
{
	void * p = malloc(sizeof(struct meta) + size);
	struct meta * m = (struct meta *)p;
	m->dtor = dtor;
	m->ptr = m + 1;
	return m->ptr;	
}

void sfree(void *ptr)
{
	void *tmp = ptr - sizeof(struct meta);
	struct meta *m = (struct meta *)tmp;
	assert(m->ptr == ptr);
	if (m->dtor != NULL) 
		m->dtor(ptr);
	free(m);
}

int main()
{
	autofree int *i = smalloc(sizeof(int) * 10, NULL);
	i[0] = 0;
	i[1] = 1;
	printf("SmartPointer Easy Test:%d\n", i[1]);
	return 0;
}
