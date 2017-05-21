#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(void)
{
	printf("Usage: \n");
	printf("    Reads the output of size from stdin and computes percentage of used memory.\n\n");
	printf("    -s <sram size in bytes>\n");
	printf("        Required: Must be greater than zero.\n");
	printf("    -f <flash size in bytes>\n");
	printf("        Required: Must be greater than zero.\n");
}

int main(int argc, char *argv[]) {
	double flash = 0;
	double sram = 0;

	double text = 0;
	double data = 0;
	double bss = 0;

	int opt;
	char *end;
	
	char *input = NULL;
	size_t count = 0;

	while ((opt = getopt(argc, argv, "s:f:")) != -1)
	{
		switch (opt)
		{
			case 's':
				sram = strtod(optarg, &end);
				break;
			case 'f':
				flash = strtod(optarg, &end);
				break;
			default:
				usage();
				return -1;
		}
	}
	
	if (flash <= 0 || sram <= 0) goto error;

	ssize_t bytes = getline(&input, &count, stdin);

	if (bytes == -1)
	{
		free(input);
		input = NULL;
		goto error;
	}
	
	free(input); // Dump first line
	input = NULL;

	bytes = getline(&input, &count, stdin);
	char *ptr = input;

	if (bytes == -1)
	{
		free(input);
		input = NULL;
		ptr = NULL;
		goto error;
	}

	text = strtod(ptr, &end);

	if (text < 0)
	{
		free(input);
		input = NULL;
		ptr = NULL;
		goto error;
	}

	ptr = end;
	data = strtod(ptr, &end);

	if (data < 0)
	{
		free(input);
		input = NULL;
		ptr = NULL;
		goto error;
	}

	ptr = end;
	bss = strtod(ptr, &end);

	if (bss < 0)
	{
		free(input);
		input = NULL;
		ptr = NULL;
		goto error;
	}

	printf("Flash usage: %d bytes, %.1f %%\n", (int)(text + data), ((text + data) / flash) * 100.0);
	printf("Ram usage:   %d bytes, %.1f %%\n", (int)(data + bss), ((data + bss) / sram) * 100.0);

	return 0;

error:
	usage();
	return -1;
}