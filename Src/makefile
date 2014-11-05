# 标志
CC = D:\programs\CodeBlocks\MinGW\bin\gcc.exe
CFLAGS = -Wall -std=c99
# rm = rm -rf
# windows 平台
rm = del /q
INCLUDEPATH = -I"./include"
LIBPATH = -L"./lib"

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDEPATH)

SeedCup: IO.o html2DOM.o SeedCup.o
	$(CC) $(CFLAGS) $^ -o SeedCup.exe -lregex $(LIBPATH)

clean:
	$(rm) *.o SeedCup.exe output*.txt

install:
	cp SeedCup.exe ..\Bin\SeedCup.exe
