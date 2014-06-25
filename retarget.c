#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

/* This is the original retarget.c but with code filled in for
 * the STM32F4 and the Butterfly board.
 */
void retarget_init()
{
  // Initialize UART
/* *** Added Code Begin *** */
	/* MUST enable the GPIO clock in ADDITION to the USART clock */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* This example uses PD5 and PD6 for Tx and Rx respectively
	 * but other pins are available for this role on USART6 (our chosen
 	 * USART) as well, such as PA2 and PA3. You can also split them
	 * so PA2 for Tx, PD6 for Rx but you would have to enable both
	 * the GPIOA and GPIOD clocks in that case
	 */
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);

	/* Actual Alternate function number (in this case 7) is part
	 * depenedent, CHECK THE DATA SHEET for the right number to
	 * use.
	 */
	gpio_set_af(GPIOC, GPIO_AF8, GPIO6 | GPIO7);


	/* This then enables the clock to the USART6 peripheral which is
	 * attached inside the chip to the APB2 bus. Different peripherals
	 * attach to different buses, and even some UARTS are attached to
	 * APB1 and some to APB2, again the data sheet is useful here.
	 */
	rcc_periph_clock_enable(RCC_USART6);

	/* Set up USART/UART parameters using the libopencm3 helper functions */
	usart_set_baudrate(USART6, 115200);
	usart_set_databits(USART6, 8);
	usart_set_stopbits(USART6, USART_STOPBITS_1);
	usart_set_mode(USART6, USART_MODE_TX_RX);
	usart_set_parity(USART6, USART_PARITY_NONE);
	usart_set_flow_control(USART6, USART_FLOWCONTROL_NONE);
	usart_enable(USART6);

/* *** Added Code End *** */

}

int _write (int fd, char *ptr, int len)
{
  /* Write "len" of char from "ptr" to file id "fd"
   * Return number of char written.
   * Need implementing with UART here. */
	int i = 0;
	while (*ptr && (i < len)) {
		usart_send_blocking(USART6, *ptr);
		if (*ptr == '\n') {
			usart_send_blocking(USART6, '\r');
		}
		i++;
		ptr++;
	}
  return i;
}

int _read (int fd, char *ptr, int len)
{
  /* Read "len" of char to "ptr" from file id "fd"
   * Return number of char read.
   * Need implementing with UART here. */
	int	i = 0;
	while (i < len) {
		*ptr = usart_recv_blocking(USART6);
		ptr++;
		i++;
	}
  return i;
}

void _ttywrch(int ch) {
  /* Write one char "ch" to the default console
   * Need implementing with UART here. */
	usart_send_blocking(USART6, ch);
}

/* SystemInit will be called before main */
void SystemInit()
{
/* *** Added code *** */
	/* Base board frequency, set to 168Mhz */
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);
/* *** End Added Code *** */
    retarget_init();
}

