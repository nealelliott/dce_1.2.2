LEX     = flex
YACC    = bison -y
YFLAGS  = -d
objects = sams.o samsgram.o samslex.o

all: samsgram.c samsgram.h samslex.c sams
samsgram.c samsgram.h: samsgram.y
	${YACC} ${YFLAGS} samsgram.y
	mv y.tab.c samsgram.c
	mv y.tab.h samsgram.h
samslex.c: samslex.l
	flex samslex.l
	mv lex.yy.c samslex.c
sams:
	gcc -o sams -I. sams.c samsgram.c samslex.c
clean: 
	rm samsgram.c samsgram.h samslex.c sams
