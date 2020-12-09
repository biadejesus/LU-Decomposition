# Compiler spec file for Intrepid GCC/UPC 4.x with Berkeley UPC Runtime (UPCR)

upc_home = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/dbg

# upc compiler command
upc_compiler = $upc_home$/upcc

# upc run command
# Following replacements are performed:
# %N - number of UPC threads
# %P - program executable
# %A - arguments to program
# %B - berkeley-specific upcrun arguments (should appear if and only if this is Berkeley upcrun)
upcrun_command = $upc_home$/upcrun -q -n %N %B %P %A

# default sysconf file to use
default_sysconf = smp-interactive

# list of supported compiler features
feature_list = trans_bupc,pragma_upc_code,driver_upcc,runtime_upcr,gasnet,upc_collective,upc_io,upc_memcpy_async,upc_memcpy_vis,upc_ptradd,upc_thread_distance,upc_tick,upc_sem,upc_dump_shared,upc_trace_printf,upc_trace_mask,upc_local_to_shared,upc_all_free,upc_atomics,pupc,upc_types,upc_castable,upc_nb,debug,trace,stats,debugmalloc,nogasp,segment_fast,os_linux,cpu_x86_64,cpu_64,cc_gnu,packedsptr,upc_io_64  gasnet_has_smp gasnet_has_udp gasnet_has_mpi gasnet_has_ibv gasnet_has_seq gasnet_has_par cc_subfamily_gnu

# option to pass upc compiler to get %T static threads
upc_static_threads_option = -T %T

# option for performing just the source-to-source compile step
# or empty if not supported by the compiler
upc_trans_option = 

# colon-delimited path where to find harness.conf files
suite_path = $TOP_SRCDIR$/gasnet/tests/upcr-harness:$TOP_SRCDIR$/upc-tests:$TOP_SRCDIR$/upc-examples

# GNU make
gmake = /usr/bin/make

# misc system tools
ar = /usr/bin/ar
ranlib = /usr/bin/ranlib

# C compiler & flags (should be empty on upcr/GASNet to auto-detect)
cc =
cflags =
ld = 
ldflags = 
libs =

# host C compiler (or empty for same as cc)
host_cc = 
host_cflags = 
host_ld = 
host_ldflags = 
host_libs = 

# OS suffix for exxcutables, or empty for none
exe_suffix = 

# known failures, in the format: test-path/test-name[failure comment] | test-name[failure comment]
# lines may be broken using backslash
# known failures may also include more specific failure/platform selectors, eg:
# mupc/test_stress_05-int [compile-all ; (cpu_ia64 || cpu_i686) && os_linux ; Compile failure on Itanium+x86 Linux... ] 
# known_failures may be empty to use the ones in the harness.conf files
known_failures = \
benchmarks/upc_io_test [run-all ; network_ibv ; gasnet bug955 (failures on upc_io_test - firehose mishandles free)] | \
bug6b [compile-warning ;; expect fail but got warning due to large value of UPC_MAX_BLOCK_SIZE] | \
bug7 [compile-failure ; pthreads ; GCC/UPC thread locals are TLS variables (address unknown at compile time)] | \
bug7b [compile-failure ; pthreads ; GCC/UPC thread locals are TLS variables (address unknown at compile time)] | \
bug247 [compile-failure ; cpu_32 ; GCC/UPC does not support huge pointer offsets on 32-bit targets] | \
bug317a [compile-failure ;; GCC/UPC does not support casts from shared pointers to integers] | \
bug438 [compile-failure ;; GCC/UPC generates undefined external reference to an inlined function when compiled at -O0] | \
bug544 [compile-failure ; pthreads ; GCC/UPC thread locals are TLS variables (address unknown at compile time)] | \
bug547 [compile-warning ;; GCC/UPC detects constants that exceed range of 'float'] | \
bug604 [run-crash; cpu_ia64 && structsptr; GCC/UPC invalid code leads to segfault for struct sptr when compiled with optimization (IA64).  See GCC Bugzilla bug 50489] | \
bug846 [compile-failure ; cpu_32 ; GCC/UPC does not support the declaration of huge arrays on 32-bit targets] | \
bug899 [compile-failure ;; GCC/UPC does not properly handle #pragma upc c_code, by undefining UPC keywords and reserved identifiers] | \
bug899b [compile-failure ;; GCC/UPC does not properly handle #pragma upc c_code, by undefining UPC keywords and reserved identifiers] | \
bug899c [compile-failure ;; GCC/UPC does not properly handle #pragma upc c_code, by undefining UPC keywords and reserved identifiers] | \
bug922 [compile-failure ;; GCC/UPC does not properly handle #pragma upc c_code, by undefining UPC keywords and reserved identifiers] | \
bug1228 [run-all ;; UPCR run-time error (Ambiguity in barrier matching semantics)] | \
bug1275 [compile-failure ; pthreads ; GCC/UPC thread locals are TLS variables (address unknown at compile time)] | \
bug1275a [compile-failure ; pthreads ; GCC/UPC thread locals are TLS variables (address unknown at compile time)] | \
bug2947init [run-crash ; cpu_ia64 ; GCC/UPC generates mis-aligned store to shared struct (IA64)] | \
bug3036 [compile-failure ; pthreads ; GCC/UPC converted variable to TLS while extern definition was not converted (pragma c_code was in place)] | \
bug3038 [compile-failure ; pthreads ; GCC/UPC converted variable to TLS while extern definition was not converted (pragma c_code was in place)] | \
bug3038a [compile-failure ; pthreads ; GCC/UPC converted variable to TLS while extern definition was not converted (pragma c_code was in place)] | \
bug3038a [compile-warning ; trans_O ; GCC/UPC link fails due to GCC bug 49379 - warning from linker: alignment lost for -ftree-vectorize optimization] | \
bug3051 [compile-warning ;; GCC/UPC issues spurious warning due to GCC bug 62198 - initialization discards qualifier] | \
bug3051 [compile-crash ;; GCC/UPC ICE when compiling sizeof indirect through pointer to shared VLA] | \
bug3056 [compile-warning ;; GCC/UPC issues spurious warning due to GCC bug 62198 - initialization discards qualifier] | \
bug3056 [compile-crash ;; GCC/UPC ICE when compiling sizeof indirect through pointer to shared VLA] | \
gasnet-tests/testconduitspecific [compile-failure ;;  Compile-time warning due to gcc 4.5.1 work-around] | \
gasnet-tests/testconduitspecific [run-crash ; os_darwin && conduit_mpi ; GASNET tests sporadically crash (may not be GCC/UPC related)] | \
gasnet-tests/testthreads-par [run-crash ; os_darwin && conduit_mpi ; GASNET tests sporadically crash (may not be GCC/UPC related)] | \
gasnet-tests/testgasnet-parsync [compile-failure ;; Missing gasnet-udp-parsync library, Makefile problem] | \
guts_main/barrier_neg [run-crash ;; test intended to FAIL, but harness detects as CRASH] | \
gwu/I_case5_ii [run-crash; cpu_ia64 && structsptr; GCC/UPC invalid code leads to segfault for struct sptr when compiled with optimization (IA64).  See GCC Bugzilla bug 50489] | \
gwu-npb-upc/btio-A [compile-failure ; packedsptr || cpu_32 ; GCC/UPC does not support block sizes > 65536 in some configurations] | \
gwu-npb-upc/btio-A [compile-failure ; pthreads ; GCC/UPC multiple defined symbols error reported for duplicates in .tbss section] | \
gwu-npb-upc/btio-A [compile-warning ; os_darwin && conduit_mpi && trans_O; GCC/UPC alignment lost in merging tentative definition - linker warning] | \
gwu-npb-upc/btio-A [run-match ;; Bug 1508 - btio fails with NaNs] | \
gwu-npb-upc/btio-S [compile-failure ; pthreads ; GCC/UPC multiple defined symbols error reported for duplicates in .tbss section] | \
gwu-npb-upc/btio-S [compile-warning ; os_darwin && conduit_mpi && trans_O; GCC/UPC alignment lost in merging tentative definition - linker warning] | \
gwu-npb-upc/btio-S [run-match ;; Bug 1508 - btio fails with NaNs] | \
gwu-npb-upc/btio-W [compile-failure ; packedsptr || cpu_32 ; GCC/UPC does not support block sizes > 65536 in some configurations] | \
gwu-npb-upc/btio-W [compile-failure ; pthreads ; GCC/UPC multiple defined symbols error reported for duplicates in .tbss section] | \
gwu-npb-upc/btio-W [compile-warning ; os_darwin && conduit_mpi && trans_O; GCC/UPC alignment lost in merging tentative definition - linker warning] | \
gwu-npb-upc/btio-W [run-match ;; Bug 1508 - btio fails with NaNs] | \
gwu-npb-upc/cg-A [run-match ; cpu_32 && trans_O ; bug 653 - NPBs known to fail verification due to application bugs] | \
gwu-npb-upc/cg-S [run-match ; cpu_32 && trans_O ; bug 653 - NPBs known to fail verification due to application bugs] | \
gwu-npb-upc/cg-W [run-match ; cpu_32 && trans_O ; bug 653 - NPBs known to fail verification due to application bugs] | \
gwu-npb-upc/mg-S [run-match ;; bug 653 - NPBs known to fail verification: mg-S with thread count >= 16] | \
matmult/mat-opt [run-crash ; cpu_ia64 ; bug 2946 - (FP Emulation traps on Altix / error in test)] | \
perverse naming/p1 [compile-failure ;; GCC/UPC when invoked from 'upcc' cannot handle spaces in file names] | \
perverse naming/p2 [compile-failure ;; GCC/UPC when invoked from 'upcc' cannot handle pathnames containing special punctuation characters] | \
mupc/test_app_matmult       [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
pearls/z_order1             [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
sobel/sobel-static          [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
sobel/sobel-static-prefetch [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
uts/uts-upc-enhanced-t3xxl [run-mem ; os_linux && cpu_32 && network_smp && pshm ; ILP32 address space too small for test with PSHM ] | \
uts/uts-upc-enhanced-t2wl  [run-mem ; os_linux && cpu_32 && network_smp && pshm ; ILP32 address space too small for test with PSHM ] | \
uts/uts-upc-t2wl           [run-mem ; os_linux && cpu_32 && network_smp && pshm ; ILP32 address space too small for test with PSHM ] | \
spec1.3/issue03 [run-match ;; GCC/UPC does not properly handle a pointer to a shared array type] | \
spec1.3/issue11 [compile-failure ;; GCC/UPC does not allow casts from a pointer-to-shared to an integer] | \
spec1.3/issue11a [compile-failure ;; GCC/UPC does not allow casts from a pointer-to-shared to an integer] | \
spec1.3/issue64 [compile-failure ;; GCC/UPC allows only integer expressions as upc_barrier/upc_notify/upc_wait statement] | \
spec1.3/issue64a [compile-failure ;; GCC/UPC allows only integer expressions as upc_barrier/upc_notify/upc_wait statement] | \
spec1.3/issue64b [compile-failure ;; GCC/UPC allows only integer expressions as upc_barrier/upc_notify/upc_wait statement] | \
spec1.3/issue64c [compile-failure ;; GCC/UPC allows only integer expressions as upc_barrier/upc_notify/upc_wait statement] | \
spec1.3/issue71 [compile-pass ; static ; GCC/UPC does not diagnose [*] in typedefs as an error for static threads] | \
spec1.3/issue104 [compile-pass ;; GCC/UPC does not diagnose certain relational operations involving a PTS as an error] | \
spec1.3/issue104a [compile-pass ;; GCC/UPC does not diagnose certain relational operations involving a PTS as an error]