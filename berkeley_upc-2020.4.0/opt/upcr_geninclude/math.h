/*
  Note that this is a half-wrapper:
  + On first preprocess this is a replacement
  + On second preprocess this is a pass-through wrapper
*/
#ifdef __BERKELEY_UPC_FIRST_PREPROCESS__
/* A stub math.h file - we want to use all the math functions 
   provided by the backend compiler, but don't want to parse 
   the troublesome math.h file that comes with the native compiler,
   because it often contains embedded inline assembly and/or
   compiler-specific builtins/pragmas.
   This is a temporary workaround.
 */

#ifndef  _MATH_H_
#define  _MATH_H_

typedef float float_t;
typedef double double_t;

/* these are all supposed to be preprocess-time constant expressions,
   but hopefully noone will notice */
#ifndef HUGE_VAL
extern double HUGE_VAL;
#endif
#ifndef HUGE_VALF
extern float HUGE_VALF;
#endif
#ifndef HUGE_VALL
extern long double HUGE_VALL;
#endif
#ifndef INFINITY
extern float INFINITY;
#endif
#ifndef NAN
extern float NAN;
#endif

#ifndef FP_INFINITE
extern int FP_INFINITE;
#endif
#ifndef FP_NAN
extern int FP_NAN;
#endif
#ifndef FP_NORMAL
extern int FP_NORMAL;
#endif
#ifndef FP_SUBNORMAL
extern int FP_SUBNORMAL;
#endif
#ifndef FP_ZERO
extern int FP_ZERO;
#endif

#ifndef FP_ILOGB0
extern int FP_ILOGB0;
#endif
#ifndef FP_ILOGBNAN
extern int FP_ILOGBNAN;
#endif

#define MATH_ERRNO 1
#define MATH_ERREXCEPT 2
extern int math_errhandling;


/* all of the math functions in ANSI-C 99 */

typedef long double real_floating_t;

double  	acos(double _x);
float   	acosf(float _x);
double  	acosh(double _x);
float   	acoshf(float _x);
long double	acoshl(long double _x);
long double	acosl(long double _x);
double  	asin(double _x);
float   	asinf(float _x);
double  	asinh(double _x);
float   	asinhf(float _x);
long double	asinhl(long double _x);
long double	asinl(long double _x);
double  	atan(double _x);
double  	atan2(double _y, double _x);
float   	atan2f(float _y, float _x);
long double	atan2l(long double _y, long double _x);
float   	atanf(float _x);
double  	atanh(double _x);
float   	atanhf(float _x);
long double	atanhl(long double _x);
long double	atanl(long double _x);
double  	cbrt(double _x);
float   	cbrtf(float _x);
long double	cbrtl(long double _x);
double  	ceil(double _x);
float   	ceilf(float _x);
long double	ceill(long double _x);
double  	copysign(double _x, double _y);
float   	copysignf(float _x, float _y);
long double	copysignl(long double _x, long double _y);
double  	cos(double _x);
float   	cosf(float _x);
double  	cosh(double _x);
float   	coshf(float _x);
long double	coshl(long double _x);
long double	cosl(long double _x);
double  	erf(double _x);
double  	erfc(double _x);
float   	erfcf(float _x);
long double	erfcl(long double _x);
float   	erff(float _x);
long double	erfl(long double _x);
double  	exp(double _x);
double  	exp2(double _x);
float   	exp2f(float _x);
long double	exp2l(long double _x);
float   	expf(float _x);
long double	expl(long double _x);
double  	expm1(double _x);
float   	expm1f(float _x);
long double	expm1l(long double _x);
double  	fabs(double _x);
float   	fabsf(float _x);
long double	fabsl(long double _x);
double  	fdim(double _x, double _y);
float   	fdimf(float _x, float _y);
long double	fdiml(long double _x, long double _y);
double  	floor(double _x);
float   	floorf(float _x);
long double	floorl(long double _x);
double  	fma(double _x, double _y, double _z);
float   	fmaf(float _x, float _y, float _z);
long double	fmal(long double _x, long double _y, long double _z);
double  	fmax(double _x, double _y);
float   	fmaxf(float _x, float _y);
long double	fmaxl(long double _x, long double _y);
double  	fmin(double _x, double _y);
float   	fminf(float _x, float _y);
long double	fminl(long double _x, long double _y);
double  	fmod(double _x, double _y);
float   	fmodf(float _x, float _y);
long double	fmodl(long double _x, long double _y);
int     	fpclassify(real_floating_t _x);
double  	frexp(double _value, int *_exp);
float   	frexpf(float _value, int *_exp);
long double	frexpl(long double _value, int *_exp);
double  	hypot(double _x, double _y);
float   	hypotf(float _x, float _y);
long double	hypotl(long double _x, long double _y);
int     	ilogb(double _x);
int     	ilogbf(float _x);
int     	ilogbl(long double _x);
int     	isfinite(real_floating_t _x);
int     	isgreater(real_floating_t _x, real_floating_t _y);
int     	isgreaterequal(real_floating_t _x, real_floating_t _y);
int     	isinf(real_floating_t _x);
int     	isless(real_floating_t _x, real_floating_t _y);
int     	islessequal(real_floating_t _x, real_floating_t _y);
int     	islessgreater(real_floating_t _x, real_floating_t _y);
int     	isnan(real_floating_t _x);
int     	isnormal(real_floating_t _x);
int     	isunordered(real_floating_t _x, real_floating_t _y);
double  	ldexp(double _x, int _exp);
float   	ldexpf(float _x, int _exp);
long double	ldexpl(long double _x, int _exp);
double  	lgamma(double _x);
float   	lgammaf(float _x);
long double	lgammal(long double _x);
long long int	llrint(double _x);
long long int	llrintf(float _x);
long long int	llrintl(long double _x);
long long int	llround(double _x);
long long int	llroundf(float _x);
long long int	llroundl(long double _x);
double  	log(double _x);
double  	log10(double _x);
float   	log10f(float _x);
long double	log10l(long double _x);
double  	log1p(double _x);
float   	log1pf(float _x);
long double	log1pl(long double _x);
double  	log2(double _x);
float   	log2f(float _x);
long double	log2l(long double _x);
double  	logb(double _x);
float   	logbf(float _x);
long double	logbl(long double _x);
float   	logf(float _x);
long double	logl(long double _x);
long int	lrint(double _x);
long int	lrintf(float _x);
long int	lrintl(long double _x);
long int	lround(double _x);
long int	lroundf(float _x);
long int	lroundl(long double _x);
double  	modf(double _value, double *_iptr);
float   	modff(float _value, float *_iptr);
long double	modfl(long double _value, long double *_iptr);
double  	nan(const char *_tagp);
float   	nanf(const char *_tagp);
long double	nanl(const char *_tagp);
double  	nearbyint(double _x);
float   	nearbyintf(float _x);
long double	nearbyintl(long double _x);
double  	nextafter(double _x, double _y);
float   	nextafterf(float _x, float _y);
long double	nextafterl(long double _x, long double _y);
double  	nexttoward(double _x, long double _y);
float   	nexttowardf(float _x, long double _y);
long double	nexttowardl(long double _x, long double _y);
double  	pow(double _x, double _y);
float   	powf(float _x, float _y);
long double	powl(long double _x, long double _y);
double  	remainder(double _x, double _y);
float   	remainderf(float _x, float _y);
long double	remainderl(long double _x, long double _y);
double  	remquo(double _x, double _y, int *_quo);
float   	remquof(float _x, float _y, int *_quo);
long double	remquol(long double _x, long double _y, int *_quo);
double  	rint(double _x);
float   	rintf(float _x);
long double	rintl(long double _x);
double  	round(double _x);
float   	roundf(float _x);
long double	roundl(long double _x);
double  	scalbln(double _x, long int _n);
float   	scalblnf(float _x, long int _n);
long double	scalblnl(long double _x, long int _n);
double  	scalbn(double _x, int _n);
float   	scalbnf(float _x, int _n);
long double	scalbnl(long double _x, int _n);
int     	signbit(real_floating_t _x);
double  	sin(double _x);
float   	sinf(float _x);
double  	sinh(double _x);
float   	sinhf(float _x);
long double	sinhl(long double _x);
long double	sinl(long double _x);
double  	sqrt(double _x);
float   	sqrtf(float _x);
long double	sqrtl(long double _x);
double  	tan(double _x);
float   	tanf(float _x);
double  	tanh(double _x);
float   	tanhf(float _x);
long double	tanhl(long double _x);
long double	tanl(long double _x);
double  	tgamma(double _x);
float   	tgammaf(float _x);
long double	tgammal(long double _x);
double  	trunc(double _x);
float   	truncf(float _x);
long double	truncl(long double _x);

#endif /* _MATH_H_ */
#else /* __BERKELEY_UPC_FIRST_PREPROCESS__ */
/* Thinnest possible wrapper... */

#if __APPLE_CC__ > 1 && __APPLE_CC__ <= 5341 && !__DARWIN_UNIX03 && PLATFORM_ARCH_POWERPC
  /* workaround an OSX 10.4 warning caused by a bug in /usr/include/architecture/ppc/math.h,
     which gcc hides when math.h is in a system dir, but fails to hide
     when we include it by full path in the wrapper below.
     Bug is present in Xcode 2.3 but fixed starting in 2.4
     __APPLE_CC__ > 1 is to exclude non-Apple gcc on OSX, 
     which unconditionally defines it to 1 regardless of system header xcode version
  */
  #define scalb ldexp
#endif

#ifndef _IN_UPCR_MATH_H
#define _IN_UPCR_MATH_H

      #if 1
         #include_next <math.h>
         #include_next <math.h>
      #endif

#undef _IN_UPCR_MATH_H
#elif !defined(_IN_UPCR_MATH_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_MATH_H_AGAIN

      #if 1
         #include_next <math.h>
         #include_next <math.h>
      #endif

#endif
#endif /* __BERKELEY_UPC_FIRST_PREPROCESS__ */