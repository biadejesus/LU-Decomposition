/*
 * This utility configures a NPB to be built for a specific class.
 * It creates a file "npbparams.h"
 * in the source directory. This file keeps state information about
 * which size of benchmark is currently being built (so that nothing
 * if unnecessarily rebuilt) and defines (through PARAMETER statements)
 * the number of nodes and class for which a benchmark is being built.

 * The utility takes 3 arguments:
 *       setparams benchmark-name class
 *    benchmark-name is "sp", "bt", etc
 *    class is the size of the benchmark
 * These parameters are checked for the current benchmark. If they
 * are invalid, this program prints a message and aborts.
 * If the parameters are ok, the current npbsize.h (actually just
 * the first line) is read in. If the new parameters are the same as
 * the old, nothing is done, but an exit code is returned to force the
 * user to specify (otherwise the make procedure succeeds but builds a
 * binary of the wrong name).  Otherwise the file is rewritten.
 * Errors write a message (to stdout) and abort.
 *
 * This program makes use of two extra benchmark "classes"
 * class "X" means an invalid specification. It is returned if
 * there is an error parsing the config file.
 * class "U" is an external specification meaning "unknown class"
 *
 * Unfortunately everything has to be case sensitive. This is
 * because we can always convert lower to upper or v.v. but
 * can't feed this information back to the makefile, so typing
 * make CLASS=a and make CLASS=A will produce different binaries.
 *
 * -- CHANGES on BT, SP and CG workloads by GWU - HPCL on May 2003
 *   by F. Cantonnet
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* This is the master version number for this set of
 * NPB benchmarks. It is in an obscure place so people
 * won't accidentally change it.  */

/* #define VERSION "2.3" */ /* all others */
#define VERSION "2.4" /* CG 2.4 */
char _VERSION[] = VERSION;
#undef VERSION
#define VERSION _VERSION

/* controls verbose output from setparams */
/* #define VERBOSE */

#define FILENAME "npbparams.h"
#define DESC_LINE "/* CLASS = %c */\n"
#define THREADS_LINE "/* THREADS = %d */\n"
#define DEF_CLASS_LINE     "#define CLASS '%c'\n"
#define FINDENT  "        "
#define CONTINUE "     > "

void get_info(char *argv[], int *typep, char *classp, int *npb);
void check_info(int type, char class);
void read_info(int type, char *classp, int *nbp);
void write_info(int type, char class, int npb);
void write_sp_info(FILE *fp, char class, int npb);
void write_bt_info(FILE *fp, char class, int npb);
void write_lu_info(FILE *fp, char class, int npb);
void write_mg_info(FILE *fp, char class, int npb);
void write_cg_info(FILE *fp, char class, int npb);
void write_ft_info(FILE *fp, char class, int npb);
void write_ep_info(FILE *fp, char class, int npb);
void write_is_info(FILE *fp, char class, int npb);
void write_compiler_info(int type, FILE *fp);
void write_convertdouble_info(int type, FILE *fp);
void check_line(char *line, const char *label, char *val);
int  check_include_line(char *line, char *filename);
void put_string(FILE *fp, char *name, char *val);
void put_def_string(FILE *fp, const char *name, char *val);
void put_def_variable(FILE *fp, char *name, char *val);
int ilog2(int i);

enum benchmark_types {SP, BT, LU, MG, FT, IS, EP, CG};

int main(int argc, char *argv[])
{
    int type;
    char class, class_old;
    int old_nbp, nbp;

    if (argc != 4) {
        printf("Usage: %s benchmark-name class nb_procs\n", argv[0]);
        exit(1);
    }

    /* Get command line arguments. Make sure they're ok. */
    get_info(argv, &type, &class, &nbp);
    if (class != 'U') {
#ifdef VERBOSE
        printf("setparams: For benchmark %s: class = %c\n",
                argv[1], class);
#endif
        check_info(type, class);
    }

    /* Get old information. */
    read_info(type, &class_old, &old_nbp);
    if (class != 'U') {
        if (class_old != 'X') {
#ifdef VERBOSE
            printf("setparams:     old settings: class = %c\n",
                    class_old);
#endif
        }
    } else {
        printf("setparams:\n\
                *********************************************************************\n\
                * You must specify CLASS to build this benchmark                    *\n\
                * For example, to build a class A benchmark, type                   *\n\
                *       make {benchmark-name} CLASS=A NP=4                          *\n\
                *********************************************************************\n\n");

            if (class_old != 'X') {
#ifdef VERBOSE
                printf("setparams: Previous settings were CLASS=%c \n", class_old);
#endif
            }
        exit(1); /* exit on class==U */
    }

    /* Write out new information if it's different. */
    if( (class != class_old) || (nbp != old_nbp)) {
#ifdef VERBOSE
        printf("setparams: Writing %s\n", FILENAME);
#endif
        write_info(type, class, nbp);
    } else {
#ifdef VERBOSE
        printf("setparams: Settings unchanged. %s unmodified\n", FILENAME);
#endif
    }

    return 0;
}

/* get_info(): Get parameters from command line */
void get_info(char *argv[], int *typep, char *classp, int *nbp)
{
    *classp = *argv[2];
    sscanf( argv[3], "%d", nbp );

    if      (!strcmp(argv[1], "sp") || !strcmp(argv[1], "SP")) *typep = SP;
    else if (!strcmp(argv[1], "bt") || !strcmp(argv[1], "BT")) *typep = BT;
    else if (!strcmp(argv[1], "ft") || !strcmp(argv[1], "FT")) *typep = FT;
    else if (!strcmp(argv[1], "lu") || !strcmp(argv[1], "LU")) *typep = LU;
    else if (!strcmp(argv[1], "mg") || !strcmp(argv[1], "MG")) *typep = MG;
    else if (!strcmp(argv[1], "is") || !strcmp(argv[1], "IS")) *typep = IS;
    else if (!strcmp(argv[1], "ep") || !strcmp(argv[1], "EP")) *typep = EP;
    else if (!strcmp(argv[1], "cg") || !strcmp(argv[1], "CG")) *typep = CG;
    else {
        printf("setparams: Error: unknown benchmark type %s\n", argv[1]);
        exit(1);
    }
}

/*  check_info(): Make sure command line data is ok for this benchmark */
void check_info(int type, char class)
{
    int tmplog;

    /* check class */
    if (class != 'S' &&
            class != 'A' &&
            class != 'B' &&
            class != 'R' &&
            class != 'W' &&
            class != 'C' &&
            class != 'D' ) {
        printf("setparams: Unknown benchmark class %c\n", class);
        printf("setparams: Allowed classes are \"S\", \"A\", \"B\", \"C\" and \"D\"\n");
        exit(1);
    }

    if( (class == 'D') && (type == IS) )
    {
        printf("setparams: Invalid benchmark class %c\n", class );
        printf("setparams: Allowed classes are \"S\", \"A\", \"B\" and \"C\"\n");
        exit(1);
    }
}

/* read_info(): Read previous information from file.
 *              Not an error if file doesn't exist, because this
 *              may be the first time we're running.
 *              Assumes the first line of the file is in a special
 *              format that we understand (since we wrote it).  */
void read_info(int type, char *classp, int *nb)
{
    int nread, gotem = 0;
    char line[200];
    int n;
    FILE *fp;
    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
#ifdef VERBOSE
        printf("setparams: INFO: configuration file %s does not exist (yet)\n", FILENAME);
#endif
        goto abort;
    }

    /* first line of file contains info (fortran), first two lines (C) */
    switch(type) {
        case SP:
        case BT:
        case FT:
        case MG:
        case LU:
        case EP:
        case CG:
            nread = fscanf(fp, DESC_LINE, classp);
            if (nread != 1) {
                printf("setparams: Error parsing config file %s. Ignoring previous settings\n", FILENAME);
                goto abort;
            }
            break;
        case IS:
            nread = fscanf(fp, DEF_CLASS_LINE, classp);
            if (nread != 1) {
                printf("setparams: Error parsing config file %s. Ignoring previous settings\n", FILENAME);
                goto abort;
            }
            break;
        default:
            /* never should have gotten this far with a bad name */
            printf("setparams: (Internal Error) Benchmark type %d unknown to this program\n", type);
            exit(1);
    }

    /* normal_return: */
    nread = fscanf( fp, THREADS_LINE, &n );
    if (nread != 1) {
        printf("setparams: Error parsing config file %s. Ignoring previous settings\n", FILENAME);
        goto abort;
    }
    *nb = n;
    *classp = *classp;
    fclose(fp);
    return;

abort:
    *classp = 'X';
    return;
}

/* write_info(): Write new information to config file.
 *               First line is in a special format so we can read
 *               it in again. Then comes a warning. The rest is all
 *               specific to a particular benchmark.  */
void write_info(int type, char class, int nbp)
{
    FILE *fp;
    fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("setparams: Can't open file %s for writing\n", FILENAME);
        exit(1);
    }

    switch(type) {
        case SP:
        case BT:
        case FT:
        case MG:
        case LU:
        case EP:
        case CG:
            /* Write out the header */
            fprintf(fp, DESC_LINE, class);
            fprintf(fp, THREADS_LINE, nbp );
            /* Print out a warning so bozos don't mess with the file */
            fprintf(fp, "\
/*\n\
  This file is generated automatically by the setparams utility.\n\
  It sets the number of processors and the class of the NPB\n\
  in this directory. Do not modify it by hand.\n\
 */\n");

                break;
        case IS:
            fprintf(fp, DEF_CLASS_LINE, class);
            fprintf(fp, THREADS_LINE, nbp );
            fprintf(fp, "\
                    /*\n\
                      This file is generated automatically by the setparams utility.\n\
                      It sets the number of processors and the class of the NPB\n\
                      in this directory. Do not modify it by hand.   */\n\
                    \n");
            break;
        default:
            printf("setparams: (Internal error): Unknown benchmark type %d\n",
                    type);
            exit(1);
    }

    /* Now do benchmark-specific stuff */
    switch(type) {
        case SP:
            write_sp_info(fp, class, nbp);
            break;
        case BT:
            write_bt_info(fp, class, nbp);
            break;
        case LU:
            write_lu_info(fp, class, nbp);
            break;
        case MG:
            write_mg_info(fp, class, nbp);
            break;
        case IS:
            write_is_info(fp, class, nbp);
            break;
        case FT:
            write_ft_info(fp, class, nbp);
            break;
        case EP:
            write_ep_info(fp, class, nbp);
            break;
        case CG:
            write_cg_info(fp, class, nbp);
            break;
        default:
            printf("setparams: (Internal error): Unknown benchmark type %d\n", type);
            exit(1);
    }
    write_convertdouble_info(type, fp);
    write_compiler_info(type, fp);
    fclose(fp);
    return;
}

/* write_sp_info(): Write SP specific info to config file */
void write_sp_info(FILE *fp, char class, int nbp)
{
    int problem_size, niter;
    const char *dt;
    double d;
    double dd_d;

    /* nbp must be a square number */
    d = sqrt( nbp );
    if( d != floor( d ))
    {
        printf("setparams: The number of threads used must be a SQUARE number\n");
        exit( 1 );
    }

    if      (class == 'S') { problem_size = 12;  dt = "0.015";   niter = 100; }
    else if (class == 'W') { problem_size = 36;  dt = "0.0015";  niter = 400; }
    else if (class == 'A') { problem_size = 64;  dt = "0.0015";  niter = 400; }
    else if (class == 'B') { problem_size = 102; dt = "0.001";   niter = 400; }
    else if (class == 'C') { problem_size = 162; dt = "0.00067"; niter = 400; }
    else {
        printf("setparams: Internal error: invalid class %c\n", class);
        exit(1);
    }
    fprintf(fp, "#define\tPROBLEM_SIZE\t%d\n", problem_size);
    fprintf(fp, "#define\tNITER_DEFAULT\t%d\n", niter);
    fprintf(fp, "#define\tDT_DEFAULT\t%s\n", dt);
    fprintf(fp, "#define\tmaxcells\t%.0lf\n", d );

    dd_d = (problem_size * 1.0);
    dd_d /= d;
    dd_d ++;
    dd_d = floor( dd_d );
    fprintf(fp, "#define\tMAX_CELL_DIM\t%.0lf\n", dd_d );

    dd_d *= dd_d;
    dd_d *= (((double)d)-1.0);
    dd_d *= 120;
    dd_d += 1.0;

    fprintf(fp, "#define\tBUF_SIZE\t%.0lf\n", dd_d );
}

/* write_bt_info(): Write BT specific info to config file */
void write_bt_info(FILE *fp, char class, int nbp)
{
    int problem_size, niter;
    const char *dt;
    double d;
    double dd_d;

    /* nbp must be a square number */
    d = sqrt( nbp );
    if( d != floor( d ))
    {
        printf("setparams: The number of threads used must be a SQUARE number\n");
        exit( 1 );
    }

    if      (class == 'S') { problem_size = 12;  dt = "0.010";   niter = 60; }
    else if (class == 'W') { problem_size = 24;  dt = "0.0008";  niter = 200; }
    else if (class == 'A') { problem_size = 64;  dt = "0.0008";  niter = 200; }
    else if (class == 'B') { problem_size = 102; dt = "0.0003";  niter = 200; }
    else if (class == 'C') { problem_size = 162; dt = "0.0001";  niter = 200; }
    else {
        printf("setparams: Internal error: invalid class %c\n", class);
        exit(1);
    }
    fprintf(fp, "#define\tPROBLEM_SIZE\t%d\n", problem_size);
    fprintf(fp, "#define\tNITER_DEFAULT\t%d\n", niter);
    fprintf(fp, "#define\tDT_DEFAULT\t%s\n", dt);
    fprintf(fp, "#define\tmaxcells\t%.0lf\n", d );

    dd_d = (problem_size * 1.0);
    dd_d /= d;
    dd_d ++;
    dd_d = floor( dd_d );
    fprintf(fp, "#define\tMAX_CELL_DIM\t%.0lf\n", dd_d );

    dd_d *= dd_d;
    dd_d *= (((double)d)-1.0);
    dd_d *= 60;
    dd_d += 1.0;

    fprintf(fp, "#define\tBUF_SIZE\t%.0lf\n", dd_d );
}

/* write_lu_info(): Write SP specific info to config file */
void write_lu_info(FILE *fp, char class, int nbp)
{
    int isiz1, isiz2, itmax, inorm, problem_size;
    int xdiv, ydiv; /* number of cells in x and y direction */
    const char *dt_default;

    if      (class == 'S') { problem_size = 12;  dt_default = "0.5"; itmax = 50; }
    else if (class == 'W') { problem_size = 33;  dt_default = "1.5e-3"; itmax = 300; }
    else if (class == 'A') { problem_size = 64;  dt_default = "2.0"; itmax = 250; }
    else if (class == 'B') { problem_size = 102; dt_default = "2.0"; itmax = 250; }
    else if (class == 'C') { problem_size = 162; dt_default = "2.0"; itmax = 250; }
    else {
        printf("setparams: Internal error: invalid class %c\n", class);
        exit(1);
    }
    inorm = itmax;
    isiz1 = problem_size;
    isiz2 = problem_size;

    fprintf(fp, "\n/* full problem size */\n");
    fprintf(fp, "#define\tISIZ1\t%d\n", problem_size);
    fprintf(fp, "#define\tISIZ2\t%d\n", problem_size);
    fprintf(fp, "#define\tISIZ3\t%d\n", problem_size);

    fprintf(fp, "/* number of iterations and how often to print the norm */\n");
    fprintf(fp, "#define\tITMAX_DEFAULT\t%d\n", itmax);
    fprintf(fp, "#define\tINORM_DEFAULT\t%d\n", inorm);

    fprintf(fp, "#define\tDT_DEFAULT\t%s\n", dt_default);
}

/* write_mg_info(): Write MG specific info to config file */
void write_mg_info(FILE *fp, char class, int nbp)
{
    int problem_size, nit, log2_size, lt_default, lm;
    int ndim1, ndim2, ndim3;
    if      (class == 'S') { problem_size = 32;   nit = 4;  }
    else if (class == 'W') { problem_size = 64;   nit = 40; }
    else if (class == 'A') { problem_size = 256;  nit = 4;  }
    else if (class == 'B') { problem_size = 256;  nit = 20; }
    else if (class == 'C') { problem_size = 512;  nit = 20; }
    else if (class == 'D') { problem_size = 1024; nit = 50; }
    else {
        printf("setparams: Internal error: invalid class type %c\n", class);
        exit(1);
    }
    log2_size = ilog2(problem_size);
    /* lt is log of largest total dimension */
    lt_default = log2_size;
    /* log of log of maximum dimension on a node */
    lm = log2_size;
    ndim1 = lm;
    ndim3 = log2_size;
    ndim2 = log2_size;

    fprintf(fp, "#define\tNX_DEFAULT\t%d\n", problem_size);
    fprintf(fp, "#define\tNY_DEFAULT\t%d\n", problem_size);
    fprintf(fp, "#define\tNZ_DEFAULT\t%d\n", problem_size);
    fprintf(fp, "#define\tNIT_DEFAULT\t%d\n", nit);
    fprintf(fp, "#define\tLM\t%d\n", lm);
    fprintf(fp, "#define\tLT_DEFAULT\t%d\n", lt_default);
    fprintf(fp, "#define\tDEBUG_DEFAULT\t%d\n", 0);
    fprintf(fp, "#define\tNDIM1\t%d\n", ndim1);
    fprintf(fp, "#define\tNDIM2\t%d\n", ndim2);
    fprintf(fp, "#define\tNDIM3\t%d\n", ndim3);
}

/* write_is_info(): Write IS specific info to config file */
void write_is_info(FILE *fp, char class, int nbp)
{
    int m1, m2, m3 ;
    if( class != 'S' &&
            class != 'W' &&
            class != 'A' &&
            class != 'B' &&
            class != 'C')
    {
        printf("setparams: Internal error: invalid class type %c\n", class);
        exit(1);
    }
}

/* write_cg_info(): Write CG specific info to config file */
void write_cg_info(FILE *fp, char class, int nbp)
{
    int na,nonzer,niter, ii, jj;
    const char *shift,*rcond="1.0e-1";
    const char *shiftS="10.0",
         *shiftW="12.0",
         *shiftA="20.0",
         *shiftB="60.0",
         *shiftC="110.0",
         *shiftD="500.0";
    int num_proc_cols, num_proc_rows;

    if( class == 'S' )
    { na=1400; nonzer=7; niter=15; shift=shiftS; }
    else if( class == 'W' )
    { na=7000; nonzer=8; niter=15; shift=shiftW; }
    else if( class == 'A' )
    { na=14000; nonzer=11; niter=15; shift=shiftA; }
    else if( class == 'B' )
    { na=75000; nonzer=13; niter=75; shift=shiftB; }
    else if( class == 'C' )
    { na=150000; nonzer=15; niter=75; shift=shiftC; }
    else if( class == 'D' )
    { na=1500000; nonzer=21; niter=100; shift=shiftD; }
    else
    {
        printf("setparams: Internal error: invalid class type %c\n", class);
        exit(1);
    }
    fprintf( fp, "#define\tNA\t%d\n", na);
    fprintf( fp, "#define\tNONZER\t%d\n", nonzer);
    fprintf( fp, "#define\tNITER\t%d\n", niter);
    fprintf( fp, "#define\tSHIFT\t%s\n", shift);
    fprintf( fp, "#define\tRCOND\t%s\n", rcond);

    num_proc_cols = num_proc_rows = ilog2( nbp ) / 2;
    if( (num_proc_cols + num_proc_rows) != ilog2( nbp ))
        num_proc_cols ++;

    jj = num_proc_cols;
    num_proc_cols = 1;
    for( ii=0; ii<jj; ii++ )
    {
        num_proc_cols *= 2;
    }

    jj = num_proc_rows;
    num_proc_rows = 1;
    for( ii=0; ii<jj; ii++ )
    {
        num_proc_rows *= 2;
    }

    fprintf( fp, "#define\tNUM_PROC_COLS\t%d\n", num_proc_cols );
    fprintf( fp, "#define\tNUM_PROC_COLS2\t%d\n", (2*num_proc_cols) );
    fprintf( fp, "#define\tNUM_PROC_ROWS\t%d\n", num_proc_rows );
    fprintf( fp, "#define\tDIM_PER_THREAD\t%d\n", (na/num_proc_rows)+2 );
}

/* write_ft_info(): Write FT specific info to config file */
void write_ft_info(FILE *fp, char class, int nbp)
{
    /* easiest way (given the way the benchmark is written)
     * is to specify log of number of grid points in each
     * direction m1, m2, m3. nt is the number of iterations */
    int nx, ny, nz, maxdim, niter, np_min;
    double d;

    if      (class == 'S') { nx = 64;   ny = 64;   nz = 64;   niter = 6;}
    else if (class == 'W') { nx = 128;  ny = 128;  nz = 32;   niter = 6;}
    else if (class == 'A') { nx = 256;  ny = 256;  nz = 128;  niter = 6;}
    else if (class == 'B') { nx = 512;  ny = 256;  nz = 256;  niter =20;}
    else if (class == 'C') { nx = 512;  ny = 512;  nz = 512;  niter =20;}
    else if (class == 'D') { nx = 2048; ny = 1024; nz = 1024; niter =25;}
    else {
        printf("setparams: Internal error: invalid class type %c\n", class);
        exit(1);
    }
    maxdim = nx;
    if (ny > maxdim) maxdim = ny;
    if (nz > maxdim) maxdim = nz;
    fprintf(fp, "#define\tNX\t%d\n", nx);
    fprintf(fp, "#define\tNY\t%d\n", ny);
    fprintf(fp, "#define\tNZ\t%d\n", nz);
    fprintf(fp, "#define\tMAXDIM\t%d\n", maxdim);
    fprintf(fp, "#define\tNITER_DEFAULT\t%d\n", niter);
    /* for 2.3 specific */
    fprintf(fp, "#define\tNTOTAL\t%d\n", nx*ny*nz);
    /* for 2.4 specific */
    d = 1.0 * nx * ny * nz;
    fprintf(fp, "#define\tNP_MIN\t%d\n", nbp );
    fprintf(fp, "#define\tNTDIVNP\t%d\n", ((nx*ny)/nbp)*nz );
    fprintf(fp, "#define\tNTOTAL_F\t%lf\n", d );
}

/* write_ep_info(): Write EP specific info to config file */
void write_ep_info(FILE *fp, char class, int nbp)
{
    /* easiest way (given the way the benchmark is written)
     * is to specify log of number of grid points in each
     * direction m1, m2, m3. nt is the number of iterations */
    int m;
    if      (class == 'S') { m = 24; }
    else if (class == 'W') { m = 25; }
    else if (class == 'A') { m = 28; }
    else if (class == 'B') { m = 30; }
    else if (class == 'C') { m = 32; }
    else if (class == 'D') { m = 36; }
    else {
        printf("setparams: Internal error: invalid class type %c\n", class);
        exit(1);
    }

    fprintf(fp, "#define\tCLASS\t \'%c\'\n", class);
    fprintf(fp, "#define\tM\t%d\n", m);
}


/* This is a gross hack to allow the benchmarks to
 * print out how they were compiled. Various other ways
 * of doing this have been tried and they all fail on
 * some machine - due to a broken "make" program, or
 * F77 limitations, of whatever. Hopefully this will
 * always work because it uses very portable C. Unfortunately
 * it relies on parsing the make.def file - YUK.
 * If your machine doesn't have <string.h> or <ctype.h>, happy hacking!
 */

#define VERBOSE
#define LL 400
#include <stdio.h>
#define DEFFILE "../config/make.def"
#define DEFAULT_MESSAGE "(none)"
#undef isspace /* fix warnings from some buggy ctype.h (NetBSD) */
void write_compiler_info(int type, FILE *fp)
{
    FILE *deffile;
    char line[LL];
    char f77[LL], flink[LL], f_lib[LL], f_inc[LL], fflags[LL], flinkflags[LL];
    char compiletime[LL], randfile[LL];
    char cc[LL], cflags[LL], clink[LL], clinkflags[LL],
         c_lib[LL], c_inc[LL];
    struct tm *tmp;
    time_t t;
    deffile = fopen(DEFFILE, "r");
    if (deffile == NULL) {
        printf("\n\
                setparams: File %s doesn't exist. To build the NAS benchmarks\n\
                you need to create is according to the instructions\n\
                in the README in the main directory and comments in \n\
                the file config/make.def.template\n", DEFFILE);
        exit(1);
    }
    strcpy(f77, DEFAULT_MESSAGE);
    strcpy(flink, DEFAULT_MESSAGE);
    strcpy(f_lib, DEFAULT_MESSAGE);
    strcpy(f_inc, DEFAULT_MESSAGE);
    strcpy(fflags, DEFAULT_MESSAGE);
    strcpy(flinkflags, DEFAULT_MESSAGE);
    strcpy(randfile, DEFAULT_MESSAGE);
    strcpy(cc, DEFAULT_MESSAGE);
    strcpy(cflags, DEFAULT_MESSAGE);
    strcpy(clink, DEFAULT_MESSAGE);
    strcpy(clinkflags, DEFAULT_MESSAGE);
    strcpy(c_lib, DEFAULT_MESSAGE);
    strcpy(c_inc, DEFAULT_MESSAGE);

    while (fgets(line, LL, deffile) != NULL) {
        if (*line == '#') continue;
        /* yes, this is inefficient. but it's simple! */
        check_line(line, "F77", f77);
        check_line(line, "FLINK", flink);
        check_line(line, "F_LIB", f_lib);
        check_line(line, "F_INC", f_inc);
        check_line(line, "FFLAGS", fflags);
        check_line(line, "FLINKFLAGS", flinkflags);
        check_line(line, "RAND", randfile);
        check_line(line, "CC", cc);
        check_line(line, "CFLAGS", cflags);
        check_line(line, "CLINK", clink);
        check_line(line, "CLINKFLAGS", clinkflags);
        check_line(line, "C_LIB", c_lib);
        check_line(line, "C_INC", c_inc);
    }

#ifdef BUPC_TEST_HARNESS
  { /* For the benefit of (much) older backend compilers */
#endif
    FILE *last_conf = fopen("../config/last_parameters", "r");
    if( last_conf != NULL ){
        while (fgets(line, LL, last_conf) != NULL) {
            if (*line == '#')
                continue;
            check_line(line, "F77", f77);
            check_line(line, "FLINK", flink);
            check_line(line, "F_LIB", f_lib);
            check_line(line, "F_INC", f_inc);
            check_line(line, "FFLAGS", fflags);
            check_line(line, "FLINKFLAGS", flinkflags);
            check_line(line, "RAND", randfile);
            check_line(line, "CC", cc);
            check_line(line, "CFLAGS", cflags);
            check_line(line, "CLINK", clink);
            check_line(line, "CLINKFLAGS", clinkflags);
            check_line(line, "C_LIB", c_lib);
            check_line(line, "C_INC", c_inc);
        }
    }else{
        printf("WARNING: Unable to open ../config/last_parameters for reading\n");
    }
#ifdef BUPC_TEST_HARNESS
  }
#endif

    (void) time(&t);
    tmp = localtime(&t);
    (void) strftime(compiletime, (size_t)LL, "%d %b %Y", tmp);

    switch(type) {
        case FT:
        case SP:
        case BT:
        case MG:
        case LU:
        case EP:
        case CG:
            put_def_string(fp, "COMPILETIME", compiletime);
            put_def_string(fp, "NPBVERSION", VERSION);
            put_def_string(fp, "NPB_CS1", cc);
            put_def_string(fp, "NPB_CS2", clink);
            put_def_string(fp, "NPB_CS3", c_lib);
            put_def_string(fp, "NPB_CS4", c_inc);
            put_def_string(fp, "NPB_CS5", cflags);
            put_def_string(fp, "NPB_CS6", clinkflags);
            put_def_string(fp, "NPB_CS7", randfile);
            break;
        case IS:
            put_def_string(fp, "COMPILETIME", compiletime);
            put_def_string(fp, "NPBVERSION", VERSION);
            put_def_string(fp, "CC", cc);
            put_def_string(fp, "CFLAGS", cflags);
            put_def_string(fp, "CLINK", clink);
            put_def_string(fp, "CLINKFLAGS", clinkflags);
            put_def_string(fp, "C_LIB", c_lib);
            put_def_string(fp, "C_INC", c_inc);
            break;
        default:
            printf("setparams: (Internal error): Unknown benchmark type %d\n",
                    type);
            exit(1);
    }
}

void check_line(char *line, const char *label, char *val)
{
    char *original_line;
    original_line = line;
    /* compare beginning of line and label */
    while (*label != '\0' && *line == *label) {
        line++; label++;
    }
    /* if *label is not EOS, we must have had a mismatch */
    if (*label != '\0') return;
    /* if *line is not a space, actual label is longer than test label */
    if (!isspace(*line) && *line != '=') return ;
    /* skip over white space */
    while (isspace(*line)) line++;
    /* next char should be '=' */
    if (*line != '=') return;
    /* skip over white space */
    while (isspace(*++line));
    /* if EOS, nothing was specified */
    if (*line == '\0') return;
    /* finally we've come to the value */
    strcpy(val, line);
    /* chop off the newline at the end */
    val[strlen(val)-1] = '\0';
    if (val[strlen(val) - 1] == '\\') {
        printf("\n\
                setparams: Error in file make.def. Because of the way in which\n\
                command line arguments are incorporated into the\n\
                executable benchmark, you can't have any continued\n\
                lines in the file make.def, that is, lines ending\n\
                with the character \"\\\". Although it may be ugly, \n\
                you should be able to reformat without continuation\n\
                lines. The offending line is\n\
                %s\n", original_line);
        exit(1);
    }
}

int check_include_line(char *line, char *filename)
{
    const char *include_string = "include";
    /* compare beginning of line and "include" */
    while (*include_string != '\0' && *line == *include_string) {
        line++; include_string++;
    }
    /* if *include_string is not EOS, we must have had a mismatch */
    if (*include_string != '\0') return(0);
    /* if *line is not a space, first word is not "include" */
    if (!isspace(*line)) return(0);
    /* skip over white space */
    while (isspace(*++line));
    /* if EOS, nothing was specified */
    if (*line == '\0') return(0);
    /* next keyword should be name of include file in *filename */
    while (*filename != '\0' && *line == *filename) {
        line++; filename++;
    }
    if (*filename != '\0' ||
            (*line != ' ' && *line != '\0' && *line !='\n')) return(0);
    else return(1);
}

#define MAXL 46
void put_string(FILE *fp, char *name, char *val)
{
    int len;
    len = strlen(val);
    if (len > MAXL) {
        val[MAXL] = '\0';
        val[MAXL-1] = '.';
        val[MAXL-2] = '.';
        val[MAXL-3] = '.';
        len = MAXL;
    }
    fprintf(fp, "%scharacter*%d %s\n", FINDENT, len, name);
    fprintf(fp, "%sparameter (%s=\'%s\')\n", FINDENT, name, val);
}

/* NOTE: is the ... stuff necessary in C? */
void put_def_string(FILE *fp, const char *name, char *val)
{
    int len;
    len = strlen(val);
    if (len > MAXL) {
        val[MAXL] = '\0';
        val[MAXL-1] = '.';
        val[MAXL-2] = '.';
        val[MAXL-3] = '.';
        len = MAXL;
    }
    fprintf(fp, "#define %s \"%s\"\n", name, val);
}

void put_def_variable(FILE *fp, char *name, char *val)
{
    int len;
    len = strlen(val);
    if (len > MAXL) {
        val[MAXL] = '\0';
        val[MAXL-1] = '.';
        val[MAXL-2] = '.';
        val[MAXL-3] = '.';
        len = MAXL;
    }
    fprintf(fp, "#define %s %s\n", name, val);
}

/* integer log base two. Return error is argument isn't
 * a power of two or is less than or equal to zero */
int ilog2(int i)
{
    int log2;
    int exp2 = 1;
    if (i <= 0) return(-1);

    for (log2 = 0; log2 < 20; log2++) {
        if (exp2 == i) return(log2);
        exp2 *= 2;
    }
    return(-1);
}

void write_convertdouble_info(int type, FILE *fp)
{
    switch(type) {
        case SP:
        case BT:
        case LU:
        case FT:
        case MG:
        case EP:
        case CG:
#ifdef CONVERTDOUBLE
            fprintf(fp, "#define\tCONVERTDOUBLE\tTRUE\n");
#else
            fprintf(fp, "#define\tCONVERTDOUBLE\tFALSE\n");
#endif
            break;
    }
}
