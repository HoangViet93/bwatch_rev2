#ifndef SIMPLE_LOCK_H
#define SIMPLE_LOCK_H

#if defined(CONFIG_FREERTOS)
#define DECLARE_LOCK(key) extern uint8_t key;
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
#define DECLARE_LOCK(key)
#define DEFINE_LOCK(key)
#define LOCK(key)
#define UNLOCK(key)
#endif

#endif
