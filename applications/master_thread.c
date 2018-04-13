#include "master_thread.h"
#include "app_eaix4.h"
#include "app_wirelessuart.h"
#include "app_mpu9250.h"
#include "app_beep.h"
#include "EAI_X4.h"
struct rt_event ControlEvent;

void master_thread_entry(void *parameter)
{
	rt_uint32_t e;
	rt_uint8_t i;
	while(1)
	{
		/* 接收事件 */
		if(rt_event_recv(&ControlEvent, controlEventEaix4, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, &e)==RT_EOK)
		{
			beep(1);
			for (i = 0; i < 5; i++)
			{
					rt_kprintf("circle:%d firstpont.angle:%d firstpont.distance:%d\n", i, (around[i].firstpont.angle), (around[i].firstpont.distance));
					for (rt_uint16_t x = 0; x < around[i].number; x++)
					{
							rt_kprintf("circle:%d angle:%d distance:%d\n", i, (around[i].ap[x].angle), (around[i].ap[x].distance));
//							rt_thread_delay(rt_tick_from_millisecond(20));
					}
			}
			beep(0);
		}
		else
		{
			rt_thread_delay(rt_tick_from_millisecond(50));
		}
	}
}

int master_init()
{
    rt_thread_t tid;
		rt_kprintf("\nmaster_init.\n");
		rt_event_init(&ControlEvent,"Control",RT_IPC_FLAG_FIFO);
		eaix4_init();
		wireless_init();
		mpu9250_init();
		rt_kprintf("\nmaster_done.\n");
    /* 创建test线程 */
    tid = rt_thread_create("master",
                           master_thread_entry,
                           RT_NULL,
                           2048,
                           3,
                           20);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);	
		
		return 0;
}
INIT_APP_EXPORT(master_init);
