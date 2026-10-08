#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/shell/shell.h>
#include "main.h"

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

#define SLEEP_TIME_MS   2000
#define UART1_NODE DT_ALIAS(uart1)
const struct device *uart = DEVICE_DT_GET(UART1_NODE);


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

    littlefs_example();
    //flash_test();
    while(1){
        k_sleep(K_FOREVER);
    }
    return 0;

}