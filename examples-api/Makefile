include ../flags.mk

BLDDIR				:= ../bin
ALGEBRALIB_DIR		:= ../algebra/algebralib
LIBSTARK_DIR		:= ../libstark
GADGETLIB3_DIR		:= ../framework/gadgetlib/gadgetlib
HYPERION_DIR		:= ../framework/hyperion
FFTLIB_DIR			:= ../algebra/FFT

HYPERION_TESTS_DIR 	:= ../framework/hyperion-tests

.PHONY: 					\
		fact fact-clean 	\
		fact-local fact-prover fact-verifier \
		makedir clean

INCFLAGS=-Isrc -I$(GADGET3INC) -I$(LIBSTARKINC) -I$(ALGEBRAINC) -I$(FFTINC)
LIBFLAGS= -lhyperion -lgadgetlib -lstark -lalgebralib -lFFT
LNKFLAGS=-L"$(BLDDIR)/algebralib" -L"$(BLDDIR)/fft" -L"$(BLDDIR)/gadgetlib" -L"$(BLDDIR)/libstark" -L"$(BLDDIR)/hyperion"

default: fact

fact: makedir fact-local fact-prover fact-verifier

makedir:
	mkdir -p ../bin/examples-api

fact-local: 
	$(CC) $(CFLAGS) $(CPPFLAGS) -Isrc -I$(GADGETLIB3_DIR)/../. -I$(LIBSTARK_DIR)/src -I$(ALGEBRALIB_DIR)/headers -I$(FFTLIB_DIR)/src -I$(HYPERION_DIR)/src -c -o ../bin/examples-api/fact-local.o ./fact-local.cpp
	$(CC) -o ./fact-local.out ../bin/examples-api/fact-local.o -fopenmp $(LNKFLAGS) $(LIBFLAGS) -march=native -lm -lpthread -lcrypto -ljsoncpp

fact-prover: 
	$(CC) $(CFLAGS) $(CPPFLAGS) -Isrc -I$(GADGETLIB3_DIR)/../. -I$(LIBSTARK_DIR)/src -I$(ALGEBRALIB_DIR)/headers -I$(FFTLIB_DIR)/src -I$(HYPERION_DIR)/src -c -o ../bin/examples-api/fact-prover.o ./fact-prover.cpp
	$(CC) -o ./fact-prover.out ../bin/examples-api/fact-prover.o -fopenmp $(LNKFLAGS) $(LIBFLAGS) -march=native -lm -lpthread -lcrypto -ljsoncpp

fact-verifier: 
	$(CC) $(CFLAGS) $(CPPFLAGS) -Isrc -I$(GADGETLIB3_DIR)/../. -I$(LIBSTARK_DIR)/src -I$(ALGEBRALIB_DIR)/headers -I$(FFTLIB_DIR)/src -I$(HYPERION_DIR)/src -c -o ../bin/examples-api/fact-verifier.o ./fact-verifier.cpp
	$(CC) -o ./fact-verifier.out ../bin/examples-api/fact-verifier.o -fopenmp $(LNKFLAGS) $(LIBFLAGS) -march=native -lm -lpthread -lcrypto -ljsoncpp

fact-clean:
	$(RM) -f ./fact-*.out
	$(RM) -f ../bin/examples-api/fact-*.o

clean: fact-clean
	$(RM) -rf ../bin/examples-api
