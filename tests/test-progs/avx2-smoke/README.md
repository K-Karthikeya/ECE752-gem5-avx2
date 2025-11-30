# AVX2 Smoke Test

This is a self-checking AVX2 instruction smoke test exercising:

- VADDPS (256-bit packed single-precision add)
- VMULPS (256-bit packed single-precision multiply)
- VXORPS (256-bit packed bitwise XOR)
- VMOVAPS / VMOVUPS (aligned/unaligned loads & stores)
- VZEROUPPER (upper YMM zeroing)

The program performs operations using AVX2 intrinsics and validates results.
It exits with code 0 on success, or 1/2/3 indicating which check failed.

## Build (Linux x86-64 host or cross)

Requires `gcc` with AVX2 support.

```bash
cd tests/test-progs/avx2-smoke/src
make
```

Resulting binary:
```
../bin/x86/linux/avx2-smoke
```

## Run under gem5 (SE mode)

Example (adjust path to your built gem5 binary):

```bash
build/X86/gem5.opt configs/example/se.py --cmd tests/test-progs/avx2-smoke/bin/x86/linux/avx2-smoke
```

You should see the final message:
```
AVX2 smoke test passed.
```
Exit status 0 indicates success.

## Windows Host Notes

If building on Windows, use WSL or a cross-compilation container (Docker) to produce a Linux x86-64 binary suitable for gem5 SE execution.

## Extending

You can extend this test by adding:
- Additional arithmetic (VSUBPS, VDIVPS)
- Fused operations (if implemented later)
- YMM vs XMM partial-length tests

## Troubleshooting

| Symptom | Possible Cause | Action |
|---------|----------------|--------|
| Illegal instruction | Host CPU lacks AVX2 | Build inside environment with AVX2 or rely on gem5 only |
| Wrong result lane N | Microop implementation issue | Instrument lane values via temporary prints |
| Segfault | Misaligned unaligned pointer arithmetic | Re-check offset logic in test |

---
Maintainer: Temporary test for AVX2 patch validation.
