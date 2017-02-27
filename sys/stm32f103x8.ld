/* Specify the memory areas */
MEMORY
{
	rom (rx)	: ORIGIN = 0x08000000, LENGTH = 64K
	ram (xrw)	: ORIGIN = 0x20000000, LENGTH = 20K
	cpuid (r)   : ORIGIN = 0x1ffff7e8, LENGTH = 12
}

_cpuid_address = ORIGIN(cpuid);

INCLUDE libopencm3_stm32f1.ld