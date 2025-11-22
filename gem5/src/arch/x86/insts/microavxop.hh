#ifndef __ARCH_X86_INSTS_MICROAVXOP_HH__
#define __ARCH_X86_INSTS_MICROAVXOP_HH__

#include <algorithm>
#include <string>
#include <sstream>
#include <cstdint>
#include <cstdio>

#include "arch/x86/insts/microop.hh"
#include "arch/x86/regs/float.hh"
#include "arch/x86/regs/int.hh"
#include "arch/x86/regs/misc.hh"
#include "base/cprintf.hh"
#include "cpu/exec_context.hh"
#include "cpu/reg_class.hh"

namespace gem5 { namespace X86ISA {

class AVXOpBase : public X86MicroopBase {

public:
  enum SrcType {
    Non = 0,
    Reg,
    RegReg,
    RegImm,
    RegRegImm,
    RegRegReg, // Three opreands.
  };

  // Ensure we see size() calls for AVX microops specifically.
  void size(size_t newSize) override {
    fprintf(stderr, "[AVXOP-SIZE] inst=%s micro=%s this=%p newSize=%zu\n",
            instMnem, mnemonic, (void*)this, newSize);
    X86MicroopBase::size(newSize);
  }

protected:
  // Register index backing arrays (per-instance). We overprovision to avoid
  // overflow for future wider vectors. These must be members so that
  // setRegIdxArrays can take member pointers to arrays (required by StaticInst).
  static constexpr int MaxSrcRegs = 32;
  static constexpr int MaxDestRegs = 32;
  RegId srcRegIdxArr[MaxSrcRegs];
  RegId destRegIdxArr[MaxDestRegs];

  const SrcType srcType;
  const RegIndex dest;
  const RegIndex src1;
  const RegIndex src2;
  const uint8_t destSize;
  const uint8_t destVL;
  const uint8_t srcSize;
  const uint8_t srcVL;
  const uint8_t imm8;
  const uint8_t ext;

  // Constructor
  AVXOpBase(ExtMachInst _machInst, const char *_mnem, const char *_instMnem,
            uint64_t _setFlags, OpClass _opClass, SrcType _srcType,
            RegId _dest, RegId _src1, RegId _src2,
            uint8_t _destSize, uint8_t _destVL, uint8_t _srcSize,
            uint8_t _srcVL, uint8_t _imm8, uint8_t _ext)
      : X86MicroopBase(_machInst, _mnem, _instMnem, _setFlags, _opClass),
        srcType(_srcType), dest(_dest.index()), src1(_src1.index()),
        src2(_src2.index()), destSize(_destSize), destVL(_destVL),
        srcSize(_srcSize), srcVL(_srcVL), imm8(_imm8), ext(_ext) {
    // Install register index arrays before any set{Src,Dest}RegIdx calls.
    setRegIdxArrays(
        reinterpret_cast<RegIdArrayPtr>(&AVXOpBase::srcRegIdxArr),
        reinterpret_cast<RegIdArrayPtr>(&AVXOpBase::destRegIdxArr));
    _numSrcRegs = 0;
    _numDestRegs = 0;
    for (auto &c : _numTypedDestRegs) { c = 0; }
    fprintf(stderr, "[AVX-CONSTRUCT] mnem=%s destVL=%u srcVL=%u dest=%u src1=%u src2=%u\n",
            _mnem, destVL, srcVL, dest, src1, src2);
    assert((destVL % sizeof(uint64_t) == 0) && "Invalid destVL.\n");
    assert((srcVL % sizeof(uint64_t) == 0) && "Invalid srcVL.\n");
  }

  inline std::string generateDisassembly(gem5::Addr pc,
                                         const loader::SymbolTable *symtab) const
  {
    std::stringstream response;
    printMnemonic(response, instMnem, mnemonic);
    printDestReg(response, 0, destSize);
    if (this->srcType == SrcType::Non) {
      return response.str();
    }
    response << ", ";
    printSrcReg(response, 0, srcSize);
    switch (this->srcType) {
    case SrcType::RegReg: {
      response << ", ";
      printSrcReg(response, 1, srcSize);
      break;
    }
    case SrcType::RegImm: {
      gem5::ccprintf(response, ", %#x", imm8);
      break;
    }
    case SrcType::RegRegImm: {
      response << ", ";
      printSrcReg(response, 1, srcSize);
      gem5::ccprintf(response, ", %#x", imm8);
      break;
    }
    case SrcType::RegRegReg: {
      response << ", ";
      printSrcReg(response, 1, srcSize);
      response << ", ";
      printSrcReg(response, 2, srcSize);
      break;
    }
    default:
      break;
    }
    return response.str();
  }

  union FloatInt {
    struct __attribute__((packed)) {
      float f1;
      float f2;
    } f;
    double d;
    struct __attribute__((packed)) {
      uint32_t i1;
      uint32_t i2;
    } ui;
    struct __attribute__((packed)) {
      int32_t i1;
      int32_t i2;
    } si;
    struct __attribute__((packed)) {
      uint8_t i1;
      uint8_t i2;
      uint8_t i3;
      uint8_t i4;
      uint8_t i5;
      uint8_t i6;
      uint8_t i7;
      uint8_t i8;
    } uc;
    struct __attribute__((packed)) {
      int8_t i1;
      int8_t i2;
      int8_t i3;
      int8_t i4;
      int8_t i5;
      int8_t i6;
      int8_t i7;
      int8_t i8;
    } sc;
    uint64_t ul;
    int64_t sl;
  };

  enum BinaryOp {
    FloatAdd,
    FloatSub,
    FloatMul,
    FloatDiv,
    // Add/Sub can ignore sign.
    IntAdd,
    IntSub,
    IntAnd,
    SIntMin,
    IntXor,
  };

  inline FloatInt calcPackedBinaryOp(FloatInt src1, FloatInt src2, BinaryOp op) const
  {
    FloatInt dest;
    if (this->srcSize == 4) {
      // 2 float.
      switch (op) {
      case BinaryOp::FloatAdd:
        dest.f.f1 = src1.f.f1 + src2.f.f1;
        dest.f.f2 = src1.f.f2 + src2.f.f2;
        break;
      case BinaryOp::FloatSub:
        dest.f.f1 = src1.f.f1 - src2.f.f1;
        dest.f.f2 = src1.f.f2 - src2.f.f2;
        break;
      case BinaryOp::FloatMul:
        dest.f.f1 = src1.f.f1 * src2.f.f1;
        dest.f.f2 = src1.f.f2 * src2.f.f2;
        break;
      case BinaryOp::FloatDiv:
        dest.f.f1 = src1.f.f1 / src2.f.f1;
        dest.f.f2 = src1.f.f2 / src2.f.f2;
        break;
      case BinaryOp::IntAdd:
        dest.si.i1 = src1.si.i1 + src2.si.i1;
        dest.si.i2 = src1.si.i2 + src2.si.i2;
        break;
      case BinaryOp::IntSub:
        dest.si.i1 = src1.si.i1 - src2.si.i1;
        dest.si.i2 = src1.si.i2 - src2.si.i2;
        break;
      case BinaryOp::IntAnd:
        dest.si.i1 = src1.si.i1 & src2.si.i1;
        dest.si.i2 = src1.si.i2 & src2.si.i2;
        break;
      case BinaryOp::SIntMin:
        dest.si.i1 = std::min(src1.si.i1, src2.si.i1);
        dest.si.i2 = std::min(src1.si.i2, src2.si.i2);
        break;
      case BinaryOp::IntXor:
        dest.si.i1 = src1.si.i1 ^ src2.si.i1;
        dest.si.i2 = src1.si.i2 ^ src2.si.i2;
        break;
      default:
        assert(false && "Invalid op type.");
      }
    } else {
      // 1 double;
      switch (op) {
      case BinaryOp::FloatAdd:
        dest.d = src1.d + src2.d;
        break;
      case BinaryOp::FloatSub:
        dest.d = src1.d - src2.d;
        break;
      case BinaryOp::FloatMul:
        dest.d = src1.d * src2.d;
        break;
      case BinaryOp::FloatDiv:
        dest.d = src1.d / src2.d;
        break;
      case BinaryOp::IntAdd:
        dest.sl = src1.sl + src2.sl;
        break;
      case BinaryOp::IntSub:
        dest.sl = src1.sl - src2.sl;
        break;
      case BinaryOp::IntAnd:
        dest.sl = src1.sl & src2.sl;
        break;
      case BinaryOp::SIntMin:
        dest.sl = std::min(src1.sl, src2.sl);
        break;
      case BinaryOp::IntXor:
        dest.sl = src1.sl ^ src2.sl;
        break;
      default:
        assert(false && "Invalid op type.");
      }
    }
    return dest;
  }

  // A helper function to perform packed src1 op src2
  inline void doPackedBinaryOp(gem5::ExecContext *xc, BinaryOp op) const
  {
    // Each logical element (4 or 8 bytes) lives inside one or more 64-bit
    // sub-registers. We currently map one 64-bit subregister per iteration.
    // For 128-bit ops destVL=16 => vRegs=2 (two 64b pieces). For 256-bit ops
    // destVL=32 => vRegs=4.
  // Operand index ordering within StaticInst is:
  //   [dest0..destN-1, src10..src1N-1, src20..src2N-1]
  // i.e., all destination operands first, then all src1 lanes, then all src2 lanes.
    // Therefore, when reading sources use the source operand indices starting at 0
    // (i.e., [src10..src1N-1, src20..src2N-1]) without any destination offset.
    auto vRegs = destVL / sizeof(uint64_t);
    for (int i = 0; i < vRegs; i++) {
      FloatInt s1, s2;
      // With sources registered as [src1(0..N-1), src2(0..N-1)] starting at index 0
      s1.ul = xc->getRegOperand(this, i);
      s2.ul = xc->getRegOperand(this, vRegs + i);
      auto d = this->calcPackedBinaryOp(s1, s2, op);
      if (op == BinaryOp::FloatAdd) {
        // Each 64-bit chunk carries two 32-bit floats in order {f1,f2}.
        fprintf(stderr,
          "[AVX-TRACE] add chunk=%d raw_s1=%#016llx raw_s2=%#016llx f1={%g,%g} f2={%g,%g} result={%g,%g}\n",
          i,
          (unsigned long long)s1.ul,
          (unsigned long long)s2.ul,
          s1.f.f1, s1.f.f2,
          s2.f.f1, s2.f.f2,
          d.f.f1, d.f.f2);
      } else if (op == BinaryOp::FloatMul) {
        fprintf(stderr,
          "[AVX-TRACE] mul chunk=%d raw_s1=%#016llx raw_s2=%#016llx f1={%g,%g} f2={%g,%g} result={%g,%g}\n",
          i,
          (unsigned long long)s1.ul,
          (unsigned long long)s2.ul,
          s1.f.f1, s1.f.f2,
          s2.f.f1, s2.f.f2,
          d.f.f1, d.f.f2);
      } else if (op == BinaryOp::IntXor) {
        // For XOR, print raw hex values before/after.
        fprintf(stderr,
          "[AVX-TRACE] xor chunk=%d raw_s1=%#016llx raw_s2=%#016llx result_raw=%#016llx\n",
          i,
          (unsigned long long)s1.ul,
          (unsigned long long)s2.ul,
          (unsigned long long)d.ul);
      }
      xc->setRegOperand(this, i, d.ul);
      if (op == BinaryOp::FloatAdd) {
        // Read back what we stored to confirm write path.
        uint64_t stored = xc->getRegOperand(this, i);
        FloatInt verify; verify.ul = stored;
        fprintf(stderr,
          "[AVX-TRACE] stored chunk=%d raw=%#016llx asFloats={%g,%g}\n",
          i, (unsigned long long)stored, verify.f.f1, verify.f.f2);
      } else if (op == BinaryOp::FloatMul) {
        uint64_t stored = xc->getRegOperand(this, i);
        FloatInt verify; verify.ul = stored;
        fprintf(stderr,
          "[AVX-TRACE] stored chunk=%d raw=%#016llx asFloats={%g,%g}\n",
          i, (unsigned long long)stored, verify.f.f1, verify.f.f2);
      } else if (op == BinaryOp::IntXor) {
        uint64_t stored = xc->getRegOperand(this, i);
        fprintf(stderr,
          "[AVX-TRACE] stored chunk=%d raw=%#016llx\n",
          i, (unsigned long long)stored);
      }
    }
  }

  // A helper function to perform packed (src1 op1 src2) op2 src3
  inline void doFusedPackedBinaryOp(gem5::ExecContext *xc, BinaryOp op1, BinaryOp op2) const
  {
    auto vRegs = destVL / sizeof(uint64_t);
    FloatInt src1;
    FloatInt src2;
    FloatInt src3;
    for (int i = 0; i < vRegs; i++) {
      src1.ul = xc->getRegOperand(this, i * 3 + 0);
      src2.ul = xc->getRegOperand(this, i * 3 + 1);
      src3.ul = xc->getRegOperand(this, i * 3 + 2);
      auto tmp = this->calcPackedBinaryOp(src1, src2, op1);
      auto dest = this->calcPackedBinaryOp(tmp, src3, op2);
      xc->setRegOperand(this, i, dest.ul);
    }
  }

  // A helper function to add dest regs.
  inline void addAVXDestRegs() {
    auto vDestRegs = destVL / sizeof(uint64_t);
    fprintf(stderr, "[AVX-DESTREGS] destVL=%u vDestRegs=%zu dest=%u\n", 
            destVL, vDestRegs, dest);
    assert(vDestRegs <= NumXMMSubRegs && "DestVL overflow.");
    _numDestRegs = vDestRegs;
    _numTypedDestRegs[FloatRegClass] = vDestRegs;
    for (int i = 0; i < vDestRegs; i++) {
      fprintf(stderr, "[AVX-DESTREGS]   reg[%d] = FloatReg %u\n", i, dest + i);
      setDestRegIdx(i, RegId(floatRegClass, dest + i));
    }
  }
};

} } // namespace gem5::X86ISA

#endif //__ARCH_X86_INSTS_MICROAVXOP_HH__
