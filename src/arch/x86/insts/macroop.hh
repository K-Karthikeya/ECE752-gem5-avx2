/*
 * Copyright (c) 2007 The Hewlett-Packard Development Company
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ARCH_X86_INSTS_MACROOP_HH__
#define __ARCH_X86_INSTS_MACROOP_HH__

#include "arch/x86/insts/badmicroop.hh"
#include "arch/x86/insts/static_inst.hh"
#include "arch/x86/emulenv.hh"
#include "arch/x86/types.hh"
#include <typeinfo>

namespace gem5
{

namespace X86ISA
{
// Base class for combinationally generated macroops
class MacroopBase : public X86StaticInst
{
  protected:
    const char *macrocodeBlock;

    const uint32_t numMicroops;
    X86ISA::EmulEnv env;

    //Constructor.
    MacroopBase(const char *mnem, ExtMachInst _machInst,
            uint32_t _numMicroops, X86ISA::EmulEnv _env) :
                X86StaticInst(mnem, _machInst, No_OpClass),
                numMicroops(_numMicroops), env(_env)
    {
        assert(numMicroops);
        microops = new StaticInstPtr[numMicroops];
        flags[IsMacroop] = true;
    }

    ~MacroopBase()
    {
        delete [] microops;
    }

    StaticInstPtr * microops;

    StaticInstPtr
    fetchMicroop(MicroPC microPC) const override
    {
        if (microPC >= numMicroops)
            return badMicroop;
        else
            return microops[microPC];
    }

    std::string
    generateDisassembly(Addr pc,
                        const loader::SymbolTable *symtab) const override
    {
        return mnemonic;
    }

  public:
    ExtMachInst
    getExtMachInst()
    {
        return machInst;
    }

    X86ISA::EmulEnv
    getEmulEnv()
    {
        return env;
    }

    void size(size_t newSize) override
    {
        fprintf(stderr, "[MACROOP-SIZE] mnemonic=%s numMicroops=%u newSize=%zu\n",
                mnemonic, numMicroops, newSize);
        for (int i = 0; i < numMicroops; i++) {
            fprintf(stderr, "[MACROOP-SIZE]   microop[%d] = %p\n", i, 
                    (void*)microops[i].get());
            if (!microops[i]) {
                fprintf(stderr, "[MACROOP-SIZE] ERROR: microop[%d] is NULL!\n", i);
                panic("NULL microop at index %d in macroop %s\n", i, mnemonic);
            }
            // Inspect vtable pointer and a few leading bytes for corruption diagnosis.
            void **rawPtr = reinterpret_cast<void**>(microops[i].get());
            void *vtablePtr = rawPtr ? rawPtr[0] : nullptr;
            fprintf(stderr, "[MACROOP-SIZE]   microop[%d] vtable=%p\n", i, vtablePtr);
            // Dump first 32 bytes (up to four pointers) of the object (best-effort, may fault if corrupted).
            if (rawPtr) {
                uintptr_t *u64 = reinterpret_cast<uintptr_t*>(rawPtr);
                fprintf(stderr, "[MACROOP-SIZE]   microop[%d] mem[0..3]=%#016lx %#016lx %#016lx %#016lx\n",
                        i,
                        (unsigned long)(u64[0]), (unsigned long)(u64[1]),
                        (unsigned long)(u64[2]), (unsigned long)(u64[3]));
            }
            fprintf(stderr, "[MACROOP-SIZE] -> calling microop[%d]->size(%zu)\n", i, newSize);
            microops[i]->size(newSize);
            fprintf(stderr, "[MACROOP-SIZE] <- returned microop[%d]->size(%zu)\n", i, newSize);
        }
        _size = newSize;
        fprintf(stderr, "[MACROOP-SIZE] complete for %s\n", mnemonic);
    }
};

} // namespace X86ISA
} // namespace gem5

#endif //__ARCH_X86_INSTS_MACROOP_HH__
