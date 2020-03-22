

#include <iostream>
#include <string.h>


using namespace std;

const int MAX_POOLS = 1000;
const long BUF_SIZE = 104888320;

static char ALLBUF[BUF_SIZE];					
static unsigned long AVAIBLE = BUF_SIZE;		
static char* pools[MAX_POOLS];						
static unsigned int POOLS_COUNT = 1;
static unsigned int pools_size[MAX_POOLS];

static char* blocks[MAX_POOLS];
static unsigned int BLOCKS_COUNT = 0;
static unsigned int block_size[MAX_POOLS];
static unsigned int RIGHT_BLOCK;

// опрацювання помилок
static int ARS_ALLOC_ERR = 0;

#define NO_MEMORY 1
#define BLOCK_NOT_FOUND 2


void mem_init(void);						// ініціалізація
char* mem_alloc(unsigned long Size);		// виділення	
void mem_free(char*);						// виділення
int mem_frag(void);							// звільнення
char* mem_realloc(char*, unsigned long);

void mem_init(void)
{	//початок першого пулу
	pools[0] = ALLBUF;
	pools_size[0] = AVAIBLE;
}


char* mem_alloc(unsigned long Size)
{
	unsigned long int i, k;
	char* p;
	// опрацювання коли потрібно більше ніж є
	if (Size > AVAIBLE)
	{
		ARS_ALLOC_ERR = NO_MEMORY;
		return 0;
	}

	p = 0;
	// пошук першого пулу
	for (i = 0; i < POOLS_COUNT; ++i)
		if (Size <= pools_size[i])
		{
			p = pools[i];
			k = i;
			break;
		}

	if (!p)// пул не знайдено
	{
		ARS_ALLOC_ERR = NO_MEMORY;
		return 0;
	}

	blocks[BLOCKS_COUNT] = p;
	block_size[BLOCKS_COUNT] = Size;
	++BLOCKS_COUNT;
	++RIGHT_BLOCK;
	pools[k] = (char*)(p + Size + 1);// зміщення адреси початку на кінець
	pools_size[k] = pools_size[k] - Size;

	AVAIBLE -= Size;
	return p;
}


char* mem_realloc(char* p, unsigned long Size)
{
	unsigned long int i, k;
	char* newp;
	// опрацювання коли потрібно більше ніж є
	if (Size > AVAIBLE)
	{
		ARS_ALLOC_ERR = NO_MEMORY;
		return 0;
	}
	// пошук першого пулу
	for (i = 0; i < POOLS_COUNT; ++i)
		if (Size <= pools_size[i])
		{
			p = pools[i];
			k = i;
			break;
		}
	newp = p;
	if (!newp)// пул не знайдено
	{
		ARS_ALLOC_ERR = NO_MEMORY;
		return 0;
	}

	blocks[BLOCKS_COUNT] = newp;
	block_size[BLOCKS_COUNT] = Size;
	++BLOCKS_COUNT;
	++RIGHT_BLOCK;
	pools[k] = (char*)(newp + Size + 1);
	pools_size[k] = pools_size[k] - Size;

	AVAIBLE -= Size;
	return newp;
}

void mem_free(char* block)
{
	unsigned int i, k = 0;
	char* p = 0;
	// пошук блоку за адресою
	for (i = 0; i < RIGHT_BLOCK; ++i)
		if (block == blocks[i])
		{
			p = blocks[i];
			k = i;
			break;
		}

	if (!p)// блок не знайдено
	{
		ARS_ALLOC_ERR = BLOCK_NOT_FOUND;
	}

	blocks[k] = 0;
	--BLOCKS_COUNT;
	pools[POOLS_COUNT] = block;
	pools_size[POOLS_COUNT] = block_size[k];
	++POOLS_COUNT;
	AVAIBLE += block_size[k];

}



int mem_frag(void)
{
	unsigned int i, k;
	char* p = ALLBUF;
	char* t, * tmp;

	for (i = 0; i < RIGHT_BLOCK; ++i)
	{
		t = blocks[i];
		if (t == ALLBUF)
		{
			p = (char*)(blocks[i] + block_size[i] + 1);
			continue;
		}
		tmp = p;
		for (k = 0, t = blocks[i]; k < block_size[i]; ++k)
			*p++ = *t++;
		blocks[i] = tmp;
	}

	POOLS_COUNT = 1;
	pools[0] = p;
	AVAIBLE = BUF_SIZE - (unsigned long)(p - ALLBUF);
	pools_size[0] = AVAIBLE;
	RIGHT_BLOCK = 0;
	return 0;
}




int main()
{
	char x = 'g';
	mem_init();
	char* p = mem_alloc(22);

	cout << &p << endl;

	char* newp = mem_realloc(p, 35);

	cout << &newp << endl;

	mem_frag();

	mem_free(p);
	mem_free(newp);

	system("pause");
	return 0;
}

