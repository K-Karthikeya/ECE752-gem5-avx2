# avx_subset/__init__.py
# Aggregate minimal AVX2 subset microcode.

from .floating_point.arithmetic import vaddps, vmulps, vxorps
from .floating_point.data_transfer import vmovups, vmovaps, vzeroupper

microcode = ""
microcode += vaddps.microcode
microcode += vmulps.microcode  
microcode += vxorps.microcode
microcode += vmovups.microcode
microcode += vmovaps.microcode
microcode += vzeroupper.microcode