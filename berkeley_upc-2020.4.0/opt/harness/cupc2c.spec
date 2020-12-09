# Clang upc2c translator + UPCR compiler spec file - most values configure-detected
# directory used in upc_compiler and upcrun_command, with command-line override
upc_home = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt

# upc compiler command
#
# We add some flags to modify clang-upc2c's behavior ONLY for the runs of the
# harness.  The end-user will still experience the behaviors which are "native"
# to Clang:
#  -Wno-duplicate-decl-specifier
#     C99 allows duplication of type qualifiers, making them idempotent.  UPC
#     extends this to apply to "relaxed" and "strict".  However, Clang warns by
#     default about duplicated qualifiers.  This flag suppresses this warning
#     while ensuring that the harness does not hide the same warning should it
#     ever come from clang as a back-end to the Berkeley translator.
#  -Werror=pointer-arith
#     C99 does not permit sizeof(void) or arithmetic on void*, while Clang (and
#     gcc) support this as an extension.  However, there are multiple tests
#     expecting the stricter behavior AND some back-end compilers cannot accept
#     the resulting translated code.
#
upc_compiler = $upc_home$/upcc -Ww,-Wno-duplicate-decl-specifier -Ww,-Werror=pointer-arith

# upc run command
# Following replacements are performed:
# %N - number of UPC threads
# %P - program executable
# %A - arguments to program
# %B - berkeley-specific upcrun arguments (should appear if and only if this is Berkeley upcrun)
upcrun_command = $upc_home$/upcrun -q -n %N %B %P %A

# list of supported compiler features
feature_list = trans_bupc,pragma_upc_code,driver_upcc,runtime_upcr,gasnet,upc_collective,upc_io,upc_memcpy_async,upc_memcpy_vis,upc_ptradd,upc_thread_distance,upc_tick,upc_sem,upc_dump_shared,upc_trace_printf,upc_trace_mask,upc_local_to_shared,upc_all_free,upc_atomics,pupc,upc_types,upc_castable,upc_nb,nodebug,notrace,nostats,nodebugmalloc,nogasp,segment_fast,os_linux,cpu_x86_64,cpu_64,cc_gnu,packedsptr,upc_io_64  gasnet_has_smp gasnet_has_udp gasnet_has_mpi gasnet_has_ibv gasnet_has_seq gasnet_has_par cc_subfamily_gnu

# default sysconf file to use
default_sysconf = smp-interactive

# option to pass upc compiler to get %T static threads
upc_static_threads_option = -T %T

# option for performing just the source-to-source compile step
# or empty if not supported by the compiler
upc_trans_option = -trans

# colon-delimited path where to find harness.conf files
suite_path = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet/tests/upcr-harness:/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/upc-tests:/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/upc-examples

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
benchmarks/upc_io_test [compile-failure ; os_solaris && !cc_clang ; clang-upc2c issue #112 - Problems w/ ___errno on Solaris] | \
benchmarks/strided-comm [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
UPC-Atomic-RefImp/atomic_test [run-limit ; cc_xlc && nodebug ; XLC -O3 permits non-IEEE handling of Infinities] | \
RandomAccess/RandomAccess_UPC [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
upc-tutorial/heat_4 [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
cg/cg [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
mg/mg [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
bug6 [run-all ; cpu_32 && structsptr; EXTERNAL: test requires over 2GB per thread] | \
bug87 [compile-failure ; cc_pgi ; EXTERNAL: Bug 3386 - cupc2c + pgi incompatibility within tgmath.h on Linux] | \
bug87 [compile-failure ; cc_sun ; EXTERNAL: Bug 3737 - cupc2c + suncc incompatibility within tgmath.h on Linux] | \
bug247 [compile-failure ; cpu_32 ; Clang UPC does not support huge pointer offsets on 32-bit targets] | \
bug544 [compile-failure ; cc_xlc ; EXTERNAL: XLC cannot handle the initializer for pcrazy ] | \
bug846 [compile-failure ; cpu_32 ; Clang UPC does not support the declaration of huge arrays on 32-bit targets] | \
bug972 [compile-warning ; ; clang-upc2c issue #114 - Missing some brackets in struct initializers] | \
bug1080 [compile-warning ; ; clang-upc2c issue #114 - Missing some brackets in struct initializers] | \
bug3057 [compile-pass ; ; Clang UPC does not issue an error when compiling a PTS that refers to a multi-dim shared array with THREADS present in more than one dimension] | \
bug3192 [compile-failure ; cc_open64 ; EXTERNAL: opencc has no support for the tested C99 syntax] |\
bug3192 [compile-failure ; cc_pgi ; EXTERNAL: Bug 3388 - pgc 16.10 rejects declarations in bug3192.c] | \
bug3210 [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
bug3210 [compile-failure ; os_solaris && cc_sun ; clang-upc issue #75: fragile private stdarg.h] | \
bug3230 [run-match ; cc_sun && nodebug ; EXTERNAL: SunC optimizer oddness on bug3230] | \
gasnet-tests/testconduitspecific [compile-failure ; ; EXTERNAL: Compile-time warning due to gcc 4.5.1 work-around] | \
gasnet-tests/testgasnet-parsync [compile-failure ; ; EXTERNAL: Missing gasnet-udp-parsync library, Makefile problem] | \
guts_main/barrier_neg [run-crash ; ; EXTERNAL: test intended to FAIL, but harness detects as CRASH] | \
gwu-npb-upc/btio-A [compile-failure ; packedsptr || cpu_32 ; Clang UPC does not support block sizes > 65536 in some configurations] | \
gwu-npb-upc/btio-A [run-match ; ; EXTERNAL: Bug 1508 - btio fails with NaNs] | \
gwu-npb-upc/btio-S [run-match ; ; EXTERNAL: Bug 1508 - btio fails with NaNs] | \
gwu-npb-upc/btio-W [compile-failure ; packedsptr || cpu_32 ; Clang UPC does not support block sizes > 65536 in some configurations] | \
gwu-npb-upc/btio-W [run-match ; ; EXTERNAL: Bug 1508 - btio fails with NaNs] | \
gwu-npb-upc/btio-S [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
gwu-npb-upc/btio-W [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
gwu-npb-upc/btio-A [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
gwu-npb-upc/cg-A [run-match ; ; EXTERNAL: bug 653 - NPBs known to fail verification due to application bugs] | \
gwu-npb-upc/cg-S [run-match ; ; EXTERNAL: bug 653 - NPBs known to fail verification due to application bugs] | \
gwu-npb-upc/cg-W [run-match ; ; EXTERNAL: bug 653 - NPBs known to fail verification due to application bugs] | \
gwu-npb-upc/mg-S [run-match ; ; EXTERNAL: bug 653 - NPBs known to fail verification: mg-S with thread count >= 16] | \
gwu-npb-upc/mg-S [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
gwu-npb-upc/mg-W [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
gwu-npb-upc/mg-A [compile-warning ; cc_sun ; clang-upc2c issue #111 - Undeclared __isoc99...scanf() functions in output] | \
mupc/test_locks2-double [run-time ; cc_xlc && nodebug ; xlc-specific hang in sleep()] | \
mupc/test_locks2-float  [run-time ; cc_xlc && nodebug ; xlc-specific hang in sleep()] | \
mupc/test_locks2-int    [run-time ; cc_xlc && nodebug ; xlc-specific hang in sleep()] | \
mupc/test_locks2-long   [run-time ; cc_xlc && nodebug ; xlc-specific hang in sleep()] | \
mupc/test_int_multilocks-double [run-time ; cc_xlc && nodebug ; xlc-specific hang in sleep()] | \
mupc/test_int_multilocks-float  [run-time ; cc_xlc && nodebug ; xlc-specific hang in sleep()] | \
mupc/test_int_multilocks-int    [run-time ; cc_xlc && nodebug ; xlc-specific hang in sleep()] | \
mupc/test_int_multilocks-long   [run-time ; cc_xlc && nodebug ; xlc-specific hang in sleep()] | \
mupc/test_int_memstring-double [run-match ; (cc_pathscale || cc_open64) && nodebug && nopthreads ; EXTERNAL: bug2138 in pathcc and open64] | \
mupc/test_int_memstring-float  [run-match ; (cc_pathscale || cc_open64) && nodebug && nopthreads ; EXTERNAL: bug2138 in pathcc and open64] | \
pearls/upcfish [run-match ; os_netbsd || os_netbsdelf || os_openbsd ; EXTERNAL: poor rand() on NetBSD and OpenBSD causes premature exit(2)] | \
mupc/test_app_matmult       [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
pearls/z_order1             [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
sobel/sobel-static          [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
sobel/sobel-static-prefetch [compile-failure ; narrow_phase ; No syntax to express BlockSize constraint] | \
upc2c-issue-83 [compile-failure ; ; clang-upc2c issue-83: Bad code on shared assignment of anon struct] | \
upc2c-issue-99 [run-match ; ; clang-upc2c issue-99: incorrect initializer for definite shared arrays] | \
upc2c-issue-99 [run-crash ; ; clang-upc2c issue-99: incorrect initializer for definite shared arrays] | \
clang-upc-issue-75 [compile-failure ; os_solaris && cpu_i386; clang-upc issue #75: fragile private stdarg.h] | \
upc-semantic-checks/assign-pts-with-diff-block-factors-no-cast [compile-warning ; ; Clang UPC issues warning instead of error] | \
uts/uts-upc-enhanced-t3xxl [run-mem ; os_linux && cpu_32 && network_smp && pshm ; ILP32 address space too small for test with PSHM ] | \
uts/uts-upc-enhanced-t2wl  [run-mem ; os_linux && cpu_32 && network_smp && pshm ; ILP32 address space too small for test with PSHM ] | \
uts/uts-upc-t2wl           [run-mem ; os_linux && cpu_32 && network_smp && pshm ; ILP32 address space too small for test with PSHM ] | \
uts/uts-upc-t1 [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-dcq-t1 [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-enhanced-t1 [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-t4 [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-dcq-t4 [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-enhanced-t4 [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-t2l [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-dcq-t2l [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-enhanced-t2l [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-t3l [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-dcq-t3l [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-enhanced-t3l [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-t3xxl [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-dcq-t3xxl [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-enhanced-t3xxl [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-t2wl [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-dcq-t2wl [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only] | \
uts/uts-upc-enhanced-t2wl [run-match ; cc_open64 && debug && pthreads ; Bug 3042 - Open64-specific UTS validation failures, dbg only]