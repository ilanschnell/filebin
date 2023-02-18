filebin
=======

filebin is a tiny C program, which counts the occurrence of each byte
in a file (or from stdin):
```
$ ./filebin README.md
...
# cat README.md | ./filebin
...
```


Installation
------------

Run make:
```
$ make test
cc -Wall -o filebin main.c
./filebin Makefile
ASCII (32..126)........... :          94
   0 (NUL)................ :           0
   9 (Tab)................ :           3
  10 (LF)................. :           8
  11 (VTab)............... :           0
  13 (CR)................. :           0
 127 (Del) ............... :           0
Others (1..8,12,14..31) .. :           0
Others (128..255) ........ :           0
                             -----------
Total .................... :         105
```
then copy the `filebin` executable in some preferred directory
listed in `PATH`.
