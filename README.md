# file_encryptor
A multi-threaded file encryptor which applies the Caesar cipher to an input file


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


