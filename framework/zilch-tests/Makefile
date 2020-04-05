include ../../flags.mk

WHICH				:= $(shell which which)
MKDIR				:= $(shell $(WHICH) mkdir)
DIRNAME				:= $(shell $(WHICH) dirname)

INCFLAGS=-I$(ZILCHINC_DIR) -I$(GADGET3INC) -I$(LIBSTARKINC) -I$(ALGEBRAINC) -I$(FFTINC)
LIBS=						\
	gtest					\
	gadgetlib				\
	stark					\
	algebralib				\
	FFT

LIBFLAGS=$(addprefix -l, $(LIBS))
LNKFLAGS=-L"$(ALGEBRALNKDIR)" -L"$(FFTLIBLNKDIR)" -L"$(GADGET3LNKDIR)" -L"$(LIBSTARKLINKDIR)"

SRCS:=	$(shell ls *.cpp)
OBJS=$(addprefix $(BLDDIR)/, $(SRCS:.cpp=.o))
ZILCHOBJS := $(shell find $(ZILCHOBJ_DIR) -name '*.o' ! -name 'main.o')

TARGET=$(BLDDIR)/zilch-tests
all: $(TARGET)
DST=$(EXEDIR)/zilch-tests
	

$(TARGET): $(OBJS)
#	@echo 'Building target: $@'
#	@echo 'Invoking: GCC Linker'
	$(CC) -o "$@" $(OBJS) $(ZILCHOBJS) -fopenmp $(LNKFLAGS) $(LIBFLAGS) -march=native -lm -lpthread -lcrypto -ljsoncpp
#	@echo 'Finished building target: $@'
	cp $(TARGET) $(DST)


$(BLDDIR)/%.o: %.cpp
#	@echo 'Building file: $@ ($<)'
	@$(MKDIR) -p $(shell $(DIRNAME) $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCFLAGS) -c -o "$@" "$<"

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)
