/* Specify the memory areas */
MEMORY
{
	FLASH (rx)	: ORIGIN = 0x08000000, LENGTH = 64K
	RAM (xrw)	: ORIGIN = 0x20000000, LENGTH = 20K
	CPUID (r)   : ORIGIN = 0x1ffff7e8, LENGTH = 12
}

_cpuid_address = ORIGIN(CPUID);

INCLUDE cortex_m.ld