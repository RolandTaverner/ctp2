#
# $Id$
#
# This Makefile is intended to automate certain useful tasks
# for developers.
# It is NOT intended to be used to build the CTP2 executable
#
# Provided targets are:
# bootstrap
#   Will create autoconf-based buildfiles.
#   After that, you can run 
#   
#                ./configure && make
#
#   to build the targets supported by the autoconf buildsystem.
#   
# local
#   Will copy relevant files from the source tree to a local installation,
#   to allow them to be tested there.
#   The location of the local installation is taken from etc/local-install
#   (Note that is a local etc directory, not /etc).
#   This target uses a shell script, so it will require, for example, cygwin
#   to work.
#
# playtest
#   Will create a directory called playtest and copy into it all appropriate
#   files to constitute a playtest version, ready to be compressed and
#   distributed.  This starts by deleting the playtest directoy, so
#   be sure there is nothing there you wish to preserve!
#
# purge
#   Will remove any file generated by autoconf.
#   This may not include other files generated during the build
#   process (./configure && make)
#
# doc
#   Make all the documentation

all:
	#################################### \
	# This Makefile has no default     # \
	# or all target.                   # \
	# See the comments in the Makefile # \
	# for a description of its use.    # \
	####################################

bootstrap:
	@echo "Bootstraping Civilization Call to Power"
	@echo " "
	@echo "This is no official release by activision."
	@echo " "
	aclocal -I ctp2_code/os/autoconf
	autoheader
	libtoolize --force --copy
	automake --foreign --add-missing --copy
	autoconf
	@echo " "
	@echo "Bootstraping complete."
	@echo " "
	@echo "You may run ./configure and then make to compile CTP2."

local:
	bin/copy_to_local.sh

PLAYTEST = playtest

playtest:
	$(MAKE) -C doc/user
	-rm -rf playtest
	mkdir -p $(PLAYTEST)/ctp2_program/ctp/dll/
	svn export ctp2_code/ctp/dll $(PLAYTEST)/ctp2_program/ctp/dll --force
	cp -uv ctp2_code/ctp/dll/map/*.dll $(PLAYTEST)/ctp2_program/ctp/dll/map/
	cp -uv ctp2_code/ctp/anet* $(PLAYTEST)/ctp2_program/ctp/
	cp -uv ctp2_code/ctp/ctp2.* $(PLAYTEST)/ctp2_program/ctp/
	cp -uv ctp2_code/ctp/zlib1.dll $(PLAYTEST)/ctp2_program/ctp/
	cp -uv ctp2_code/ctp/jpeg62.dll $(PLAYTEST)/ctp2_program/ctp/
	cp -uv ctp2_code/ctp/libtiff3.dll $(PLAYTEST)/ctp2_program/ctp/
	#cp -uv ctp2_code/ctp/appstr.txt $(PLAYTEST)/ctp2_program/ctp/
	cp -uv doc/user/playtest/*.txt $(PLAYTEST)/
	mkdir -p $(PLAYTEST)/doc
	cp -uv doc/user/manual/user_manual.pdf $(PLAYTEST)/doc
	svn export ctp2_data $(PLAYTEST)/ctp2_data --force
	svn export Scenarios $(PLAYTEST)/Scenarios --force

doc:
	$(MAKE) -C doc

.PHONY: all local playtest doc

SRCDIRS=\
	ctp2_code/ctp \
	ctp2_code/mapgen \
	ctp2_code/gs/dbgen

MRPROPER_DIRS=\
	autom4te.cache \
	ctp2_code/autom4te.cache \
	$(foreach srcdir,$(SRCDIRS),\
		$(srcdir)/.deps \
		$(srcdir)/.lib \
		$(srcdir)/_lib \
	 )
MRPROPER_FILES=\
	GNUmakefile \
	GNUmakefile.in \
	aclocal.m4 \
	config.log \
	config.status \
	configure \
	libtool \
	ctp2_code/Makefile \
	ctp2_code/Makefile.in \
	ctp2_code/aclocal.m4 \
	ctp2_code/config.log \
	ctp2_code/config.status \
	ctp2_code/configure \
	ctp2_code/libtool \
	ctp2_code/gs/dbgen/ctpdb \
	ctp2_code/gs/dbgen/ctpdb.exe \
	ctp2_code/gs/dbgen/ctpdb_lex.c \
	ctp2_code/gs/dbgen/ctpdb_yacc.c \
	ctp2_code/gs/dbgen/ctpdb_yacc.h \
	ctp2_code/gs/dbgen/lex.yy.c \
	ctp2_code/gs/dbgen/y.tab.c \
	ctp2_code/gs/dbgen/y.tab.h \
	ctp2_code/os/autoconf/config/config.guess \
	ctp2_code/os/autoconf/config/config.sub \
	ctp2_code/os/autoconf/config/depcomp \
	ctp2_code/os/autoconf/config/install-sh \
	ctp2_code/os/autoconf/config/ltmain.sh \
	ctp2_code/os/autoconf/config/missing \
	ctp2_code/os/autoconf/config/mkinstalldirs \
	ctp2_code/os/include/config.h \
	ctp2_code/os/include/config.h.in \
	ctp2_code/os/include/config.h.in~ \
	$(foreach srcdir,$(SRCDIRS),\
		$(srcdir)/*.la \
		$(srcdir)/*.lo \
		$(srcdir)/*.o \
		$(srcdir)/Makefile \
		$(srcdir)/Makefile.in \
		$(srcdir)/so_locations \
	 )

clean:

distclean:

purge: distclean
	@for DIR in $(MRPROPER_DIRS) ; do \
		echo rm -rf $$DIR ; \
		rm -rf $$DIR ; \
	done
	@for FILE in $(MRPROPER_FILES) ; do \
		echo rm -f $$FILE ; \
		rm -f $$FILE ; \
	done
