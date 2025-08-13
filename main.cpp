#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "hash_table.hpp"
#define FILE1 "Test_data.txt"
#define FILE2 "res.txt"
class test2
{
	enum {buf_sz = 1024};
	int fd1;
	int fd2;
	const char* fname1;
	const char* fname2;
	char buf[buf_sz];
	hash_table ht;
	char* storage;
	void open_file(const char* fname, int& fd, int flags, int mode);
	void close_file(int fd);
	void read_file();
	void write_to_file();
	double split(char* bbuf);
	void get_string(int& pos, char* string);
public:	
	test2();
	void run();
	~test2();
};
void test2::open_file(const char* fname, int& fd, int flags, int mode)
{
	errno = 0;
	fd = open(fname, flags, mode);
	if (fd == -1)
	{
		perror(fname);
		exit(1);
	}
}
void test2::close_file(int fd)
{
	int res;
	errno = 0;
	res = close(fd);
	if (res == -1)
	{
		perror("ERROR: Failed to close on of the file descriptors\n");
		exit(2);
	}
}
void test2::read_file()
{
	ssize_t cnt;
	errno = 0;
	cnt = read(fd1, buf, buf_sz);
	if (cnt == -1)
	{
		perror("ERROR: read_file() -> Failed to read from a file Test_data.txt\n");
		exit(3);
	}
	*(buf + cnt) = 0;
//	printf("%s\n", buf);
}
void test2::write_to_file()
{
#if 0	
	ssize_t cnt;
	size_t len;
	len = strlen(ht.get_buf());
	errno = 0;
	cnt = write(fd2, ht.get_buf(), len);
	if (cnt == -1)
	{
		perror("ERROR: write_to_file() -> Failed to write to a file res.txt\n");
		exit(4);
	}
#endif	
}
double test2::split(char* bbuf)
{
        char ibuf[buf_sz / 100];
        double d;
	int pos = 0;
        int len, index;
        len = strlen(bbuf);
        index = 0;
        d = 0.0;
        while (pos < len)
        {
                if ((*(bbuf + pos) >= '0' && *(bbuf + pos) <= '9') || *(bbuf + pos) == '.')
                {
                        *(ibuf + index) = *(bbuf + pos);
                        index++;
                        pos++;
                }
                if (*(bbuf + pos) == ' ' || *(bbuf + pos) == '\n' || !*(bbuf + pos))
                {
                        d += atof(ibuf);
                        *ibuf = 0;
                        index = 0;
                        pos++;
                }
        }
//	printf("NUMBER - %.1f\n", d);
        return d;	
}
void test2::get_string(int& pos, char* string)
{
	int i, j;
	j = 0;
	i = pos;
	while (*(buf + i))
	{
		if (*(buf + i) == '\n')
			break;
		*(string + j) = *(buf + i);
		i++;
		j++;
		pos = i;
	}
	*(buf + i) = 0;
	pos++;
}
void test2::run()
{
	int len, pos;
	char bbuf[buf_sz / 10];
	pos = 0;
	len = strlen(buf);
	for (;;)
	{
		if (pos >= len)
			break;
		get_string(pos, bbuf);
		ht.insert(fd2, split(bbuf), bbuf);
		*bbuf = 0;
	}
	write_to_file();
}
test2::test2() : fname1(FILE1), fname2(FILE2)
{
	open_file(fname1, fd1, O_RDONLY, 0);
	open_file(fname2, fd2, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	read_file();
//	write_to_file();
}
test2::~test2()
{
	close_file(fd1);
	close_file(fd2);
}
int main()
{
	test2 t;	
	t.run();
	exit(0);
}
