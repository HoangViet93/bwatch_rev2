void reset_handler(void);

void __attribute__ ((weak)) __libc_init_array (void)
{
}

void __attribute__ ((weak)) system_init (void)
{
}

extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

extern int main(void);

void reset_handler(void)
{
	unsigned long *src, *dst;

	src = &_sidata;
	dst = &_sdata;

	while (dst < &_edata)
	{
		*(dst++) = *(src++);
	}

	dst = &_sbss;
	while (dst < &_ebss)
	{
		*(dst++) = 0;
	}

	system_init();
	__libc_init_array();
	main();

	while(1)
	{
	}
}