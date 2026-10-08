#include <zephyr/shell/shell.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

/* 假设应用程序已经全局初始化了 uart 指针 */
extern const struct device *uart;

static int cmd_uart_send(const struct shell *sh, size_t argc, char **argv)
{
    if (argc < 2) {
        shell_print(sh, "Usage: uart send <string>");
        return -EINVAL;
    }

    /* 直接使用已有的 uart 设备发送 */
    for (int i = 0; argv[1][i] != '\0'; i++) {
        uart_poll_out(uart, argv[1][i]);
    }
    uart_poll_out(uart, '\r');
    uart_poll_out(uart, '\n');

    shell_print(sh, "Sent: %s", argv[1]);
    return 0;
}

SHELL_CMD_REGISTER(uart_send, NULL, "Send data over UART", cmd_uart_send);