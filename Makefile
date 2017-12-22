Make=make
RM=rm
MKDIR=mkdir
CC=arm-none-eabi-gcc
CP=cp

OUTDIR=Debug
OUTDIR_BIN=bin
OUTFILE=$(OUTDIR)/project
CFG_INC=-Iinclude
OBJ=\
	$(OUTDIR)/lcd.o \
	$(OUTDIR)/s3c_uart.o \
	$(OUTDIR)/support.o\
	$(OUTDIR)/interrupt.o\
		$(OUTIDR)src/main.o \

CFLAGS64=-DCONFIG_MANGO_64
COMPILE64=$(CC) -c -Wall -g -o "$(OUTDIR)/$(*F).o" $(CFG_INC) $(CFLAGS64) $<
LINK64=$(CC) -g -o "$(OUTFILE)" $(OBJ) -e main -Ttext=0x50100000 --specs=nosys.specs
COPY64=$(CP) $(OUTFILE) $(OUTDIR_BIN)/

COMPILE=$(COMPILE64)
LINK=$(LINK64)
COPY=$(COPY64)

all: $(OUTFILE)

$(OUTDIR)/%.o : src/%.c
	$(COMPILE)

$(OUTFILE): $(OUTDIR) $(OBJ)
	$(LINK)
	$(COPY)

$(OUTDIR):
	$(MKDIR) -p "$(OUTDIR)" ; \
	$(MKDIR) -p "$(OUTDIR_BIN)"

rebuild: clean all

clean: 
	$(RM) -rf $(OUTDIR)
  
