#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

int sim_resol(unsigned char *iv, unsigned char *key, int byte_index, unsigned char out);

int main(int argc, char *argv[])
{
	FILE *fd;
	unsigned char tmp_line[256];
	unsigned char counts[256];
	unsigned char key[5];
	int byte_index, read, tmp, i, max, max_index;
	unsigned char plain = 'C';
	unsigned char out;
	memset(key, 0, 5);
	for (byte_index = 0; byte_index < 5; byte_index++)
	{
		memset(counts, 0, 256);
		fd = fopen(argv[1], "r");
		read = fread(tmp_line, sizeof(unsigned char), 143, fd);
		while (read != 0)
		{
			if ((tmp_line[24] == byte_index + 3) && (tmp_line[25] == 255))
			{
				out = plain^tmp_line[27];
				tmp = sim_resol(&tmp_line[24], key, byte_index, out);
				if ((tmp >= 0) && (tmp <= 255))
					counts[tmp]++;
			}
			read = fread(tmp_line, sizeof(unsigned char), 143, fd);
		}
		max = 0;
		max_index = 0;
		for (i = 0; i<256; i++)
		{
			if (counts[i] > max)
			{
				max_index = i;
				max = counts[i];
			}
		}
		key[byte_index] = (unsigned char)max_index;
	}

	for (i = 0; i < 5; i++)
	{
		cout << key[i];
	}
	cout << endl;
	return 0;
}


int sim_resol(unsigned char *iv, unsigned char *key, int byte_index, unsigned char out)
{
	unsigned char s[256];
	unsigned char a, s_1, k;
	unsigned char tmp_key[8];
	int i;
	int j;
	int sin;
	memcpy(tmp_key, iv, 3);
	memcpy(&tmp_key[3], key, 5);
	for (i = 0; i<256; i++)
		s[i] = i;
	j = k = 0;
	for (i = 0; i<byte_index + 3; i++)
	{
		j = (j + s[i] + tmp_key[k]);
		j = j % 256;
		a = s[i];
		s[i] = s[j];
		s[j] = a;
		if (++k >= 8)
			k = 0;
	}
	for (i = 0; i<256; i++)
		if (s[i] == out)
			s_1 = i;
	if ((s[0] != byte_index + 3) || (s[1] != 0))
		return -1;
	else
		sin = (s_1 - j - s[byte_index + 3]);
	sin = sin % 256;
	if (sin<0)
		sin = sin + 256;
	return sin;
}
