# avx_subset/__init__.py
# Aggregate minimal AVX2 subset microcode.

from .floating_point.arithmetic import vaddps, vmulps, vxorps
from .floating_point.data_transfer import vmovups, vmovaps, vzeroupper
from .double.arithmetic import vaddpd, vsubpd, vmulpd, vdivpd
from .double.data_transfer import vbroadcastsd, vmovddup

microcode = ""
microcode += vaddps.microcode
microcode += vmulps.microcode  
microcode += vxorps.microcode
microcode += vmovups.microcode
microcode += vmovaps.microcode
microcode += vzeroupper.microcode
microcode += vaddpd.microcode
microcode += vsubpd.microcode
microcode += vmulpd.microcode
microcode += vdivpd.microcode
microcode += vbroadcastsd.microcode
microcode += vmovddup.microcode