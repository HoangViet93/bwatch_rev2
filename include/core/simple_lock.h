#if defined(CONFIG_FREERTOS)
#define DEFINE_LOCK(key) volatile uint8_t key = 0;

#define LOCK(key)			\
	do {					\
		while (1 == key);	\
		key = 1;			\
	} while(0)				
#define UNLOCK(key)			\
	do {					\
		key = 0;			\
	} while(0)		
#else
#define LOCK(key)
#define UNLOCK(key)
#endif

