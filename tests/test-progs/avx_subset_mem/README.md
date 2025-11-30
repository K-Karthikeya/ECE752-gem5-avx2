# AVX subset mem test

This program validates AVX reg+mem operand handling, overlap semantics, and both 256-bit (YMM) and 128-bit (XMM) paths.

It expects a SysV AMD64 environment (Linux/WSL). Build with `gcc`.

## Build

- Prereqs: `gcc`, `make`, and binutils (`objdump`) if you want `make disasm`.

```sh
make
```

This produces `avx_subset_mem_test`.

## Run

```sh
./avx_subset_mem_test
```

Expected output:
- `AVX subset mem test PASS`

## Disassemble (optional)

```sh
make disasm
```

You should see the expected AVX mnemonics (vmovups, vmovaps, vaddps, vmulps, vxorps, vzeroupper).

## Notes

- The C harness is compiled with `-mno-avx` to avoid the compiler emitting unintended AVX instructions; only the assembly file uses `-mavx`.
- If you are on Windows, build under WSL or MSYS2/MinGW with a POSIX-like toolchain. Static linking is disabled for portability.
