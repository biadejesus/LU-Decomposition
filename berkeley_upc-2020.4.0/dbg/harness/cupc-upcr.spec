# Compiler spec file for Intrepid Clang UPC 3.4.x with Berkeley UPC Runtime (UPCR)

upc_home = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/dbg

# upc compiler command
upc_compiler = $upc_home$/upcc -Wc,-Wno-duplicate-decl-specifier -Wc,-Werror=pointer-arith

# upc run command
# Following replacements are performed:
# %N - number of UPC threads
# %P - program executable
# %A - arguments to program
# %B - berkeley-specific upcrun arguments (should appear if and only if this is Berkeley upcrun)
upcrun_command = $upc_home$/upcrun -q -n %N %B %P %A

# default sysconf file to use
default_sysconf = smp-interactive-no-pthreads

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
bug6 [run-all ; cpu_32 && structsptr; EXTERNAL: test requires over 2GB per thread] | \
bug137c [compile-crash ; cpu_powerpc64 && cpu_32 ; EXTERNAL: upsteam clang bug, see clang-upc issue #79] | \
bug137e [compile-crash ; cpu_powerpc64 && cpu_32 ; EXTERNAL: upsteam clang bug, see clang-upc issue #79] | \
bug247 [compile-failure ; cpu_32 ; Clang UPC does not support huge pointer offsets on 32-bit targets] | \
bug568 [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
bug544 [compile-failure ; cc_xlc ; EXTERNAL: XLC cannot handle the initializer for pcrazy ] | \
bug846 [compile-failure ; cpu_32 ; Clang UPC does not support the declaration of huge arrays on 32-bit targets] | \
bug899 [compile-failure ;; Clang UPC does not properly handle #pragma upc c_code, by undefining UPC keywords and reserved identifiers] | \
bug899b [compile-failure ;; Clang UPC does not properly handle #pragma upc c_code, by undefining UPC keywords and reserved identifiers] | \
bug899c [compile-failure ;; Clang UPC does not properly handle #pragma upc c_code, by undefining UPC keywords and reserved identifiers] | \
bug922 [compile-failure ;; Clang UPC does not properly handle #pragma upc c_code, by undefining UPC keywords and reserved identifiers] | \
bug3057 [compile-pass ; ; Clang UPC does not issue an error when compiling a PTS that refers to a multi-dim shared array with THREADS present in more than one dimension] | \
bug3198 [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gasnet-tests/testconduitspecific [compile-failure ; ; EXTERNAL: Compile-time warning due to gcc 4.5.1 work-around] | \
gasnet-tests/testgasnet-parsync [compile-failure ; ; EXTERNAL: Missing gasnet-udp-parsync library, Makefile problem] | \
guts_main/barrier_neg [run-crash ; ; EXTERNAL: test intended to FAIL, but harness detects as CRASH] | \
gwu-npb-upc/btio-A [compile-failure ; packedsptr || cpu_32 ; Clang UPC does not support block sizes > 65536 in some configurations] | \
gwu-npb-upc/btio-A [run-match ; ; EXTERNAL: Bug 1508 - btio fails with NaNs] | \
gwu-npb-upc/btio-S [run-match ; ; EXTERNAL: Bug 1508 - btio fails with NaNs] | \
gwu-npb-upc/btio-W [compile-failure ; packedsptr || cpu_32 ; Clang UPC does not support block sizes > 65536 in some configurations] | \
gwu-npb-upc/btio-W [run-match ; ; EXTERNAL: Bug 1508 - btio fails with NaNs] | \
gwu-npb-upc/cg-A [run-match ; ; EXTERNAL: bug 653 - NPBs known to fail verification due to application bugs] | \
gwu-npb-upc/cg-S [run-match ; ; EXTERNAL: bug 653 - NPBs known to fail verification due to application bugs] | \
gwu-npb-upc/cg-W [run-match ; ; EXTERNAL: bug 653 - NPBs known to fail verification due to application bugs] | \
gwu-npb-upc/mg-S [run-match ; ; EXTERNAL: bug 653 - NPBs known to fail verification: mg-S with thread count >= 16] | \
gwu-npb-upc/btio-A [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/btio-S [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/btio-W [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/cg-A [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/cg-S [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/cg-W [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/ep-A [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/ep-S [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/ep-W [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/ft-A [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/ft-S [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/ft-W [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/is-A [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/is-S [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/is-W [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/mg-A [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/mg-S [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
gwu-npb-upc/mg-W [compile-crash ; cpu_powerpc64le && structsptr && debug ; EXTERNAL: upsteam clang bug, see clang-upc issue #98] | \
mupc/test_int_precision-int [ run-match ; os_freebsd && cpu_i386 ; lack of precision FreeBSD/i386 ] | \
mupc/test_int_precision-long [ run-match ; os_freebsd && cpu_i386 ; lack of precision FreeBSD/i386 ] | \
pearls/upcfish [run-match ; os_netbsd || os_netbsdelf || os_openbsd ; EXTERNAL: poor rand() on NetBSD and OpenBSD causes premature exit(2)] | \
mupc/test_app_matmult       [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
pearls/z_order1             [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
sobel/sobel-static          [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
sobel/sobel-static-prefetch [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
upc2c-issue-83 [compile-failure ; ; issue-83: Bad code on shared assignment of anon struct] | \
uts/uts-upc-enhanced-t3xxl [run-mem ; os_linux && cpu_32 && network_smp && pshm ; ILP32 address space too small for test with PSHM ] | \
uts/uts-upc-enhanced-t2wl  [run-mem ; os_linux && cpu_32 && network_smp && pshm ; ILP32 address space too small for test with PSHM ] | \
uts/uts-upc-t2wl           [run-mem ; os_linux && cpu_32 && network_smp && pshm ; ILP32 address space too small for test with PSHM ] | \
upc-semantic-checks/assign-pts-with-diff-block-factors-no-cast [compile-warning ; ; Clang UPC issues warning instead of error]
