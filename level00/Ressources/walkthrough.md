# Level00

The program asks the user to input a password.  
By looking at the code of the decompiled executable, we see that if the password is equal to `0x149c`, or `5276` in decimal, we get access to a shell with the rights of the user `level01`.

To access the shell, we just have to enter `5276` when we are prompted for a password.