# Level04

## What the binary does
When running the level04 binary, it creates a child process using fork().

In the child, it prints:
```
Give me some shellcode, k
```

Then it calls `gets()` on a local buffer of size 128. That’s our classic buffer overflow entry point.
But this vulnerable code is only executed by the child, not the parent.

## Anti-exec protection

Before we can do anything with the shellcode, the parent is watching the child.
It uses `ptrace()` to monitor the child’s system calls.
If it detects that the child tries to execute execve, it prints `"no exec() for you"` and kills the child.

So, we can’t use execve shellcode like we did in the previous levels.
Instead of using execve, we can call `system("/bin/sh")` directly.

##  Finding the offset

As usual, we need to know how far to overflow the buffer to reach the return address.

To make GDB follow the child instead of the parent (since that’s where the overflow happens), we run:
```
set follow-fork-mode child
```

We then send a pattern like "wiremask" and check where it lands.
The offset to the return address is 156 bytes.

##  Building the exploit
We’ll overwrite the return address with the address of system(), followed by 4 bytes of padding, and then the address of the string "/bin/sh".

Why 4 bytes of padding?
Because when calling a function like system(), the first value pushed onto the stack (after return address) is usually the argument, in this case, the address of "/bin/sh".
But the return address itself also needs a placeholder, so we add 4 bytes to pad over it.

## Finding the addresses
In GDB, we need to break before the child executes (in the gets call).

### Get the address of system():
```
p system
```

### Get libc memory range:
```
info proc map
```
We need to note down the start and end addresses of libc.

### Find the `/bin/sh` string inside libc:
```
find <libc_start>, <libc_end>, "/bin/sh"
```
This gives us the address to use as an argument to system().

## The final payload

system() address is 0xf7e6aed0
"/bin/sh" address is 0xf7f897ec

```
(python -c 'print "\x90"*156 + "\xd0\xae\xe6\xf7" + "A"*4 + "\xec\x97\xf8\xf7"'; cat) | ./level04
```

This sends:
- \x90 * 156: padding (NOPs)
- \xd0\xae\xe6\xf7: address of system() (little endian)
- "A" * 4: padding for the return address of system()
- \xec\x97\xf8\xf7: address of /bin/sh
