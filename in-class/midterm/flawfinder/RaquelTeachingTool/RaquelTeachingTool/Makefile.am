# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

INCLUDES = -I$(top_scrdir)/src -I/usr/local/include/RaquelDBMS
bin_PROGRAMS = RaquelTeachingTool
RaquelTeachingTool_LDFLAGS = -L/usr/local/lib -L/usr/local
RaquelTeachingTool_LDADD = -ltcl -ltk /usr/local/lib/RaquelDBMS/libRaquelDBMS.a
noinst_HEADERS = Error.h LinuxInputStack.h TclTkInterface.h TeachingTool.h \
	config.h
RaquelTeachingTool_SOURCES = Error.cpp LinuxInputStack.cpp TclTkInterface.cpp \
	TeachingTool.cpp TokeniserTest.cpp
