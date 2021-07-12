#########################################
# 请注意，编译时，确定以下几点：
# 1、已安装oracle的客户端或oracle服务端程序，并且必须安装好OCI软件包
# 2、设置好ORACLE_HOME环境变量
#
#########################################

# oracle头文件路径
ORAINCL = -I$(ORACLE_HOME)/rdbms/public
# oracle库文件路径
ORALIB =  -L$(ORACLE_HOME)/lib -L.
# oracle的oci库
ORALIBS = -lclntsh

# mysql头文件存放路径
MYSQLINCL = -I/usr/include/mysql
# mysql库文件保存路径
MYSQLLIB = -L/usr/lib64/mysql
# mysql链接库
MYSQLLIBS = -lmysqlclient

# postgresql头文件存放路径
PGINCL = -I/usr/include
# postgresql库文件保存路径
PGLIB = -L/usr/lib64
# postgresql链接库
PGLIBS = -lpq

CC=g++

# CFLAGS = -O2
# CFLAGS = -O2 -Wall

CFLAGS = -g -Wno-write-strings -Wno-unused-variable


all: libftp.a demo11 demo12 demo13 demo14 demo18 ftpgetfiles deletefiles deletetables tcpputfiles\
     tcpfileserver tcpgetfiles exptables ftpputfiles dminoracle dminmysql dminpg httpclient wgetclient\
     hsmtables diskinfo cpuinfo meminfo tbspaceinfo tcpfileserver1 demo26 demo27 client server

libftp.a: ftplib.c
	gcc -c -o libftp.a ftplib.c

demo11: demo11.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o demo11 demo11.cpp _public.cpp -lm -lc

client: client.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o client client.cpp _public.cpp -lm -lc

server: server.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o server server.cpp _public.cpp -lm -lc

demo27: demo27.cpp _public.h  _public.cpp _ooci.h _ooci.cpp
	$(CC) $(CFLAGS) -o demo27 demo27.cpp  $(ORAINCL) $(ORALIB) $(ORALIBS) _public.cpp _ooci.cpp -lm -lc
	cp -f demo27 ../bin/.

demo28: demo28.cpp _public.h  _public.cpp _ooci.h _ooci.cpp
	$(CC) $(CFLAGS) -o demo28 demo28.cpp  $(ORAINCL) $(ORALIB) $(ORALIBS) _public.cpp _ooci.cpp -lm -lc
	cp -f demo28 ../bin/.

demo12: demo12.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o demo12 demo12.cpp _public.cpp -lm -lc

demo13: demo13.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o demo13 demo13.cpp _public.cpp -lm -lc

demo14: demo14.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o demo14 demo14.cpp _public.cpp -lm -lc

demo18: demo18.cpp _ftp.h _ftp.cpp  libftp.a
	$(CC) $(CFLAGS) -o demo18 demo18.cpp _public.cpp _ftp.cpp libftp.a -lm -lc

demo26: demo26.cpp _ftp.h _ftp.cpp  libftp.a
	$(CC) $(CFLAGS) -o demo26 demo26.cpp _public.cpp _ftp.cpp libftp.a -lm -lc

tcpputfiles: tcpputfiles.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o tcpputfiles tcpputfiles.cpp _public.cpp -lm -lc
	cp -f tcpputfiles ../bin/.

tcpgetfiles: tcpgetfiles.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o tcpgetfiles tcpgetfiles.cpp _public.cpp -lm -lc
	cp -f tcpgetfiles ../bin/.

tcpfileserver: tcpfileserver.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o tcpfileserver tcpfileserver.cpp _public.cpp -lm -lc
	cp -f tcpfileserver ../bin/.

tcpfileserver1: tcpfileserver1.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o tcpfileserver1 tcpfileserver1.cpp _public.cpp -lpthread -lm -lc
	cp -f tcpfileserver1 ../bin/.

ftpputfiles: ftpputfiles.cpp _ftp.h _ftp.cpp  libftp.a
	$(CC) $(CFLAGS) -o ftpputfiles ftpputfiles.cpp _public.cpp _ftp.cpp libftp.a -lm -lc
	cp -f ftpputfiles ../bin/.


ftpgetfiles: ftpgetfiles.cpp _ftp.h _ftp.cpp  libftp.a
	$(CC) $(CFLAGS) -o ftpgetfiles ftpgetfiles.cpp _public.cpp _ftp.cpp libftp.a -lm -lc
	cp -f ftpgetfiles ../bin/.

deletefiles: deletefiles.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o deletefiles deletefiles.cpp _public.cpp -lm -lc
	cp -f deletefiles ../bin/.

demo21: demo21.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o demo21 demo21.cpp _public.cpp -lm -lc
	cp -f demo21 ../bin/.

demo22: demo22.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o demo22 demo22.cpp _public.cpp -lpthread -lm -lc
	cp -f demo22 ../bin/.

demo23: demo23.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o demo23 demo23.cpp _public.cpp -lm -lc
	cp -f demo23 ../bin/.

demo25: demo25.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o demo25 demo25.cpp _public.cpp -lm -lc
	cp -f demo25 ../bin/.

diskinfo: diskinfo.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o diskinfo diskinfo.cpp _public.cpp -lm -lc
	cp -f diskinfo ../bin/.

cpuinfo: cpuinfo.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o cpuinfo cpuinfo.cpp _public.cpp -lm -lc
	cp -f cpuinfo ../bin/.

meminfo: meminfo.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o meminfo meminfo.cpp _public.cpp -lm -lc
	cp -f meminfo ../bin/.

httpclient: httpclient.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o httpclient httpclient.cpp _public.cpp -lm -lc
	cp -f httpclient ../bin/.

wgetclient: wgetclient.cpp _public.h _public.cpp
	$(CC) $(CFLAGS) -o wgetclient wgetclient.cpp _public.cpp -lm -lc
	cp -f wgetclient ../bin/.

deletetables: deletetables.cpp _public.h  _public.cpp _ooci.h _ooci.cpp
	$(CC) $(CFLAGS) -o deletetables deletetables.cpp  $(ORAINCL) $(ORALIB) $(ORALIBS) _public.cpp _ooci.cpp -lm -lc
	cp -f deletetables ../bin/.

tbspaceinfo: tbspaceinfo.cpp _public.h  _public.cpp _ooci.h _ooci.cpp
	$(CC) $(CFLAGS) -o tbspaceinfo tbspaceinfo.cpp  $(ORAINCL) $(ORALIB) $(ORALIBS) _public.cpp _ooci.cpp -lm -lc
	cp -f tbspaceinfo ../bin/.

hsmtables: hsmtables.cpp _public.h  _public.cpp _ooci.h _ooci.cpp
	$(CC) $(CFLAGS) -o hsmtables hsmtables.cpp  $(ORAINCL) $(ORALIB) $(ORALIBS) _public.cpp _ooci.cpp -lm -lc
	cp -f hsmtables ../bin/.

exptables: exptables.cpp _public.h  _public.cpp _ooci.h _ooci.cpp
	$(CC) $(CFLAGS) -o exptables exptables.cpp  $(ORAINCL) $(ORALIB) $(ORALIBS) _public.cpp _ooci.cpp -lm -lc
	cp -f exptables ../bin/.

dminoracle: dminoracle.cpp _public.h  _public.cpp _ooci.h _ooci.cpp
	$(CC) $(CFLAGS) -o dminoracle dminoracle.cpp  $(ORAINCL) $(ORALIB) $(ORALIBS) _public.cpp _ooci.cpp -lm -lc
	cp -f dminoracle ../bin/.

dminmysql: dminmysql.cpp _public.h  _public.cpp _mysql.h _mysql.cpp
	$(CC) $(CFLAGS) -o dminmysql dminmysql.cpp  $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) _public.cpp _mysql.cpp -lm -lc
	cp -f dminmysql ../bin/.

dminpg: dminpg.cpp _public.h  _public.cpp _postgresql.h _postgresql.cpp
	$(CC) $(CFLAGS) -o dminpg dminpg.cpp  $(PGINCL) $(PGLIB) $(PGLIBS) _public.cpp _postgresql.cpp -lm -lc
	cp -f dminpg ../bin/.

clean:
	rm -rf libftp.a demo11 demo12 demo13 demo14 demo18 ftpgetfiles deletefiles deletetables
	rm -rf tcpputfiles tcpgetfiles tcpfileserver exptables ftpputfiles dminoracle httpclient
	rm -rf dminmysql dminpg wgetclient hsmtables diskinfo cpuinfo meminfo tbspaceinfo
	rm -rf tcpfileserver1
