# AVX Mini Test

A minimal self-checking test which exercises individual AVX/AVX2 instructions:

- VADDPS (YMM)
- VMULPS (YMM)
- VXORPS (YMM)
- VMOVAPS/VMOVUPS (aligned/unaligned load+store)
- VZEROUPPER

## Build

```bash
cd tests/test-progs/avx-mini/src
make
```

This produces `../bin/x86/linux/avx-mini`.

## Run natively (optional sanity)

```bash
../bin/x86/linux/avx-mini
```

## Run with gem5 SE

```bash
build/X86/gem5.opt configs/example/se.py \
  --cmd tests/test-progs/avx-mini/bin/x86/linux/avx-mini
```

## Expected output

```
avx-mini passed.
```
