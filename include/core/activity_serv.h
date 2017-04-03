#ifndef ACTIVITY_SERV_H
#define ACTIVITY_SERV_H

enum 
{
	ACT_EVENT_INACTIVITY = 100,
	ACT_EVENT_ACTIVITY,
	ACT_EVENT_FF,
};

void activity_serv_init(void);
int8_t activity_serv_event_cb_add(void (*pfunc)(uint8_t event));
void activity_serv_event_enable(void);
void activity_serv_event_disable(void);
void activity_serv_deinit(void);

#endif

