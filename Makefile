include ./flags.mk

WHICH				:= $(shell which which)
PWD					:= $(shell $(WHICH) pwd)

WD					:= $(shell $(PWD))
BUILD_DIR			:= bin
EXE_DIR				:= $(WD)
BLDDIR				:= $(WD)/$(BUILD_DIR)
GADGETLIB3_SRC_DIR	:= framework/gadgetlib/gadgetlib
FFTLIB_SRC_DIR		:= algebra/FFT/src

ALGEBRALIB_DIR		:= $(WD)/algebra/algebralib
LIBSTARK_DIR		:= $(WD)/libstark
GADGETLIB3_DIR		:= $(WD)/$(GADGETLIB3_SRC_DIR)
ZILCH_DIR		:= $(WD)/framework/zilch
FFTLIB_DIR			:= $(WD)/algebra/FFT

ZILCH_TESTS_DIR 	:= $(WD)/framework/zilch-tests

.PHONY: 							\
		libstark libstark-clean 	\
		zilch zilch-clean 	\
		zilch-lib zilch-lib-clean \
		zilch-tests zilch-tests-clean 	\
		fft fft-clean 				\
		algebralib algebralib-clean \
		gadgetlib gadgetlib-clean 	\
		clean

INCFLAGS=-Isrc -I$(GADGET3INC) -I$(LIBSTARKINC) -I$(ALGEBRAINC) -I$(FFTINC)
LIBFLAGS= -lzilch -lgadgetlib -lstark -lalgebralib -lFFT
LNKFLAGS=-L"$(BLDDIR)/algebralib" -L"$(BLDDIR)/fft" -L"$(BLDDIR)/gadgetlib" -L"$(BLDDIR)/libstark" -L"$(BLDDIR)/zilch"
TARGET=zilch

default: zilch

help:
	./$(TARGET) --help

zilch: zilch-lib main
	$(CC) -o $(TARGET) $(WD)/$(BUILD_DIR)/main.o -fopenmp $(LNKFLAGS) $(LIBFLAGS) -march=native -lm -lpthread -lcrypto -ljsoncpp

zilch-clean: zilch-lib-clean main-clean zilch-tests-clean
	$(RM) $(WD)/zilch
	$(RM) $(WD)/zilch-tests
		
main:
	$(CC) $(CFLAGS) $(CPPFLAGS) -Isrc -I$(GADGETLIB3_DIR)/../. -I$(LIBSTARK_DIR)/src -I$(ALGEBRALIB_DIR)/headers -I$(FFTLIB_DIR)/src -I$(ZILCH_DIR)/src -c -o $(WD)/$(BUILD_DIR)/main.o $(ZILCH_DIR)/main.cpp

main-clean:
	$(RM) $(WD)/$(BUILD_DIR)/main.o

zilch-lib: gadgetlib fft algebralib libstark
	$(MAKE) -C $(ZILCH_DIR) 					\
		BLDDIR=$(BLDDIR)/zilch               \
		EXEDIR=$(EXE_DIR) 						\
		FFTINC=$(FFTLIB_DIR)/src 				\
		FFTLIBLNKDIR=$(BLDDIR)/fft				\
		ALGEBRAINC=$(ALGEBRALIB_DIR)/headers	\
		ALGEBRALNKDIR=$(BLDDIR)/algebralib 		\
		LIBSTARKINC=$(LIBSTARK_DIR)/src 		\
		LIBSTARKLINKDIR=$(BLDDIR)/libstark 		\
		GADGET3INC=$(GADGETLIB3_DIR)/../.		\
		GADGET3LNKDIR=$(BLDDIR)/gadgetlib

zilch-lib-clean:
	$(MAKE) clean -C $(ZILCH_DIR) 	\
		BLDDIR=$(BLDDIR)/zilch 		\
		EXEDIR=$(EXE_DIR)

libstark:
	$(MAKE) -C $(LIBSTARK_DIR) 		\
	BLDDIR=$(BLDDIR)/libstark 		\
	FFTINC=$(FFTLIB_DIR)/src		\
	ALGEBRAINC=$(ALGEBRALIB_DIR)/headers

libstark-clean:
	$(MAKE) clean -C $(LIBSTARK_DIR) BLDDIR=$(BLDDIR)/libstark
	
fft:
	$(MAKE) -C $(FFTLIB_DIR)  BLDDIR=$(BLDDIR)/fft

fft-clean:
	$(MAKE) clean -C $(FFTLIB_DIR)  BLDDIR=$(BLDDIR)/fft

algebralib:
	$(MAKE) -C $(ALGEBRALIB_DIR) \
	BLDDIR=$(BLDDIR)/algebralib FFTINC=$(FFTLIB_DIR)/src

algebralib-clean:
	$(MAKE) clean -C $(ALGEBRALIB_DIR) BLDDIR=$(BLDDIR)/algebralib

gadgetlib:
	$(MAKE) -C $(GADGETLIB3_DIR)						\
	GADGETINC=$(GADGETLIB3_DIR)/. 						\
	ALGEBRAINC=$(ALGEBRALIB_DIR)/headers 				\
	ALGEBRALIBLINKDIR=$(BLDDIR)/algebralib				\
	FFTLIBLNKDIR=$(BLDDIR)/fft							\
	FFTINC=$(FFTLIB_DIR) FFTLIBLNKDIR=$(BLDDIR)/fft		\
	BLDDIR=$(BLDDIR)/gadgetlib

gadgetlib-clean:
	$(MAKE) -C $(GADGETLIB3_DIR) BLDDIR=$(BLDDIR)/gadgetlib clean

zilch-tests: zilch-tests gadgetlib fft algebralib libstark
	$(MAKE) -C $(ZILCH_TESTS_DIR) 			\
		BLDDIR=$(BLDDIR)/zilch-tests     	\
		EXEDIR=$(EXE_DIR)		 				\
		ZILCHINC_DIR=$(ZILCH_DIR)/src 		\
		ZILCHOBJ_DIR=$(BLDDIR)/zilch 		\
		FFTINC=$(FFTLIB_DIR)/src 				\
		FFTLIBLNKDIR=$(BLDDIR)/fft				\
		ALGEBRAINC=$(ALGEBRALIB_DIR)/headers 	\
		ALGEBRALNKDIR=$(BLDDIR)/algebralib 		\
		LIBSTARKINC=$(LIBSTARK_DIR)/src 		\
		LIBSTARKLINKDIR=$(BLDDIR)/libstark 		\
		GADGET3INC=$(GADGETLIB3_DIR)/../.		\
		GADGET3LNKDIR=$(BLDDIR)/gadgetlib		

zilch-tests-clean:
	$(MAKE) clean -C $(ZILCH_TESTS_DIR) BLDDIR=$(BLDDIR)/zilch-tests

clean: gadgetlib-clean zilch-clean libstark-clean fft-clean algebralib-clean
	$(RM) -r $(BLDDIR)
