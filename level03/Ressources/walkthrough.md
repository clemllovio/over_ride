# Level03

When running the binary, it asks for a password. If the password is correct, it prints "Congratulations!" and gives us a shell. If not, we just get an "Invalid Password" message. 

## What the binary does
Looking inside the binary, we see a string: 
```
Q}|u`sfg~sf{}|a3
```

This string is compared to "Congratulations!", but not directly. Instead, it's XOR-decrypted first, and then compared.

The decryption key is calculated based on the password we enter. The program takes a hardcoded number (0x1337d00d) and subtracts our input from it. The result is used as the XOR key.

So to get "Congratulations!", we need to find the value that, when XORed with each character in the encrypted string, gives us the expected message.

## What is XOR?
XOR is a binary operation often used in encryption. The cool trick with XOR is that it’s reversible.
So, if we know the original and the encrypted value, we can get the key.

```
Encrypted:     Q } | u ` s f g ~ s f { } | a 3
Expected:      C o n g r a t u l a t i o n s !
```

By XORing the first character of each string:
```
'Q' (0x51) ^ 'C' (0x43) = 0x12
```

All the characters decrypt correctly with this same key: `0x12`.

##  Finding the password

Looking at the disassembly of main:
```
0x080488ca <+112>:	movl   $0x1337d00d,0x4(%esp)
0x080488d2 <+120>:	mov    %eax,(%esp)
0x080488d5 <+123>:	call   0x8048747 <test>
```

This tells us that:
   - param1 of the test function is our input (stored in %eax)
   - param2 is the hardcoded value: 0x1337d00d → 322424845 in decimal

We know from the decryption logic that:
```
param2 - param1 = 0x12
```

So:
```
param1 = param2 - 18
param1 = 322424845 - 18
param1 = 322424827
```

## Password
To get access to the shell we just enter `322424827` when we are prompted for a password.