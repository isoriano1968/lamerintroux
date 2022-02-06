CC = CC
MIPSFLAGS = -g0 -O1 -mips4
LIBS = -L/usr/nekoware/lib -L/usr/lib32 -lGL -lGLU -lglut -lX11 -lmikmod -lm
INCLUDES = -I/usr/nekoware/include 
CFLAGS = $(MIPSFLAGS) $(INCLUDES) -Wall 
PROG = LamerIntroSGI

SRCS = main.cpp

all: $(PROG)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(PROG) $(LIBS)

clean:
	rm -f $(PROG) *.o 
