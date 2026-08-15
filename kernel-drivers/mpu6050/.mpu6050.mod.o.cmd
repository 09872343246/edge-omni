savedcmd_mpu6050.mod.o := gcc -Wp,-MMD,./.mpu6050.mod.o.d -nostdinc -I/usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include -I/usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated -I/usr/src/linux-headers-6.18.35-current-sunxi/include -I/usr/src/linux-headers-6.18.35-current-sunxi/include -I/usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi -I/usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/uapi -I/usr/src/linux-headers-6.18.35-current-sunxi/include/uapi -I/usr/src/linux-headers-6.18.35-current-sunxi/include/generated/uapi -include /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/compiler-version.h -include /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kconfig.h -include /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/compiler_types.h -D__KERNEL__ -mlittle-endian -D__LINUX_ARM_ARCH__=7 -std=gnu11 -fshort-wchar -funsigned-char -fno-common -fno-PIE -fno-strict-aliasing -fno-dwarf2-cfi-asm -mno-fdpic -fno-ipa-sra -mtp=cp15 -mabi=aapcs-linux -mfpu=vfp -funwind-tables -Wa,-mimplicit-it=always -Wa,-mno-warn-deprecated -mthumb -march=armv7-a -msoft-float -Uarm -fno-delete-null-pointer-checks -O2 -fno-allow-store-data-races -fstack-protector-strong -ftrivial-auto-var-init=zero -fno-stack-clash-protection -pg -fno-strict-overflow -fno-stack-check -fconserve-stack -fno-builtin-wcslen -Wall -Wextra -Wundef -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Werror=strict-prototypes -Wno-format-security -Wno-trigraphs -Wno-frame-address -Wno-address-of-packed-member -Wmissing-declarations -Wmissing-prototypes -Wframe-larger-than=1280 -Wno-main -Wno-dangling-pointer -Wvla-larger-than=1 -Wno-pointer-sign -Wcast-function-type -Wno-array-bounds -Wno-stringop-overflow -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init -Wenum-conversion -Wunused -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-packed-not-aligned -Wno-format-overflow -Wno-format-truncation -Wno-stringop-truncation -Wno-override-init -Wno-missing-field-initializers -Wno-type-limits -Wno-shift-negative-value -Wno-maybe-uninitialized -Wno-sign-compare -Wno-unused-parameter -g -gdwarf-5 -mstack-protector-guard=tls -mstack-protector-guard-offset=1464  -DMODULE  -DKBUILD_BASENAME='"mpu6050.mod"' -DKBUILD_MODNAME='"mpu6050"' -D__KBUILD_MODNAME=kmod_mpu6050 -c -o mpu6050.mod.o mpu6050.mod.c  

source_mpu6050.mod.o := mpu6050.mod.c

deps_mpu6050.mod.o := \
    $(wildcard include/config/MODULE_UNLOAD) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/compiler-version.h \
    $(wildcard include/config/CC_VERSION_TEXT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kconfig.h \
    $(wildcard include/config/CPU_BIG_ENDIAN) \
    $(wildcard include/config/BOOGER) \
    $(wildcard include/config/FOO) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/compiler_types.h \
    $(wildcard include/config/DEBUG_INFO_BTF) \
    $(wildcard include/config/PAHOLE_HAS_BTF_TAG) \
    $(wildcard include/config/FUNCTION_ALIGNMENT) \
    $(wildcard include/config/CC_HAS_SANE_FUNCTION_ALIGNMENT) \
    $(wildcard include/config/X86_64) \
    $(wildcard include/config/ARM64) \
    $(wildcard include/config/LD_DEAD_CODE_DATA_ELIMINATION) \
    $(wildcard include/config/LTO_CLANG) \
    $(wildcard include/config/HAVE_ARCH_COMPILER_H) \
    $(wildcard include/config/KCSAN) \
    $(wildcard include/config/CC_HAS_ASSUME) \
    $(wildcard include/config/CC_HAS_COUNTED_BY) \
    $(wildcard include/config/CC_HAS_MULTIDIMENSIONAL_NONSTRING) \
    $(wildcard include/config/UBSAN_INTEGER_WRAP) \
    $(wildcard include/config/CFI) \
    $(wildcard include/config/ARCH_USES_CFI_GENERIC_LLVM_PASS) \
    $(wildcard include/config/CC_HAS_ASM_INLINE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/compiler_attributes.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/compiler-gcc.h \
    $(wildcard include/config/ARCH_USE_BUILTIN_BSWAP) \
    $(wildcard include/config/SHADOW_CALL_STACK) \
    $(wildcard include/config/KCOV) \
    $(wildcard include/config/CC_HAS_TYPEOF_UNQUAL) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/module.h \
    $(wildcard include/config/MODULES) \
    $(wildcard include/config/SYSFS) \
    $(wildcard include/config/MODULES_TREE_LOOKUP) \
    $(wildcard include/config/LIVEPATCH) \
    $(wildcard include/config/STACKTRACE_BUILD_ID) \
    $(wildcard include/config/ARCH_USES_CFI_TRAPS) \
    $(wildcard include/config/MODULE_SIG) \
    $(wildcard include/config/GENERIC_BUG) \
    $(wildcard include/config/KALLSYMS) \
    $(wildcard include/config/SMP) \
    $(wildcard include/config/TRACEPOINTS) \
    $(wildcard include/config/TREE_SRCU) \
    $(wildcard include/config/BPF_EVENTS) \
    $(wildcard include/config/DEBUG_INFO_BTF_MODULES) \
    $(wildcard include/config/JUMP_LABEL) \
    $(wildcard include/config/TRACING) \
    $(wildcard include/config/EVENT_TRACING) \
    $(wildcard include/config/DYNAMIC_FTRACE) \
    $(wildcard include/config/KPROBES) \
    $(wildcard include/config/HAVE_STATIC_CALL_INLINE) \
    $(wildcard include/config/KUNIT) \
    $(wildcard include/config/PRINTK_INDEX) \
    $(wildcard include/config/CONSTRUCTORS) \
    $(wildcard include/config/FUNCTION_ERROR_INJECTION) \
    $(wildcard include/config/DYNAMIC_DEBUG_CORE) \
    $(wildcard include/config/MITIGATION_RETPOLINE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/list.h \
    $(wildcard include/config/LIST_HARDENED) \
    $(wildcard include/config/DEBUG_LIST) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/container_of.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/build_bug.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/compiler.h \
    $(wildcard include/config/TRACE_BRANCH_PROFILING) \
    $(wildcard include/config/PROFILE_ALL_BRANCHES) \
    $(wildcard include/config/OBJTOOL) \
    $(wildcard include/config/64BIT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/asm/rwonce.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/rwonce.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kasan-checks.h \
    $(wildcard include/config/KASAN_GENERIC) \
    $(wildcard include/config/KASAN_SW_TAGS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/types.h \
    $(wildcard include/config/HAVE_UID16) \
    $(wildcard include/config/UID16) \
    $(wildcard include/config/ARCH_DMA_ADDR_T_64BIT) \
    $(wildcard include/config/PHYS_ADDR_T_64BIT) \
    $(wildcard include/config/ARCH_32BIT_USTAT_F_TINODE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/int-ll64.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/int-ll64.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/uapi/asm/bitsperlong.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitsperlong.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/bitsperlong.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/posix_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/stddef.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/stddef.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/posix_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/posix_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kcsan-checks.h \
    $(wildcard include/config/KCSAN_WEAK_MEMORY) \
    $(wildcard include/config/KCSAN_IGNORE_ATOMICS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/poison.h \
    $(wildcard include/config/ILLEGAL_POINTER_VALUE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/const.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/const.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/const.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/barrier.h \
    $(wildcard include/config/CPU_32v6K) \
    $(wildcard include/config/THUMB2_KERNEL) \
    $(wildcard include/config/CPU_XSC3) \
    $(wildcard include/config/CPU_FA526) \
    $(wildcard include/config/ARM_HEAVY_MB) \
    $(wildcard include/config/ARM_DMA_MEM_BUFFERABLE) \
    $(wildcard include/config/CPU_SPECTRE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/barrier.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/stat.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/stat.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/stat.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/time.h \
    $(wildcard include/config/POSIX_TIMERS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/cache.h \
    $(wildcard include/config/ARCH_HAS_CACHE_LINE_SIZE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/kernel.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/sysinfo.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/cache.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/cache.h \
    $(wildcard include/config/ARM_L1_CACHE_SHIFT) \
    $(wildcard include/config/AEABI) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/math64.h \
    $(wildcard include/config/ARCH_SUPPORTS_INT128) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/math.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/div64.h \
    $(wildcard include/config/CC_OPTIMIZE_FOR_PERFORMANCE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/compiler.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/div64.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/log2.h \
    $(wildcard include/config/ARCH_HAS_ILOG2_U32) \
    $(wildcard include/config/ARCH_HAS_ILOG2_U64) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/bitops.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/bits.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/bits.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/bits.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/overflow.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/limits.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/limits.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/limits.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/typecheck.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/generic-non-atomic.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/bitops.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/irqflags.h \
    $(wildcard include/config/PROVE_LOCKING) \
    $(wildcard include/config/TRACE_IRQFLAGS) \
    $(wildcard include/config/PREEMPT_RT) \
    $(wildcard include/config/IRQSOFF_TRACER) \
    $(wildcard include/config/PREEMPT_TRACER) \
    $(wildcard include/config/DEBUG_IRQFLAGS) \
    $(wildcard include/config/TRACE_IRQFLAGS_SUPPORT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/irqflags_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/cleanup.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/err.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/uapi/asm/errno.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/errno.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/errno-base.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/args.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/irqflags.h \
    $(wildcard include/config/CPU_V7M) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/ARM_THUMB) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/ptrace.h \
    $(wildcard include/config/CPU_ENDIAN_BE8) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/hwcap.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/hwcap.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/bitfield.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/byteorder.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/byteorder/little_endian.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/byteorder/little_endian.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/swab.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/swab.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/swab.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/swab.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/byteorder/generic.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/irqflags.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/percpu.h \
    $(wildcard include/config/CPU_V6) \
    $(wildcard include/config/ARM_HAS_GROUP_RELOCS) \
    $(wildcard include/config/ARM_MODULE_PLTS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/insn.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/percpu.h \
    $(wildcard include/config/DEBUG_PREEMPT) \
    $(wildcard include/config/HAVE_SETUP_PER_CPU_AREA) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/threads.h \
    $(wildcard include/config/NR_CPUS) \
    $(wildcard include/config/BASE_SMALL) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/percpu-defs.h \
    $(wildcard include/config/ARCH_MODULE_NEEDS_WEAK_PER_CPU) \
    $(wildcard include/config/DEBUG_FORCE_WEAK_PER_CPU) \
    $(wildcard include/config/AMD_MEM_ENCRYPT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/non-atomic.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/non-instrumented-non-atomic.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/builtin-__fls.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/builtin-__ffs.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/builtin-fls.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/builtin-ffs.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/ffz.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/fls64.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/sched.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/hweight.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/arch_hweight.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/const_hweight.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/lock.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/atomic.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/atomic.h \
    $(wildcard include/config/GENERIC_ATOMIC64) \
    $(wildcard include/config/ARM_LPAE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/prefetch.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/processor.h \
    $(wildcard include/config/HAVE_HW_BREAKPOINT) \
    $(wildcard include/config/BINFMT_ELF_FDPIC) \
    $(wildcard include/config/MMU) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/hw_breakpoint.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/unified.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/vdso/processor.h \
    $(wildcard include/config/ARM_ERRATA_754327) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/cmpxchg.h \
    $(wildcard include/config/CPU_SA1100) \
    $(wildcard include/config/CPU_SA110) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/cmpxchg-emu.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/cmpxchg-local.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/atomic/atomic-arch-fallback.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/atomic/atomic-long.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/atomic/atomic-instrumented.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/instrumented.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kmsan-checks.h \
    $(wildcard include/config/KMSAN) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/instrumented-lock.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/le.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bitops/ext2-atomic-setbit.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/math64.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/time64.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/time64.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/time.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/time_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/time32.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/timex.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/timex.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/param.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/uapi/asm/param.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/param.h \
    $(wildcard include/config/HZ) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/param.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/timex.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/time32.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/time.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/uidgid.h \
    $(wildcard include/config/MULTIUSER) \
    $(wildcard include/config/USER_NS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/uidgid_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/highuid.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/buildid.h \
    $(wildcard include/config/VMCORE_INFO) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kmod.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/umh.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/gfp.h \
    $(wildcard include/config/HIGHMEM) \
    $(wildcard include/config/ZONE_DMA) \
    $(wildcard include/config/ZONE_DMA32) \
    $(wildcard include/config/ZONE_DEVICE) \
    $(wildcard include/config/NUMA) \
    $(wildcard include/config/COMPACTION) \
    $(wildcard include/config/CONTIG_ALLOC) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/gfp_types.h \
    $(wildcard include/config/KASAN_HW_TAGS) \
    $(wildcard include/config/LOCKDEP) \
    $(wildcard include/config/SLAB_OBJ_EXT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/mmzone.h \
    $(wildcard include/config/ARCH_FORCE_MAX_ORDER) \
    $(wildcard include/config/PAGE_BLOCK_MAX_ORDER) \
    $(wildcard include/config/CMA) \
    $(wildcard include/config/MEMORY_ISOLATION) \
    $(wildcard include/config/ZSMALLOC) \
    $(wildcard include/config/UNACCEPTED_MEMORY) \
    $(wildcard include/config/IOMMU_SUPPORT) \
    $(wildcard include/config/SWAP) \
    $(wildcard include/config/NUMA_BALANCING) \
    $(wildcard include/config/HUGETLB_PAGE) \
    $(wildcard include/config/TRANSPARENT_HUGEPAGE) \
    $(wildcard include/config/LRU_GEN) \
    $(wildcard include/config/LRU_GEN_STATS) \
    $(wildcard include/config/LRU_GEN_WALKS_MMU) \
    $(wildcard include/config/MEMCG) \
    $(wildcard include/config/SPARSEMEM) \
    $(wildcard include/config/MEMORY_HOTPLUG) \
    $(wildcard include/config/MEMORY_FAILURE) \
    $(wildcard include/config/FLATMEM) \
    $(wildcard include/config/PAGE_EXTENSION) \
    $(wildcard include/config/DEFERRED_STRUCT_PAGE_INIT) \
    $(wildcard include/config/HAVE_MEMORYLESS_NODES) \
    $(wildcard include/config/SPARSEMEM_VMEMMAP) \
    $(wildcard include/config/SPARSEMEM_EXTREME) \
    $(wildcard include/config/SPARSEMEM_VMEMMAP_PREINIT) \
    $(wildcard include/config/HAVE_ARCH_PFN_VALID) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/spinlock.h \
    $(wildcard include/config/DEBUG_SPINLOCK) \
    $(wildcard include/config/PREEMPTION) \
    $(wildcard include/config/DEBUG_LOCK_ALLOC) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/preempt.h \
    $(wildcard include/config/PREEMPT_COUNT) \
    $(wildcard include/config/TRACE_PREEMPT_TOGGLE) \
    $(wildcard include/config/PREEMPT_NOTIFIERS) \
    $(wildcard include/config/PREEMPT_DYNAMIC) \
    $(wildcard include/config/PREEMPT_NONE) \
    $(wildcard include/config/PREEMPT_VOLUNTARY) \
    $(wildcard include/config/PREEMPT) \
    $(wildcard include/config/PREEMPT_LAZY) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/linkage.h \
    $(wildcard include/config/ARCH_USE_SYM_ANNOTATIONS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/stringify.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/export.h \
    $(wildcard include/config/MODVERSIONS) \
    $(wildcard include/config/GENDWARFKSYMS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/linkage.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/asm/preempt.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/preempt.h \
    $(wildcard include/config/HAVE_PREEMPT_DYNAMIC_KEY) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/thread_info.h \
    $(wildcard include/config/THREAD_INFO_IN_TASK) \
    $(wildcard include/config/GENERIC_ENTRY) \
    $(wildcard include/config/ARCH_HAS_PREEMPT_LAZY) \
    $(wildcard include/config/HAVE_ARCH_WITHIN_STACK_FRAMES) \
    $(wildcard include/config/SH) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/bug.h \
    $(wildcard include/config/PRINTK) \
    $(wildcard include/config/BUG_ON_DATA_CORRUPTION) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/bug.h \
    $(wildcard include/config/DEBUG_BUGVERBOSE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/opcodes.h \
    $(wildcard include/config/CPU_ENDIAN_BE32) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/bug.h \
    $(wildcard include/config/BUG) \
    $(wildcard include/config/GENERIC_BUG_RELATIVE_POINTERS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/instrumentation.h \
    $(wildcard include/config/NOINSTR_VALIDATION) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/once_lite.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/panic.h \
    $(wildcard include/config/PANIC_TIMEOUT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/stdarg.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/printk.h \
    $(wildcard include/config/MESSAGE_LOGLEVEL_DEFAULT) \
    $(wildcard include/config/CONSOLE_LOGLEVEL_DEFAULT) \
    $(wildcard include/config/CONSOLE_LOGLEVEL_QUIET) \
    $(wildcard include/config/EARLY_PRINTK) \
    $(wildcard include/config/DYNAMIC_DEBUG) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/init.h \
    $(wildcard include/config/HAVE_ARCH_PREL32_RELOCATIONS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kern_levels.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/ratelimit_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/spinlock_types_raw.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/spinlock_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/lockdep_types.h \
    $(wildcard include/config/PROVE_RAW_LOCK_NESTING) \
    $(wildcard include/config/LOCK_STAT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/dynamic_debug.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/jump_label.h \
    $(wildcard include/config/HAVE_ARCH_JUMP_LABEL_RELATIVE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/jump_label.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/restart_block.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/errno.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/errno.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/current.h \
    $(wildcard include/config/CURRENT_POINTER_IN_TPIDRURO) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/KASAN) \
    $(wildcard include/config/VMAP_STACK) \
    $(wildcard include/config/ARM_THUMBEE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/VFPv3) \
    $(wildcard include/config/IWMMXT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/page.h \
    $(wildcard include/config/CPU_COPY_V4WT) \
    $(wildcard include/config/CPU_COPY_V4WB) \
    $(wildcard include/config/CPU_COPY_FEROCEON) \
    $(wildcard include/config/CPU_COPY_FA) \
    $(wildcard include/config/CPU_XSCALE) \
    $(wildcard include/config/CPU_COPY_V6) \
    $(wildcard include/config/KUSER_HELPERS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/page.h \
    $(wildcard include/config/PAGE_SHIFT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/glue.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/pgtable-2level-types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/memory.h \
    $(wildcard include/config/NEED_MACH_MEMORY_H) \
    $(wildcard include/config/PAGE_OFFSET) \
    $(wildcard include/config/DRAM_BASE) \
    $(wildcard include/config/DRAM_SIZE) \
    $(wildcard include/config/XIP_KERNEL) \
    $(wildcard include/config/HAVE_TCM) \
    $(wildcard include/config/ARM_PATCH_PHYS_VIRT) \
    $(wildcard include/config/PHYS_OFFSET) \
    $(wildcard include/config/DEBUG_VIRTUAL) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sizes.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/kasan_def.h \
    $(wildcard include/config/KASAN_SHADOW_OFFSET) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/getorder.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/memory_model.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/pfn.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/traps.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/bottom_half.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/instruction_pointer.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/lockdep.h \
    $(wildcard include/config/DEBUG_LOCKING_API_SELFTESTS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/smp.h \
    $(wildcard include/config/UP_LATE_INIT) \
    $(wildcard include/config/CSD_LOCK_WAIT_DEBUG) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/cpumask.h \
    $(wildcard include/config/FORCE_NR_CPUS) \
    $(wildcard include/config/HOTPLUG_CPU) \
    $(wildcard include/config/DEBUG_PER_CPU_MAPS) \
    $(wildcard include/config/CPUMASK_OFFSTACK) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kernel.h \
    $(wildcard include/config/PREEMPT_VOLUNTARY_BUILD) \
    $(wildcard include/config/HAVE_PREEMPT_DYNAMIC_CALL) \
    $(wildcard include/config/PREEMPT_) \
    $(wildcard include/config/DEBUG_ATOMIC_SLEEP) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/align.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/align.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/array_size.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/hex.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kstrtox.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/minmax.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sprintf.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/static_call_types.h \
    $(wildcard include/config/HAVE_STATIC_CALL) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/trace_printk.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/util_macros.h \
    $(wildcard include/config/FOO_SUSPEND) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/wordpart.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/bitmap.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/find.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/string.h \
    $(wildcard include/config/BINARY_PRINTF) \
    $(wildcard include/config/FORTIFY_SOURCE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/string.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/string.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/fortify-string.h \
    $(wildcard include/config/CC_HAS_KASAN_MEMINTRINSIC_PREFIX) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/bitmap-str.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/cpumask_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/numa.h \
    $(wildcard include/config/NUMA_KEEP_MEMINFO) \
    $(wildcard include/config/HAVE_ARCH_NODE_DEV_GROUP) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/nodemask.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/nodemask_types.h \
    $(wildcard include/config/NODES_SHIFT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/random.h \
    $(wildcard include/config/VMGENID) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/random.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/ioctl.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/uapi/asm/ioctl.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/ioctl.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/ioctl.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/irqnr.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/irqnr.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/smp_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/llist.h \
    $(wildcard include/config/ARCH_HAVE_NMI_SAFE_CMPXCHG) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/smp.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/asm/mmiowb.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/mmiowb.h \
    $(wildcard include/config/MMIOWB) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/spinlock_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rwlock_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/spinlock.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rwlock.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/spinlock_api_smp.h \
    $(wildcard include/config/INLINE_SPIN_LOCK) \
    $(wildcard include/config/INLINE_SPIN_LOCK_BH) \
    $(wildcard include/config/INLINE_SPIN_LOCK_IRQ) \
    $(wildcard include/config/INLINE_SPIN_LOCK_IRQSAVE) \
    $(wildcard include/config/INLINE_SPIN_TRYLOCK) \
    $(wildcard include/config/INLINE_SPIN_TRYLOCK_BH) \
    $(wildcard include/config/UNINLINE_SPIN_UNLOCK) \
    $(wildcard include/config/INLINE_SPIN_UNLOCK_BH) \
    $(wildcard include/config/INLINE_SPIN_UNLOCK_IRQ) \
    $(wildcard include/config/INLINE_SPIN_UNLOCK_IRQRESTORE) \
    $(wildcard include/config/GENERIC_LOCKBREAK) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rwlock_api_smp.h \
    $(wildcard include/config/INLINE_READ_LOCK) \
    $(wildcard include/config/INLINE_WRITE_LOCK) \
    $(wildcard include/config/INLINE_READ_LOCK_BH) \
    $(wildcard include/config/INLINE_WRITE_LOCK_BH) \
    $(wildcard include/config/INLINE_READ_LOCK_IRQ) \
    $(wildcard include/config/INLINE_WRITE_LOCK_IRQ) \
    $(wildcard include/config/INLINE_READ_LOCK_IRQSAVE) \
    $(wildcard include/config/INLINE_WRITE_LOCK_IRQSAVE) \
    $(wildcard include/config/INLINE_READ_TRYLOCK) \
    $(wildcard include/config/INLINE_WRITE_TRYLOCK) \
    $(wildcard include/config/INLINE_READ_UNLOCK) \
    $(wildcard include/config/INLINE_WRITE_UNLOCK) \
    $(wildcard include/config/INLINE_READ_UNLOCK_BH) \
    $(wildcard include/config/INLINE_WRITE_UNLOCK_BH) \
    $(wildcard include/config/INLINE_READ_UNLOCK_IRQ) \
    $(wildcard include/config/INLINE_WRITE_UNLOCK_IRQ) \
    $(wildcard include/config/INLINE_READ_UNLOCK_IRQRESTORE) \
    $(wildcard include/config/INLINE_WRITE_UNLOCK_IRQRESTORE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/list_nulls.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/wait.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/seqlock.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/mutex.h \
    $(wildcard include/config/DEBUG_MUTEXES) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/osq_lock.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/debug_locks.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/mutex_types.h \
    $(wildcard include/config/MUTEX_SPIN_ON_OWNER) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/seqlock_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/pageblock-flags.h \
    $(wildcard include/config/HUGETLB_PAGE_SIZE_VARIABLE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/page-flags-layout.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/generated/bounds.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/sparsemem.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/mm_types.h \
    $(wildcard include/config/HAVE_ALIGNED_STRUCT_PAGE) \
    $(wildcard include/config/HUGETLB_PMD_PAGE_TABLE_SHARING) \
    $(wildcard include/config/SLAB_FREELIST_HARDENED) \
    $(wildcard include/config/USERFAULTFD) \
    $(wildcard include/config/ANON_VMA_NAME) \
    $(wildcard include/config/PER_VMA_LOCK) \
    $(wildcard include/config/SCHED_MM_CID) \
    $(wildcard include/config/HAVE_ARCH_COMPAT_MMAP_BASES) \
    $(wildcard include/config/MEMBARRIER) \
    $(wildcard include/config/FUTEX_PRIVATE_HASH) \
    $(wildcard include/config/ARCH_HAS_ELF_CORE_EFLAGS) \
    $(wildcard include/config/AIO) \
    $(wildcard include/config/MMU_NOTIFIER) \
    $(wildcard include/config/SPLIT_PMD_PTLOCKS) \
    $(wildcard include/config/ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH) \
    $(wildcard include/config/IOMMU_MM_DATA) \
    $(wildcard include/config/KSM) \
    $(wildcard include/config/MM_ID) \
    $(wildcard include/config/CORE_DUMP_DEFAULT_ELF_HEADERS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/mm_types_task.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/auxvec.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/auxvec.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/auxvec.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/auxvec.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kref.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/refcount.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/refcount_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rbtree.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rbtree_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rcupdate.h \
    $(wildcard include/config/PREEMPT_RCU) \
    $(wildcard include/config/TINY_RCU) \
    $(wildcard include/config/RCU_STRICT_GRACE_PERIOD) \
    $(wildcard include/config/RCU_LAZY) \
    $(wildcard include/config/RCU_STALL_COMMON) \
    $(wildcard include/config/NO_HZ_FULL) \
    $(wildcard include/config/VIRT_XFER_TO_GUEST_WORK) \
    $(wildcard include/config/RCU_NOCB_CPU) \
    $(wildcard include/config/TASKS_RCU_GENERIC) \
    $(wildcard include/config/TASKS_RCU) \
    $(wildcard include/config/TASKS_TRACE_RCU) \
    $(wildcard include/config/TASKS_RUDE_RCU) \
    $(wildcard include/config/TREE_RCU) \
    $(wildcard include/config/DEBUG_OBJECTS_RCU_HEAD) \
    $(wildcard include/config/PROVE_RCU) \
    $(wildcard include/config/ARCH_WEAK_RELEASE_ACQUIRE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sched.h \
    $(wildcard include/config/VIRT_CPU_ACCOUNTING_NATIVE) \
    $(wildcard include/config/SCHED_INFO) \
    $(wildcard include/config/SCHEDSTATS) \
    $(wildcard include/config/SCHED_CORE) \
    $(wildcard include/config/FAIR_GROUP_SCHED) \
    $(wildcard include/config/RT_GROUP_SCHED) \
    $(wildcard include/config/RT_MUTEXES) \
    $(wildcard include/config/UCLAMP_TASK) \
    $(wildcard include/config/UCLAMP_BUCKETS_COUNT) \
    $(wildcard include/config/KMAP_LOCAL) \
    $(wildcard include/config/MEM_ALLOC_PROFILING) \
    $(wildcard include/config/SCHED_CLASS_EXT) \
    $(wildcard include/config/CGROUP_SCHED) \
    $(wildcard include/config/CFS_BANDWIDTH) \
    $(wildcard include/config/BLK_DEV_IO_TRACE) \
    $(wildcard include/config/MEMCG_V1) \
    $(wildcard include/config/COMPAT_BRK) \
    $(wildcard include/config/CGROUPS) \
    $(wildcard include/config/BLK_CGROUP) \
    $(wildcard include/config/PSI) \
    $(wildcard include/config/PAGE_OWNER) \
    $(wildcard include/config/EVENTFD) \
    $(wildcard include/config/ARCH_HAS_CPU_PASID) \
    $(wildcard include/config/X86_BUS_LOCK_DETECT) \
    $(wildcard include/config/TASK_DELAY_ACCT) \
    $(wildcard include/config/STACKPROTECTOR) \
    $(wildcard include/config/ARCH_HAS_SCALED_CPUTIME) \
    $(wildcard include/config/VIRT_CPU_ACCOUNTING_GEN) \
    $(wildcard include/config/POSIX_CPUTIMERS) \
    $(wildcard include/config/POSIX_CPU_TIMERS_TASK_WORK) \
    $(wildcard include/config/KEYS) \
    $(wildcard include/config/SYSVIPC) \
    $(wildcard include/config/DETECT_HUNG_TASK) \
    $(wildcard include/config/IO_URING) \
    $(wildcard include/config/AUDIT) \
    $(wildcard include/config/AUDITSYSCALL) \
    $(wildcard include/config/DETECT_HUNG_TASK_BLOCKER) \
    $(wildcard include/config/UBSAN) \
    $(wildcard include/config/UBSAN_TRAP) \
    $(wildcard include/config/TASK_XACCT) \
    $(wildcard include/config/CPUSETS) \
    $(wildcard include/config/X86_CPU_RESCTRL) \
    $(wildcard include/config/FUTEX) \
    $(wildcard include/config/COMPAT) \
    $(wildcard include/config/PERF_EVENTS) \
    $(wildcard include/config/RSEQ) \
    $(wildcard include/config/DEBUG_RSEQ) \
    $(wildcard include/config/FAULT_INJECTION) \
    $(wildcard include/config/LATENCYTOP) \
    $(wildcard include/config/FUNCTION_GRAPH_TRACER) \
    $(wildcard include/config/UPROBES) \
    $(wildcard include/config/BCACHE) \
    $(wildcard include/config/SECURITY) \
    $(wildcard include/config/BPF_SYSCALL) \
    $(wildcard include/config/KSTACK_ERASE) \
    $(wildcard include/config/KSTACK_ERASE_METRICS) \
    $(wildcard include/config/RANDOMIZE_KSTACK_OFFSET) \
    $(wildcard include/config/X86_MCE) \
    $(wildcard include/config/KRETPROBES) \
    $(wildcard include/config/RETHOOK) \
    $(wildcard include/config/ARCH_HAS_PARANOID_L1D_FLUSH) \
    $(wildcard include/config/RV) \
    $(wildcard include/config/RV_PER_TASK_MONITORS) \
    $(wildcard include/config/USER_EVENTS) \
    $(wildcard include/config/UNWIND_USER) \
    $(wildcard include/config/SCHED_PROXY_EXEC) \
    $(wildcard include/config/MEM_ALLOC_PROFILING_DEBUG) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/sched.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/pid_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sem_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/shm.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/shmparam.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kmsan_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/plist_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/hrtimer_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/timerqueue_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/timer_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/seccomp_types.h \
    $(wildcard include/config/SECCOMP) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/resource.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/resource.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/uapi/asm/resource.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/resource.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/resource.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/latencytop.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sched/prio.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sched/types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/signal_types.h \
    $(wildcard include/config/OLD_SIGACTION) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/signal.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/signal.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/signal.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/signal-defs.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/uapi/asm/sigcontext.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/uapi/asm/siginfo.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/asm-generic/siginfo.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/syscall_user_dispatch_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/netdevice_xmit.h \
    $(wildcard include/config/NET_EGRESS) \
    $(wildcard include/config/NET_ACT_MIRRED) \
    $(wildcard include/config/NF_DUP_NETDEV) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/task_io_accounting.h \
    $(wildcard include/config/TASK_IO_ACCOUNTING) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/posix-timers_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/rseq.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kcsan.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rv.h \
    $(wildcard include/config/RV_LTL_MONITOR) \
    $(wildcard include/config/RV_REACTORS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/tracepoint-defs.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/static_key.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/unwind_deferred_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/generated/asm/kmap_size.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/kmap_size.h \
    $(wildcard include/config/DEBUG_KMAP_LOCAL) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/generated/rq-offsets.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sched/ext.h \
    $(wildcard include/config/EXT_GROUP_SCHED) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/context_tracking_irq.h \
    $(wildcard include/config/CONTEXT_TRACKING_IDLE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rcutree.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/maple_tree.h \
    $(wildcard include/config/MAPLE_RCU_DISABLED) \
    $(wildcard include/config/DEBUG_MAPLE_TREE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rwsem.h \
    $(wildcard include/config/RWSEM_SPIN_ON_OWNER) \
    $(wildcard include/config/DEBUG_RWSEMS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/completion.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/swait.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/uprobes.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/timer.h \
    $(wildcard include/config/DEBUG_OBJECTS_TIMERS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/ktime.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/jiffies.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/jiffies.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/generated/timeconst.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/vdso/ktime.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/timekeeping.h \
    $(wildcard include/config/POSIX_AUX_CLOCKS) \
    $(wildcard include/config/GENERIC_CMOS_UPDATE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/clocksource_ids.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/debugobjects.h \
    $(wildcard include/config/DEBUG_OBJECTS) \
    $(wildcard include/config/DEBUG_OBJECTS_FREE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/workqueue.h \
    $(wildcard include/config/DEBUG_OBJECTS_WORK) \
    $(wildcard include/config/FREEZER) \
    $(wildcard include/config/WQ_WATCHDOG) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/alloc_tag.h \
    $(wildcard include/config/MEM_ALLOC_PROFILING_ENABLED_BY_DEFAULT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/codetag.h \
    $(wildcard include/config/CODE_TAGGING) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/workqueue_types.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/percpu_counter.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/percpu.h \
    $(wildcard include/config/RANDOM_KMALLOC_CACHES) \
    $(wildcard include/config/PAGE_SIZE_4KB) \
    $(wildcard include/config/NEED_PER_CPU_PAGE_FIRST_CHUNK) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/mmdebug.h \
    $(wildcard include/config/DEBUG_VM) \
    $(wildcard include/config/DEBUG_VM_IRQSOFF) \
    $(wildcard include/config/DEBUG_VM_PGFLAGS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/mmu.h \
    $(wildcard include/config/CPU_HAS_ASID) \
    $(wildcard include/config/VDSO) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/page-flags.h \
    $(wildcard include/config/PAGE_IDLE_FLAG) \
    $(wildcard include/config/ARCH_USES_PG_ARCH_2) \
    $(wildcard include/config/ARCH_USES_PG_ARCH_3) \
    $(wildcard include/config/MIGRATION) \
    $(wildcard include/config/HUGETLB_PAGE_OPTIMIZE_VMEMMAP) \
    $(wildcard include/config/DEBUG_KMAP_LOCAL_FORCE_MAP) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/local_lock.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/local_lock_internal.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/zswap.h \
    $(wildcard include/config/ZSWAP) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/memory_hotplug.h \
    $(wildcard include/config/ARCH_HAS_ADD_PAGES) \
    $(wildcard include/config/MEMORY_HOTREMOVE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/notifier.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/srcu.h \
    $(wildcard include/config/TINY_SRCU) \
    $(wildcard include/config/NEED_SRCU_NMI_SAFE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rcu_segcblist.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/srcutree.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rcu_node_tree.h \
    $(wildcard include/config/RCU_FANOUT) \
    $(wildcard include/config/RCU_FANOUT_LEAF) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/topology.h \
    $(wildcard include/config/USE_PERCPU_NUMA_NODE_ID) \
    $(wildcard include/config/SCHED_SMT) \
    $(wildcard include/config/GENERIC_ARCH_TOPOLOGY) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/arch_topology.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/topology.h \
    $(wildcard include/config/ARM_CPU_TOPOLOGY) \
    $(wildcard include/config/BL_SWITCHER) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/topology.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sysctl.h \
    $(wildcard include/config/SYSCTL) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/sysctl.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/elf.h \
    $(wildcard include/config/ARCH_HAVE_EXTRA_ELF_NOTES) \
    $(wildcard include/config/ARCH_USE_GNU_PROPERTY) \
    $(wildcard include/config/ARCH_HAVE_ELF_PROT) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/elf.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/user.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/elf.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/uapi/linux/elf-em.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kobject.h \
    $(wildcard include/config/UEVENT_HELPER) \
    $(wildcard include/config/DEBUG_KOBJECT_RELEASE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sysfs.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kernfs.h \
    $(wildcard include/config/KERNFS) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/idr.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/radix-tree.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/xarray.h \
    $(wildcard include/config/XARRAY_MULTI) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sched/mm.h \
    $(wildcard include/config/MMU_LAZY_TLB_REFCOUNT) \
    $(wildcard include/config/ARCH_HAS_MEMBARRIER_CALLBACKS) \
    $(wildcard include/config/ARCH_HAS_SYNC_CORE_BEFORE_USERMODE) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sync_core.h \
    $(wildcard include/config/ARCH_HAS_PREPARE_SYNC_CORE_CMD) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/sched/coredump.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/kobject_ns.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/moduleparam.h \
    $(wildcard include/config/ALPHA) \
    $(wildcard include/config/PPC64) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/rbtree_latch.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/error-injection.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/error-injection.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/module.h \
    $(wildcard include/config/ARM_UNWIND) \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/asm-generic/module.h \
    $(wildcard include/config/HAVE_MOD_ARCH_SPECIFIC) \
  /usr/src/linux-headers-6.18.35-current-sunxi/arch/arm/include/asm/unwind.h \
  /usr/src/linux-headers-6.18.35-current-sunxi/include/linux/export-internal.h \
    $(wildcard include/config/PARISC) \

mpu6050.mod.o: $(deps_mpu6050.mod.o)

$(deps_mpu6050.mod.o):
