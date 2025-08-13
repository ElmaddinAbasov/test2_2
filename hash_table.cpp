#include "hash_table.hpp"
inline int hash_table::hash(int key)
{
	return key % hash_table_sz;
}
void hash_table::init()
{
	int i;
	for (i = 0; i < hash_table_sz; i++)
		table[i].chain_cap = 0;
}
int hash_table::search(int index, const char* b)
{
	int i;
	for (i = 0; i < table[index].chain_cap; i++)
	{
		if (0 == strcmp(table[index].b[i].buf, b))
			return 1;
	}
	return 0;
}
hash_table::hash_table() : hash_cap(0)
{
	init();
}
void hash_table::insert(int fd, double v, const char* b)
{
	bucket my_bucket;
	size_t len;
	int index;
	index = hash(floor(v));
	if (search(index, b))
		return;
	len = strlen(b);
	if (len < buf_cap)
	{
		my_bucket.value = v;
		strcpy(my_bucket.buf, b);
		*(my_bucket.buf + len) = 0;
	}
	if (index >= 0 && index < hash_table_sz && table[index].chain_cap < chain_length)
	{
		if (!table[index].chain_cap)
			hash_cap++;
		table[index].b[table[index].chain_cap] = my_bucket;
		(table[index].chain_cap)++;
		lseek(fd, 0, SEEK_CUR);
		write(fd, my_bucket.buf, strlen(my_bucket.buf));
		lseek(fd, 0, SEEK_CUR);
		write(fd, "\n", strlen("\n"));
		return;
	}
	fprintf(stderr, "ERROR: Failed to add a new bucket to a hash table\n");
}
hash_table::~hash_table()
{
	hash_cap = 0;
}
