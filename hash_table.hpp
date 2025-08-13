#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
class hash_table
{
	enum {hash_table_sz = 1024};
	enum {chain_length = 50};
	enum {buf_cap = 50};
	struct bucket
	{
		double value;
		char buf[buf_cap];
	};
	struct chain
	{
		bucket b[chain_length];
		int chain_cap;
	};
	chain table[hash_table_sz];
	int hash_cap;
	char* buf;
	int hash(int key);
	void init();
	int search(int index, const char* b);
public:	
	hash_table();
	void insert(int fd, double v, const char* b);
	~hash_table();
};
#endif
