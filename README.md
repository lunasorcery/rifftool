rifftool
========
[![Build Status](https://travis-ci.org/willkirkby/rifftool.svg?branch=master)](https://travis-ci.org/willkirkby/rifftool) 

**rifftool** takes [RIFF](https://en.wikipedia.org/wiki/Resource_Interchange_File_Format)-formated files and dumps out the block structure in human-readable form.

Usage:

```
rifftool [options] <path> [path2 [path3...]]
```
 
Options:

```
-v, --verbose    Prints out a hexdump of the block data in addition to the block structure. 
```

Example usage:

```cpp
$ rifftool testfile.wav 
testfile.wav: 
0x0000000000000000: chunk 'RIFF'  40886 bytes
0x0000000000000008:  list 'WAVE'
0x000000000000000c:   chunk 'fmt '  16 bytes
0x0000000000000024:   chunk 'data'  40850 bytes
```
