CC ?= cc
CFLAGS ?= -O0 -g
OUTNAME ?= network-monitoring
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
SUDO ?= sudo

all:
	$(CC) $(CFLAGS) network-monitoring.c -o $(OUTNAME)

install:
	mkdir -p $(DESTDIR)$(BINDIR)
	install -m0755 $(OUTNAME) $(DESTDIR)$(BINDIR)/$(OUTNAME)

install-systemd:
	mkdir -p $(DESTDIR)/etc/systemd/system/
	install -m0644 network-monitoring.service $(DESTDIR)/etc/systemd/system/network-monitoring.service

run:
	LOGDIR=. ./$(OUTNAME)
