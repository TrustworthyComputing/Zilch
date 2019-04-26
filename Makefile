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
HYPERION_DIR		:= $(WD)/framework/hyperion
FFTLIB_DIR			:= $(WD)/algebra/FFT

HYPERION_TESTS_DIR 	:= $(WD)/framework/hyperion-tests

.PHONY: 							\
		libstark libstark-clean 	\
		hyperion hyperion-clean 	\
		fft fft-clean 				\
		algebralib algebralib-clean \
		gadgetlib gadgetlib-clean 	\
		clean

default: hyperion

help:
	./hyperion -h

run-prover:
	./hyperion $(ARGS) --tsteps 10 --prover

run-verifier:
	./hyperion $(ARGS) --tsteps 10 --verifier

libstark:
	$(MAKE) -C $(LIBSTARK_DIR) 		\
	BLDDIR=$(BLDDIR)/libstark 		\
	FFTINC=$(FFTLIB_DIR)/src		\
	ALGEBRAINC=$(ALGEBRALIB_DIR)/headers

libstark-clean:
	$(MAKE) clean -C $(LIBSTARK_DIR) BLDDIR=$(BLDDIR)/libstark

hyperion: gadgetlib fft algebralib libstark
	$(MAKE) -C $(HYPERION_DIR) 					\
		BLDDIR=$(BLDDIR)/hyperion               \
		EXEDIR=$(EXE_DIR) 						\
		FFTINC=$(FFTLIB_DIR)/src 				\
		FFTLIBLNKDIR=$(BLDDIR)/fft				\
		ALGEBRAINC=$(ALGEBRALIB_DIR)/headers	\
		ALGEBRALNKDIR=$(BLDDIR)/algebralib 		\
		LIBSTARKINC=$(LIBSTARK_DIR)/src 		\
		LIBSTARKLINKDIR=$(BLDDIR)/libstark 		\
		GADGET3INC=$(GADGETLIB3_DIR)/../.		\
		GADGET3LNKDIR=$(BLDDIR)/gadgetlib

hyperion-clean:
	$(MAKE) clean -C $(HYPERION_DIR) 	\
		BLDDIR=$(BLDDIR)/hyperion 		\
		EXEDIR=$(EXE_DIR)

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

hyperion-tests: hyperion-tests gadgetlib fft algebralib libstark
	$(MAKE) -C $(HYPERION_TESTS_DIR) 			\
		BLDDIR=$(BLDDIR)/hyperion-tests     	\
		EXEDIR=$(EXE_DIR)		 				\
		HYPERIONINC_DIR=$(HYPERION_DIR)/src 		\
		HYPERIONOBJ_DIR=$(BLDDIR)/hyperion 		\
		FFTINC=$(FFTLIB_DIR)/src 				\
		FFTLIBLNKDIR=$(BLDDIR)/fft				\
		ALGEBRAINC=$(ALGEBRALIB_DIR)/headers 	\
		ALGEBRALNKDIR=$(BLDDIR)/algebralib 		\
		LIBSTARKINC=$(LIBSTARK_DIR)/src 		\
		LIBSTARKLINKDIR=$(BLDDIR)/libstark 		\
		GADGET3INC=$(GADGETLIB3_DIR)/../.		\
		GADGET3LNKDIR=$(BLDDIR)/gadgetlib		

hyperion-tests-clean:
	$(MAKE) clean -C $(HYPERION_TESTS_DIR) BLDDIR=$(BLDDIR)/hyperion-tests

clean: gadgetlib-clean hyperion-clean libstark-clean fft-clean algebralib-clean
	$(RM) -r $(BLDDIR)
