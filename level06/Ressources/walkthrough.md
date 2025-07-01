# Level06

When decompiling the binary, we find that the program:
- Takes two inputs: a login and a password.
- Calculates a hash from the login.
- Compares the hash with the password.

If the login has fewer than 6 characters, the function immediately returns 1.

If a debugger is detected via `ptrace(PTRACE_TRACEME)`, it prints a warning and exits.

## Password = hash(login)
The login is hashed, and the result is compared to the password.

The hash depends entirely on the content of the login string, so the trick is to find a login that produces the expected password.

## Exploitation Strategy
1. Choose a login string with more than 6 characters.
2. Recreate the hashing function in our own code.
3. Run the hash function on our login and get the resulting integer.
4. Use that integer as our password.

## C Code to Compute Hash
```
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
			printf("local14LOL: %d\n", local_14);
	}
	return 0;
}
```

## Result
If we run:
```
./a.out yourlogin
```

It will give you the correct password to enter for that login.