ZLIB_DIR = zlib
ZLIB_OBJS += inffast.o inflate.o inftrees.o
ZLIB_OBJS += adler32.o trees.o uncompr.o crc32.o zutil.o

LIB_OBJS += $(addprefix $(ZLIB_DIR)/, $(ZLIB_OBJS))
CPPFLAGS += -I $(CURDIR)/$(ZLIB_DIR)
BUILD_SUBDIRS += $(ZLIB_DIR)
