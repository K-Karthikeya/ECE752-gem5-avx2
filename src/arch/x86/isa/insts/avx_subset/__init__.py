# avx_subset/__init__.py
# Aggregate minimal AVX2 subset microcode.

from .floating_point.arithmetic import vaddps, vmulps, vxorps, vsubps, vandps, vdivps, vfmadd231ps, vcmpps
from .floating_point.data_transfer import vmovups, vmovaps, vzeroupper
from .integer.arithmetic import vpaddd, vpand, vpminsd, vpbroadcastd
from .integer.data_transfer import vmovdqu

microcode = ""
microcode += vaddps.microcode
microcode += vmulps.microcode  
microcode += vxorps.microcode
microcode += vmovups.microcode
microcode += vmovaps.microcode
microcode += vzeroupper.microcode
microcode += vsubps.microcode
microcode += vandps.microcode
microcode += vdivps.microcode
microcode += vfmadd231ps.microcode
microcode += vcmpps.microcode
microcode += vpaddd.microcode
microcode += vpand.microcode
microcode += vpminsd.microcode
microcode += vpbroadcastd.microcode
microcode += vmovdqu.microcode