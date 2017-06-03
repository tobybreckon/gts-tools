#####################################################################

# gts-tools Makefile - dist

# Copyright (C) 2006  Toby Breckon
# License : GPL

#####################################################################

INSTALL_PATH_BIN=/usr/local/bin/
INSTALL_PATH_DOC=/usr/share/doc/packages/gts-tools/

CURRENTPWD=$(PWD)

#####################################################################

all:
	mkdir tools; cd src; make

install:
	install tools/gts* $(INSTALL_PATH_BIN);
	test -e $(INSTALL_PATH_DOC) || mkdir $(INSTALL_PATH_DOC);
	install -m 644 $(INSTALL_DOC_FILES) $(INSTALL_PATH_DOC)

clean:
	rm -f *~; \
	cd tools && rm -f *; cd ..; \
	rmdir tools; \
	cd src; make clean \

#####################################################################
