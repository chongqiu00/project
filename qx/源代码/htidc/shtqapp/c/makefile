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

# 通用框架头文件路径
PUBINCL = -I/htidc/public/c

# 通用框架库文件路径
PUBCPP = /htidc/public/c/_public.cpp 
FTPCPP = /htidc/public/c/_ftp.cpp 
OCICPP = /htidc/public/c/_ooci.cpp

CC=g++

# CFLAGS = -O2
# CFLAGS = -O2 -Wall

CFLAGS = -g -Wno-write-strings -Wno-unused-variable


all: shtqappserver client


shtqappserver:shtqappserver.cpp 
	$(CC) $(CFLAGS) -o shtqappserver shtqappserver.cpp $(ORAINCL) $(ORALIB) $(ORALIBS) $(PUBINCL) $(PUBCPP) $(OCICPP) -lm -lc
	cp -f shtqappserver ../bin/.

client:client.cpp 
	$(CC) $(CFLAGS) -o client client.cpp  $(PUBINCL) $(PUBCPP) -lm -lc
	cp -f client ../bin/.


clean:
	rm -rf shtqappserver client
