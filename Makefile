# 编译整个项目

# programs, flags
MAKE     = 	make -r
CPP 	 =  g++
RM		 =  rm -f
CPPFLAGS =  -I include/ -c -std=c++11


OBJS = branch.o loop.o main.o preprocess.o sequence.o util.o variable.o global.o KMP.o
HEADERS = include/functions.h include/variable.h include/const.h include/headers.h \
		  include/code.h include/global.h

TARGET = ../Bin/SeedCup2016.exe

everything : ${TARGET}

all : realclean everything

branch.o : branch.cpp ${HEADERS}
	$(CPP) $(CPPFLAGS) -o $@ $<

loop.o : loop.cpp ${HEADERS}
	$(CPP) $(CPPFLAGS) -o $@ $<

preprocess.o : preprocess.cpp ${HEADERS}
	$(CPP) $(CPPFLAGS) -o $@ $<

sequence.o : sequence.cpp ${HEADERS}
	$(CPP) $(CPPFLAGS) -o $@ $<

util.o : util.cpp ${HEADERS}
	$(CPP) $(CPPFLAGS) -o $@ $<

variable.o : variable.cpp ${HEADERS}
	$(CPP) $(CPPFLAGS) -o $@ $<

global.o : global.cpp ${HEADERS}
	$(CPP) $(CPPFLAGS) -o $@ $<

KMP.o : KMP.cpp ${HEADERS}
	$(CPP) $(CPPFLAGS) -o $@ $<	

main.o : main.cpp ${HEADERS}
	$(CPP) $(CPPFLAGS) -o $@ $<

${TARGET} : ${OBJS}
	$(CPP) -o $@ ${OBJS}

run : ${TARGET}
	cd ../Bin && ./SeedCup2016.exe

clean :
	${RM} ${OBJS}

test : ${TARGET}
	cd ../Bin && ./SeedCup2016.exe
	cat ../Bin/output.txt
	cat ../Bin/answer.txt

realclean : 
	${RM} ${OBJS} ${TARGET}
