CC=g++
FLAG=-g
#FLAG=-02

all: libftp.a demo18 ftpgetfile

libftp.a:ftplib.c
	gcc -c -o libftp.a ftplib.c

demo18:demo18.cpp _ftp.h _ftp.cpp libftp.a
	$(CC) $(FLAG) -o demo18 demo18.cpp _public.cpp _ftp.cpp libftp.a -lm -lc

ftpgetfile:ftpgetfile.cpp _ftp.h _ftp.cpp libftp.a
	$(CC) $(FLAG) -o ftpgetfile ftpgetfile.cpp _public.cpp _ftp.cpp libftp.a -lm -lc
	cp ftpgetfile ../bin/.

clean:
	rm -rf crtsurfdata
	rm -rf demo18
