include ../../flags.mk

INCFLAGS=-Isrc -I$(GADGET3INC) -I$(LIBSTARKINC) -I$(ALGEBRAINC) -I$(FFTINC)
LIBS=						\
	gadgetlib				\
	stark					\
	algebralib				\
	FFT

LIBFLAGS=$(addprefix -l, $(LIBS))
LNKFLAGS=-L"$(ALGEBRALNKDIR)" -L"$(FFTLIBLNKDIR)" -L"$(GADGET3LNKDIR)" -L"$(LIBSTARKLINKDIR)"

SRCDIR  = src
SRCEXT  = cpp
OBJDIR  = $(BLDDIR)/obj

SRCS    := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
SRCDIRS := $(shell find . -name '*.$(SRCEXT)' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.$(SRCEXT),$(OBJDIR)/%.o,$(SRCS))

TARGET=$(BLDDIR)/libzilch.a
all: $(TARGET)

$(TARGET): buildrepo $(OBJS)
#	@echo 'Create Zilch library'
	ar -r $(TARGET) $(OBJS)

$(OBJDIR)/%.o: %.$(SRCEXT) src/TinyRAM/TinyRAMDefinitions.hpp ../../configs.hpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCFLAGS) -c -o "$@" "$<"

clean:
	$(RM) -f $(TARGET) $(OBJS) $(DEPS)
	$(RM) -rf $(OBJDIR)

buildrepo:
	$(call make-repo)

define make-repo
for dir in $(SRCDIRS); \
	do \
	mkdir -p $(OBJDIR)/$$dir; \
	done
endef
