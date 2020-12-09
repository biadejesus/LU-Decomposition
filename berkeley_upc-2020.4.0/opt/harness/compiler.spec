# Berkeley UPC + UPCR compiler spec file - most values configure-detected
# directory used in upc_compiler and upcrun_command, with command-line override
upc_home = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt

# upc compiler command
upc_compiler = $upc_home$/upcc
#upc_compiler = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/profile/dump/upcc-dump

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
known_failures = 

