#include <string.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	int sVar1;
	int local_18;
	int local_14;
	(void)argc;

	sVar1 = strcspn(argv[1],"\n");
	argv[1][sVar1] = '\0';
	local_14 = ((int)argv[1][3] ^ 0x1337U) + 0x5eeded;
	for (local_18 = 0; local_18 < (int)sVar1; local_18 = local_18 + 1) {
			if (argv[1][local_18] < ' ') {
					return 1;
			}
			local_14 = local_14 + ((int)argv[1][local_18] ^ local_14) % 0x539;
			printf("local14: %d\n", local_14);
	}
	return 0;
}
