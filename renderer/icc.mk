CC = icpc
GENERALFLAGS = -fPIC -g -Wall -Wunused-variable -Wcheck -Wextra-tokens -Wformat -Wformat-security -Wmissing-declarations -Wmissing-prototypes -Wpointer-arith -Wreturn-type -Wsign-compare -Wuninitialized
OPTIMFLAGS = -axSSE3,SSE4.1,SSE4.2,AVX -align -O3 -pipe -ipo -fast -parallel -openmp -pthread
REPORTSFLAGS = -opt-report1 -par-report1 -vec-report1 -openmp-report1
#-Winline -Wimplicit
FPFLAGS = -fp-model fast=2 -no-prec-sqrt
DEBUGFLAG = -g
CFLAGS += $(DEBUGFLAG) $(GENERALFLAGS) $(OPTIMFLAGS)
ifeq ($(DEBUG_MODE), 0)
	CFLAGS += -DNDEBUG
endif
ifeq ($(PRODUCE_REPORTS), 1)
	CFLAGS += $(REPORTSFLAGS) 
endif