#ifndef HEART_SERV_H
#define HEART_SERV_H

void heart_serv_init(void);
void heart_serv_add_event(void (*event)(uint16_t, uint16_t *, uint8_t));
void heart_serv_update(void);
void heart_serv_deinit(void);

#endif

