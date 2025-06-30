# Level01

The program first asks the user to input a username.  
If the username is correct, it then asks for a password.

By looking at the code of the decompiled executable, we find that the username is written in clear: `dat_wil`.

As for the password, it doesn’t matter what the `verify_user_pass` function returns, the program always ends up in the same condition and prints `nope, incorrect password...` using a `puts`.

However, the password is stored in a buffer of size 64 (`char local_54[64];`), and the user input is read using `fgets(local_54, 100, stdin);`.
This means we can overflow the buffer and take control of the execution flow.

## Finding the Offset

To find the offset where the return address is overwritten:

```bash
run < <(python -c 'print "dat_wil"'; python -c 'print "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag"')
```
Offset = 80

## Exploitation Strategy

When we are prompted for a password, we are going to enter:
```
shellcode + padding + address of the shellcode
```

- Shellcode: 21 bytes
- Padding: 80 (offset) - 21 (shellcode size) = 59 bytes
- Address of shellcode: The address where the second fgets() is writing the input

Instead of executing the puts() function, our shellcode will be executed, giving us a shell.