# file_encryptor

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
Enter a read buffer size: 20
Enter a write buffer size: 20
Reset requested
Total input count with current key is  143
A:13 B:3 C:1 D:5 E:19 F:2 G:2 H:10 I:7 J:0 K:0 L:4 M:0 N:6 O:5 P:2 Q:0 R:9 S:4 T:13 U:2 V:1 W:2 X:0 Y:2 Z:1
Total output count with current key is  143
A:1 B:13 C:3 D:1 E:5 F:19 G:2 H:2 I:10 J:7 K:0 L:0 M:4 N:0 O:6 P:5 Q:2 R:0 S:9 T:4 U:13 V:2 W:1 X:2 Y:0 Z:2
Reset finished

Reset requested
Total input count with current key is  963
A:74 B:8 C:8 D:32 E:110 F:16 G:12 H:56 I:43 J:1 K:10 L:44 M:15 N:41 O:48 P:19 Q:0 R:54 S:37 T:82 U:12 V:6 W:19 X:0 Y:9 Z:1
Total output count with current key is  963
A:48 B:19 C:0 D:54 E:37 F:82 G:12 H:6 I:19 J:0 K:9 L:1 M:74 N:8 O:8 P:32 Q:110 R:16 S:12 T:56 U:43 V:1 W:10 X:44 Y:15 Z:41
Reset finished

Reset requested
Total input count with current key is  1351
A:96 B:13 C:20 D:56 E:149 F:26 G:21 H:66 I:54 J:0 K:8 L:72 M:18 N:60 O:76 P:27 Q:0 R:60 S:76 T:96 U:17 V:16 W:26 X:0 Y:13 Z:2
Total output count with current key is  1351
A:56 B:149 C:26 D:21 E:66 F:54 G:0 H:8 I:72 J:18 K:60 L:76 M:27 N:0 O:60 P:76 Q:96 R:17 S:16 T:26 U:0 V:13 W:2 X:96 Y:13 Z:20
Reset finished

Reset requested
Total input count with current key is  962
A:75 B:12 C:11 D:36 E:95 F:17 G:20 H:57 I:46 J:0 K:4 L:36 M:10 N:45 O:47 P:14 Q:0 R:48 S:51 T:74 U:9 V:11 W:23 X:0 Y:8 Z:1
Total output count with current key is  962
A:75 B:12 C:11 D:36 E:95 F:17 G:20 H:57 I:46 J:0 K:4 L:36 M:10 N:45 O:47 P:14 Q:0 R:48 S:51 T:74 U:9 V:11 W:23 X:0 Y:8 Z:1
Reset finished

Reset requested
Total input count with current key is  674
A:45 B:8 C:15 D:28 E:71 F:10 G:6 H:43 I:34 J:1 K:9 L:33 M:11 N:28 O:30 P:7 Q:0 R:28 S:37 T:46 U:10 V:5 W:14 X:1 Y:6 Z:0
Total output count with current key is  674
A:71 B:10 C:6 D:43 E:34 F:1 G:9 H:33 I:11 J:28 K:30 L:7 M:0 N:28 O:37 P:46 Q:10 R:5 S:14 T:1 U:6 V:0 W:45 X:8 Y:15 Z:28
Reset finished

Total input count with current key is  111
A:8 B:0 C:1 D:5 E:9 F:1 G:1 H:3 I:3 J:0 K:1 L:3 M:0 N:3 O:7 P:3 Q:0 R:5 S:3 T:4 U:2 V:0 W:4 X:0 Y:1 Z:0
Total output count with current key is  111
A:7 B:3 C:0 D:5 E:3 F:4 G:2 H:0 I:4 J:0 K:1 L:0 M:8 N:0 O:1 P:5 Q:9 R:1 S:1 T:3 U:3 V:0 W:1 X:3 Y:0 Z:3
End of file reached
```

input and resulting output file can be viewed under `file_encryptor/input.txt` and  `file_encrypter/output.txt`

### input/output excerpts

input.txt 
```
As they rounded a bend in the path that ran beside the river, Lara recognized the silhouette of a fig tree atop a nearby hill. The weather was hot and the days were long. The fig tree was in full leaf, but not yet bearing fruit.
Soon Lara spotted other landmarks—an outcropping of limestone beside the path that had a silhouette like a man’s face, a marshy spot beside the river where the waterfowl were easily startled, a tall tree that looked like a man with his arms upraised. They were drawing near to the place where there was an island in the river. The island was a good spot to make camp. They would sleep on the island tonight.
Lara had been back and forth along the river path many times in her short life. Her people had not created the path—it had always been there, like the river—but their deerskin-shod feet and the wooden wheels of their handcarts kept the path well worn. Lara’s people were salt traders, and their livelihood took them on a continual journey.
At the mouth of the river,
```

output.txt 

```
Bt uifz spvoefe b cfoe jo uif qbui uibu sbo cftjef uif sjwfs, Mbsb sfdphojafe uif tjmipvfuuf pg b gjh usff bupq b ofbscz ijmm. Uif xfbuifs xbt taf mzp ftq pmke iqdq xazs. Ftq rus fdqq ime uz rgxx xqmr, ngf zaf kqf nqmduzs rdguf.
Eaaz Xmdm ebaffqp aftqd xmzpymdwe—mz agfodabbuzs ar xuyqefazq nqeupq ftq bmft ftmf tmp m euxtagqffq xuwq m ymz’e rmoq, m ymdetk ebaf nqeupq ftq duhqd itqdq ftq imfqdraix iqdq qmeuxk efmdfxqp, m fmxx fdqq ftmf xaawqp xuwq m ymz iuft tue mdye gbdmueqp. Ftqk iqdq pdmiuzs zqmd fa ftq bxmoq itqdq ftqdq ime mz uexmzp uz ftq duhqd. Ftq uexmzp ime m saap ebaf fa ymwq omyb. Ftqk iagxp exqqb az ftq uexmzp fazustf.
Xmdm tmp nqqz nmow mzp radft mxazs ftq duhqd bmft ymzk fuyqe uz tqd etadf xurq. Tqd bqabxq tmp zaf odqmfqp ftq bmft—uf tmp mximke nqqz ftqdq, xuwq ftq duhqd—ngf ftqud pqqdewuz-etap rqqf mzp ftq iaapqz itqqxe ar ftqud tmzpomdfe wqbf ftq bmft iqxx iadz. Xmdm’e bqabxq iqdq emxf fdmpqde, mzp ftqud xuhqxutaap faaw ftqy az m oazfuzgmx vagdzqk.
Mf ftq yagft ar ftq duhqd,
```




