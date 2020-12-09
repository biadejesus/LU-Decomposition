
SRCS   = guppie.upc guppie-async.upc

UPCC   = upcc -T $(THREADS)
THREADS = 4
UPCC_FLAGS = 
BENCHMARK_FLAGS = -DLTABSIZE=19L

all: guppie_st$(THREADS) guppie-async_st$(THREADS) guppie-async-pipeline_st$(THREADS)

guppie_st%: 
	$(UPCC) -o $@ $(BENCHMARK_FLAGS) $(UPCC_FLAGS) guppie.upc

guppie-async_st%: 
	$(UPCC) -o $@ $(BENCHMARK_FLAGS) $(UPCC_FLAGS) guppie-async.upc

guppie-async-pipeline_st%: 
	$(UPCC) -o $@ $(BENCHMARK_FLAGS) $(UPCC_FLAGS) guppie-async-pipeline.upc

clean:
	-rm -f guppie_st* guppie-async_st* guppie-async-pipeline_st* *.out *.err
