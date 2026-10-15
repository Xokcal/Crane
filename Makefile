CC      = gcc
CFLAGS  = -Wall -g
TARGET  = main.exe

SRCS = cranegram/cracode.c \
       cranegram/cravm.c \
       cranegram/craparse.c \
       cranegram/typevar.c \
       cranegram/types.c \
       cranegram/cgm_file.c \
       cranegram/cgm_gramma.c \
       cranegram/cgm_parse.c \
       cranegram/cgm_printf.c \
       tool/file/file.c \
       tool/log/log.c \
       tool/map/hashmap_int.c \
       tool/String/String.c \
       tool/xokmalloc/xokmalloc.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	del $(TARGET)