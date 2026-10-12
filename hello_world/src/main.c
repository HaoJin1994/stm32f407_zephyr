#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/printk.h>
#include "main.h"

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

#define SLEEP_TIME_MS   2000
#define UART1_NODE DT_ALIAS(uart1)
const struct device *uart = DEVICE_DT_GET(UART1_NODE);

#if 0
/* 回调函数：为每个线程调用 */
static void thread_stats_cb(const struct k_thread *thread, void *user_data)
{
    k_thread_runtime_stats_t stats;
    int ret;

    ret = k_thread_runtime_stats_get((k_tid_t)thread, &stats);
    if (ret != 0) {
        return;  /* 跳过无法获取统计信息的线程 */
    }

    /* 计算 CPU 使用率（基于系统总周期数） */
    uint32_t total_cycles = k_cycle_get_32();
    if (total_cycles == 0) {
        return;  /* 避免除零 */
    }

    uint32_t usage = (uint32_t)((stats.execution_cycles * 100U) / total_cycles);

    const char *name = k_thread_name_get((k_tid_t)thread);
    printk("%-20s %3u%%\n", (name && name[0]) ? name : "unnamed", usage);
}

void print_thread_stats(void)
{
    printk("Thread Name          CPU Usage\n");
    printk("--------------------------------\n");

    k_thread_foreach(thread_stats_cb, NULL);
}
#endif
static void uart_callback(const struct device *dev, void *user_data)
{
    uint8_t c;

    uart_irq_update(dev);

    // 检查是否有数据可读
    if (uart_irq_rx_ready(dev)) {
        // 读取数据
        while (uart_fifo_read(dev, &c, 1) == 1) {
            
            //printk("%02x ", c);
            uart_poll_out(dev, c);      
        }
        //printk("\r\n");
        uart_poll_out(dev, '\n');

    }
}


int main(void)
{

    int value = 42;
    printk("Application started\n");
    
    k_msleep(SLEEP_TIME_MS);


    if(!device_is_ready(uart)){
         printk("UART device not ready\n");
         return -1;
    }
    uart_irq_callback_set(uart,uart_callback);
    
    uart_irq_rx_enable(uart);

    printk("UART interrupt mode enabled\n");

    //littlefs_example();
    //flash_test();
    //k_msleep(SLEEP_TIME_MS);
    //print_thread_stats();
    while(1){
        k_sleep(K_FOREVER);
    }
    return 0;

}