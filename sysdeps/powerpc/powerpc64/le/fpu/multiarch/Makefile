ifeq ($(subdir),math)

#
# Only enable ifunc _Float128 support if the baseline cpu support
# is older than power9.
ifneq (yes,$(libc-submachine-power9))
do_f128_multiarch = yes
endif

#
# This is an ugly, but contained, mechanism to provide hardware optimized
# _Float128 and ldouble == ieee128 optimized routines for P9 and beyond
# hardware.  At a very high level, we rely on ASM renames, and rarely
# macro renames to build two sets of _Float128 ABI, one with _power8 (the
# baseline powerpc64le cpu) and _power9 (the first powerpc64le cpu to introduce
# hardware support for _Float128).
#
# At a high level, we compile 3 files for each object file.
#   1.  The baseline soft-float128, unsuffixed objects $(object).$(sfx)
#       The symbols contained in these files is suffixed by _power8.
#   2.  The hard-float128, power9, suffixed objects $(object)-power9.$(sfx).
#       The symbols contained in these files is suffixed by _power9.
#   3.  The IFUNC wrapper object to export ABI, $(object)-ifunc.$(sfx)
#       This glues the above together and implements the ABI.
#
# 2 & 3 are automatically generated by Makefile rule.  Placing the exported
# ABI into a separate file allows reuse of existing aliasing macros
# with minimal hassle.
#
#
# If the float128 ABI is expanded, and a new ifunc wrappers are desired,
# the following lists how to map new symbols from the shared headers into
# their local overrides here:
#
#   float128_private.h
#
#     is used to rename the ldouble == ieee128 object files.  This takes
#     it a step further and redirects symbols to a local name.  This supports
#     nearly all files in sysdeps/ieee754/float128, but not all _Float128
#     objects.  However, this is only meant to be used internally to support
#     compilation of ldbl-128 into float128.
#
#   math-type-macros-float128.h
#
#     renames symbols which are generated via shared templated in math/.
#
#   math_private.h
#
#     provides internal declarations for common macros and functions which
#     are called from within libm.  Note, float128_private.h duplicates
#     some of these declarations as these headers are generally not included
#     in the same translation unit.
#
# The above is supported by several header files as described below:
#
#   float128-ifunc.h
#
#     provides support for generating the IFUNC objects in part 3 above.
#     This header is only included with wrapper functions.
#
#   float128-ifunc-macros.h
#
#     disables all first-order float128 aliasing macros used in libm,
#     and libm wrappers around libc-symbols.h.
#
#   float128-ifunc-redirect-macros.h
#
#     provides macros which implement the appending of the suffix to
#     symbols what have been selected.
#
#   float128-ifunc-redirects.h
#
#     provides ASM redirects for symbols which are redirected in the
#     private copy of math.h used by glibc, but not declared by math_private.h
#
#   float128-ifunc-redirects-mp.h
#
#     provides ASM redirects which are used by math_private.h (the -mp suffix)
#     and the interposer float128_private.h discussed late.
#
# Notably, this enforces a slightly different mechanism for machine specific
# overrides.  Optimizations for all targets must all be reachable from the same
# file.  See the history to fmaf128 or sqrtf128 to understand how this looks
# in practice.
#
ifeq ($(do_f128_multiarch),yes)

gen-libm-f128-ifunc-routines = \
	e_acosf128 e_acoshf128 e_asinf128 e_atan2f128 e_atanhf128 e_coshf128 \
	e_expf128 e_fmodf128 e_hypotf128 e_j0f128 e_j1f128 e_jnf128 \
	e_lgammaf128_r e_logf128 e_log10f128 e_powf128 e_remainderf128 \
	e_sinhf128 e_sqrtf128 e_gammaf128_r e_ilogbf128 k_tanf128 s_asinhf128 \
	s_atanf128 s_cbrtf128 s_ceilf128 s_cosf128 s_erff128 s_expm1f128 \
	s_fabsf128 s_floorf128 s_log1pf128 s_logbf128 \
	s_rintf128 s_scalblnf128 s_sinf128 s_tanf128 \
	s_tanhf128 s_truncf128 s_remquof128 e_log2f128 \
	s_roundf128 s_nearbyintf128 s_sincosf128 s_fmaf128 s_lrintf128 \
	s_llrintf128 s_lroundf128 s_llroundf128 e_exp10f128 \
	m_modff128 m_scalbnf128 m_frexpf128 m_ldexpf128 x2y2m1f128 \
	gamma_productf128 lgamma_negf128 lgamma_productf128 s_roundevenf128 \
	cargf128 conjf128 cimagf128 crealf128 cabsf128 e_scalbf128 s_cacosf128 \
	s_cacoshf128 s_ccosf128 s_ccoshf128 s_casinf128 s_csinf128 \
	s_casinhf128 k_casinhf128 s_csinhf128 k_casinhf128 s_csinhf128 \
	s_catanhf128 s_catanf128 s_ctanf128 s_ctanhf128 s_cexpf128 s_clogf128 \
	s_cprojf128 s_csqrtf128 s_cpowf128 s_clog10f128 s_fdimf128 \
	s_fmaxf128 s_fminf128 w_ilogbf128 w_llogbf128 \
	w_log1pf128 w_scalblnf128 w_acosf128 \
	w_acoshf128 w_asinf128 w_atan2f128 w_atanhf128 w_coshf128 w_exp10f128 \
	w_exp2f128 w_fmodf128 w_hypotf128 w_j0f128 w_j1f128 w_jnf128 \
	w_logf128 w_log10f128 w_log2f128 w_powf128 w_remainderf128 \
	w_scalbf128 w_sinhf128 w_sqrtf128 w_tgammaf128 w_lgammaf128 \
	w_lgammaf128_r w_expf128 e_exp2f128 \
	k_sinf128 k_cosf128 k_sincosf128 e_rem_pio2f128


f128-march-routines-p9 = $(addsuffix -power9,$(gen-libm-f128-ifunc-routines))
f128-march-routines-ifunc = $(addsuffix -ifunc,$(gen-libm-f128-ifunc-routines))
f128-march-routines = $(f128-march-routines-p9) $(f128-march-routines-ifunc)
f128-march-cpus = power9

libm-routines += $(f128-march-routines)
generated += $(f128-march-routines)

CFLAGS-float128-ifunc.c += $(type-float128-CFLAGS) $(no-gnu-attribute-CFLAGS)

# Copy special CFLAGS for some functions
CFLAGS-m_modff128-power9.c += -fsignaling-nans

# Generate ifunc wrapper files and target specific wrappers around
# each routine above.  Note, m_%.c files are fixed up to include
# s_%.c files.  This is an artifact of the makefile rules which allow
# some files to be compiled for libc and libm.
$(objpfx)gen-float128-ifuncs.stmp: Makefile
	$(make-target-directory)
	for gcall in $(gen-libm-f128-ifunc-routines); do \
	  ifile="$${gcall}";                             \
	  if [ $${gcall##m_} != $${gcall} ]; then        \
	    ifile="s_$${gcall##m_}";                     \
	  fi;                                            \
	  for cpu in $(f128-march-cpus); do              \
	    file=$(objpfx)$${gcall}-$${cpu}.c;           \
	    {                                            \
	      echo "#include <$${ifile}.c>";             \
	    } > $${file};                                \
	  done;                                          \
	  name="$${gcall##?_}";                          \
	  pfx="$${gcall%%_*}";                           \
	  R="";                                          \
	  r="";                                          \
	  if [ $${gcall##m_} != $${gcall} ]; then        \
	    pfx="s";                                     \
	  fi;                                            \
	  if [ $${#pfx} != 1 ]; then                     \
	    pfx="";                                      \
	  else                                           \
	    pfx="_$${pfx}";                              \
	  fi;                                            \
	  if [ $${name%%_r} != $${name} ]; then          \
	    R="_R";                                      \
	    r="_r";                                      \
	    name="$${name%%_r}";                         \
	  fi;                                            \
	  name="$${name%%f128}";                         \
	  decl="DECL_ALIAS$${pfx}_$${name}$${r}";        \
	  compat="GEN_COMPAT$${pfx}_$${name}$${r}";      \
	  declc="DECL_ALIAS$${R}$${pfx}";                \
	  {                                              \
	    echo "#include <float128-ifunc.h>";          \
	    echo "#ifndef $${decl}";                     \
	    echo "# define $${decl}(f) $${declc} (f)";   \
	    echo "#endif";                               \
	    echo "#ifndef $${compat}";                   \
	    echo "# define $${compat}(f)";               \
	    echo "#endif";                               \
	    echo "$${decl} ($${name});";                 \
	    echo "$${compat} ($${name});";               \
	  } > $(objpfx)$${gcall}-ifunc.c;                \
	done;                                            \
	echo > $(@)

$(foreach f,$(f128-march-routines),$(objpfx)$(f).c): $(objpfx)gen-float128-ifuncs.stmp

enable-f128-ifunc-CFLAGS = -D_F128_ENABLE_IFUNC $(no-gnu-attributes-CFLAGS) $(type-float128-CFLAGS)

# Enable IFUNC on baseline (power8) implementations
include $(o-iterator)
define o-iterator-doit
$(foreach f,$(gen-libm-f128-ifunc-routines),$(objpfx)$(f)$(o)): sysdep-CFLAGS += -D_F128_ENABLE_IFUNC
endef
object-suffixes-left := $(all-object-suffixes)
include $(o-iterator)

# Likewise, but for power9.
include $(o-iterator)
define o-iterator-doit
$(foreach f,$(f128-march-routines-p9),$(objpfx)$(f)$(o)): sysdep-CFLAGS += $$(enable-f128-ifunc-CFLAGS) -mcpu=power9
endef
object-suffixes-left := $(all-object-suffixes)
include $(o-iterator)

endif # do_f128_multiarch
endif
