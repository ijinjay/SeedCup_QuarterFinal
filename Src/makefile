# 标志
CC = clang
CFLAGS = -Wall -std=c99
rm = rm -rf
# windows 平台
# rm = del /q

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

SeedCup: fileHandle.o SeedCup.o
	$(CC) $(CFLAGS) $^ -o SeedCup.exe

clean:
	$(rm) *.o SeedCup.exe output*.txt

install:
	cp SeedCup.exe ..\Bin\SeedCup.exe
