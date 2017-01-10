//Collin Dreher
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char str[100];
	int length = 0;
	char c;
	FILE *f;
	
	f = fopen(argv[1], "r");
	
	if(f == NULL)
	{
		printf("mystrings: No such file\n");
		return 0;
	}
	
	while(feof(f) == 0)
	{
		fread(&c, sizeof(c), 1, f);
		if(c > 31 && c < 127)
		{
			str[length] = c;
			length++;
			continue;
		}
	
		str[length] = '\0';
	
		if(4 <= length)
		{
			printf("%s\n", str);
		}
	
		length = 0;
		str[0] = '\0';
	}
	
	fclose(f);
	return 0;
}
