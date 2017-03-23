#ifndef EVENT_H
#define EVENT_H

enum
{
    BTN5 = 0,
    BTN6,
    BTN_MAX
};

enum
{
    BTN_PRESSED = 0,
    BTN_LONG_PRESSED,
};

void button_init(uint8_t id);
void button_enable(uint32_t id);
void button_disable(uint32_t id);
int8_t button_register_event(uint8_t id, void (*pfunc)(uint8_t event));
int8_t button_unregister_event(uint8_t id, void (*pfunc)(uint8_t event));

#endif

