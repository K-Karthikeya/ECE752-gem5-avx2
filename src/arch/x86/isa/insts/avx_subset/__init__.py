# avx_subset/__init__.py
# Aggregate minimal AVX2 subset microcode.

from .double.arithmetic import (
    vaddpd,
    vdivpd,
    vmulpd,
    vsubpd,
)
from .double.data_transfer import (
    vbroadcastsd,
    vmovddup,
)
from .floating_point.arithmetic import (
    vaddps,
    vandps,
    vcmpps,
    vdivps,
    vfmadd231ps,
    vmulps,
    vsubps,
    vxorps,
)
from .floating_point.data_transfer import (
    vmovaps,
    vmovups,
    vzeroupper,
)

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
microcode += vaddpd.microcode
microcode += vsubpd.microcode
microcode += vmulpd.microcode
microcode += vdivpd.microcode
microcode += vbroadcastsd.microcode
microcode += vmovddup.microcode
