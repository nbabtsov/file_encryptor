# file_encryptor (ISU CS352 - Operating Systems, 2020)

A multi-threaded file encryptor which applies the Caesar cipher to an input file and outputs the result to a specified output file. 
To reduce predicatability, the encryption module randomly resets itself with a new encryption key. 


## How it works 
Five threads exist: main, encryption, reader, writer, input counter and output counter. Several semaphores are defined to control syncronization among these threads to avoid race conditions. 

![image](https://github.com/nbabtsov/file_encryptor/assets/31867318/71e3c286-5e1c-430b-aad4-9a1e0933c0cd)




#### Main Thread 
1. Obtains the input and output files as command line arguments. If the number of 
command line arguments is incorrect, it exits after displaying a message about correct usage. 
2. Calls init( ) with the input and output file names. 
3. Prompts the user for the input and output buffer sizes N and M. The buffers may be any 
size >1. 
4. Initialize shared variables. This includes allocating appropriate data structures for the 
input (N characters) and output (M characters) buffers.
5. Creates the other threads. 
6. Waits for all threads to complete. 
7. Displays the number of occurrences of each letter in the input and output files.
   
#### Encryption Thread
Consumes one character at a time from the input buffer, encrypts it (using `caesar_encrypt()`), and places it in the output buffer. The encryption thread may need to wait for an item to 
become available in the input buffer, and for a slot to become available in the output buffer. A character in the output buffer cannot be overwritten until the writer thread and the output 
counter thread have processed the character. The encryption thread continues until all characters of the input file have been encrypted.

#### Reader Thread 

Reads from the input file one character at a time (using `read_input()`), and places the characters in the input buffer. Each buffer item corresponds to a character. The reader thread may 
need to block until other threads have consumed data from the input buffer. Specifically, a character in the input buffer cannot be overwritten until the encryptor thread and the input 
counter thread have processed the character. The reader continues until the end of the input file is reached.

#### Writer Thread
Writes the encrypted characters in the output buffer to the output file (using `write_output`). It will block until an encrypted character is available in the buffer. The writer 
continues until it has written the last encrypted character. 

#### Input Counter Thread 
Counts occurrences of each letter (using `count_input()`) in the input file by looking at each character in the input buffer. It will block if no characters are available in the input buffer.

#### Outpt Counter Thread 
Counts occurrences of each letter (using `count_output()`) in the output file by looking at each character in the output buffer. It will block if no characters are available in the output buffer.

#### Encryption Module Reset 
To reduce predicatability, the encryption module randomly resets itself with a new encryption key. 
When this happens, the input and output counts are reset to zero. Before it performs a reset, it 
will inform the application by calling `reset_requested()`. After a reset is complete, it will call 
`reset_finished()`.These function calls are used to print the input and output counts before the 
reset, to ensure the correct behavior of the system during and after the reset. Only the 
characters encrypted using a particular key would be counted together and there would be no 
characters that go uncounted.

## Compile
```
nikababtsov@LAPTOP-VGHR5LTP:/mnt/c/Users/Nika/file_encryptor$ make
gcc -c encrypt-module.c
gcc -c encrypt-module.h
gcc -pthread encrypt-module.o -o encrypt352 -lm
```
## Sample Run 
```
nikababtsov@LAPTOP-VGHR5LTP:/mnt/c/Users/Nika/file_encryptor$ ./encrypt352 input.txt output.txt
Enter a read buffer size: 4
Enter a write buffer size: 4
Reset requested
Total input count with current key is  265
A:16 B:0 C:2 D:2 E:23 F:3 G:7 H:9 I:16 J:0 K:2 L:6 M:8 N:14 O:11 P:2 Q:0 R:11 S:11 T:19 U:10 V:7 W:0 X:1 Y:7 Z:0
Total output count with current key is  265
A:0 B:16 C:0 D:2 E:2 F:23 G:3 H:7 I:9 J:16 K:0 L:2 M:6 N:8 O:14 P:11 Q:2 R:0 S:11 T:11 U:19 V:10 W:7 X:0 Y:1 Z:7
Reset finished

Total input count with current key is  32
A:3 B:0 C:0 D:1 E:3 F:2 G:0 H:1 I:3 J:0 K:0 L:0 M:1 N:1 O:1 P:0 Q:0 R:2 S:0 T:2 U:1 V:1 W:0 X:0 Y:2 Z:0
Total output count with current key is  32
A:0 B:1 C:3 D:2 E:0 F:1 G:3 H:0 I:0 J:0 K:1 L:1 M:1 N:0 O:0 P:2 Q:0 R:2 S:1 T:1 U:0 V:0 W:2 X:0 Y:3 Z:0
End of file reached
```

input and resulting output file can be viewed under `file_encryptor/input.txt` and  `file_encrypter/output.txt`

### input/output excerpts

input.txt 
```
Seven : 	
"I've spent the last three years struggling to regain my Humanity. I'm afraid I may have lost it again in that arena."
Tuvok : 	
"You are experiencing difficult emotions."
Seven : 	
"Guilt, shame, remorse."
Tuvok : 	
"Then you haven't lost your Humanity. You have reaffirmed it."
```

output.txt 

```
Tfwfo : 	
"J'wf tqfou uif mbtu uisff zfbst tusvhhmjoh up sfhbjo nz Ivnbojuz. J'n bgsbje J nbz ibwf mptu ju bhbjo jo uibu bsfob."
Uvwpl : 	
"Zpv bsf fyqfsjfodjoh ejggjdvmu fnpujpot."
Tfwfo : 	
"Hvjmu, tibnf, sfnpstf."
Uvwpl : 	
"Uifo zpv ibwfo'u mptu zpvs Ivnylgrw. Wms fytc pcyddgpkcb gr."
```




