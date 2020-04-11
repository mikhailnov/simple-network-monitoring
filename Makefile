CC ?= cc
CFLAGS ?= -O0 -g
OUTNAME ?= network-monitoring

all:
	$(CC) $(CFLAGS) network-monitoring.c -o $(OUTNAME)

run:
	LOGDIR=. ./$(OUTNAME)
