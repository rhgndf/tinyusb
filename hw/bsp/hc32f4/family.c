/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2026, tinyusb.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* metadata:
   manufacturer: HDSC
*/

#include "hc32_ll.h"
#include "bsp/board_api.h"
#include "board.h"

#define BOARD_PERIPH_WE                                                                                      \
  (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_INTC | LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)

static volatile uint32_t system_ticks = 0;

static void usbfs_irq_handler(void) {
  tusb_int_handler(0, true);
}

static void board_clock_init(void) {
  stc_clock_xtal_init_t xtal_init;
  stc_clock_pll_init_t  pll_init;
  stc_clock_pllx_init_t usb_pll_init;

  CLK_SetClockDiv(CLK_BUS_CLK_ALL, (CLK_HCLK_DIV1 | CLK_EXCLK_DIV2 | CLK_PCLK0_DIV1 | CLK_PCLK1_DIV2 | CLK_PCLK2_DIV4 |
                                    CLK_PCLK3_DIV4 | CLK_PCLK4_DIV2));

  GPIO_AnalogCmd(XTAL_PORT, XTAL_IN_PIN | XTAL_OUT_PIN, ENABLE);

  (void)CLK_XtalStructInit(&xtal_init);
  xtal_init.u8Mode       = CLK_XTAL_MD_OSC;
  xtal_init.u8Drv        = CLK_XTAL_DRV_ULOW;
  xtal_init.u8State      = CLK_XTAL_ON;
  xtal_init.u8StableTime = CLK_XTAL_STB_2MS;
  TU_ASSERT(CLK_XtalInit(&xtal_init) == LL_OK, );

  (void)CLK_PLLStructInit(&pll_init);
  pll_init.u8PLLState       = CLK_PLL_ON;
  pll_init.PLLCFGR          = 0UL;
  pll_init.PLLCFGR_f.PLLM   = 1UL - 1UL;
  pll_init.PLLCFGR_f.PLLN   = 50UL - 1UL;
  pll_init.PLLCFGR_f.PLLP   = 2UL - 1UL;
  pll_init.PLLCFGR_f.PLLQ   = 2UL - 1UL;
  pll_init.PLLCFGR_f.PLLR   = 2UL - 1UL;
  pll_init.PLLCFGR_f.PLLSRC = CLK_PLL_SRC_XTAL;
  TU_ASSERT(CLK_PLLInit(&pll_init) == LL_OK, );

  SRAM_SetWaitCycle(SRAM_SRAMH, SRAM_WAIT_CYCLE0, SRAM_WAIT_CYCLE0);
  SRAM_SetWaitCycle((SRAM_SRAM12 | SRAM_SRAM3 | SRAM_SRAMR), SRAM_WAIT_CYCLE1, SRAM_WAIT_CYCLE1);
  TU_ASSERT(EFM_SetWaitCycle(EFM_WAIT_CYCLE5) == LL_OK, );
  GPIO_SetReadWaitCycle(GPIO_RD_WAIT3);
  TU_ASSERT(PWC_HighSpeedToHighPerformance() == LL_OK, );

  CLK_SetSysClockSrc(CLK_SYSCLK_SRC_PLL);

  (void)CLK_PLLxStructInit(&usb_pll_init);
  usb_pll_init.u8PLLState     = CLK_PLL_ON;
  usb_pll_init.PLLCFGR        = 0UL;
  usb_pll_init.PLLCFGR_f.PLLM = 2UL - 1UL;
  usb_pll_init.PLLCFGR_f.PLLN = 120UL - 1UL;
  usb_pll_init.PLLCFGR_f.PLLP = 10UL - 1UL;
  usb_pll_init.PLLCFGR_f.PLLQ = 6UL - 1UL;
  usb_pll_init.PLLCFGR_f.PLLR = 6UL - 1UL;
  TU_ASSERT(CLK_PLLxInit(&usb_pll_init) == LL_OK, );

  CLK_SetUSBClockSrc(CLK_USBCLK_PLLXP);
}

static void gpio_init(void) {
  stc_gpio_init_t gpio_init;

  (void)GPIO_StructInit(&gpio_init);
  gpio_init.u16PinDir = PIN_DIR_OUT;
  GPIO_ResetPins(LED_PORT, LED_PIN);
  (void)GPIO_Init(LED_PORT, LED_PIN, &gpio_init);

  (void)GPIO_StructInit(&gpio_init);
  gpio_init.u16PullUp = PIN_PU_ON;
  (void)GPIO_Init(BUTTON_PORT, BUTTON_PIN, &gpio_init);
}

static void uart_init(void) {
  stc_usart_uart_init_t uart_init;

  FCG_Fcg1PeriphClockCmd(PRINT_UART_CLOCK, ENABLE);
  GPIO_SetFunc(PRINT_UART_TX_PORT, PRINT_UART_TX_PIN, PRINT_UART_TX_FUNC);
  GPIO_SetFunc(PRINT_UART_RX_PORT, PRINT_UART_RX_PIN, PRINT_UART_RX_FUNC);

  (void)USART_UART_StructInit(&uart_init);
  uart_init.u32ClockSrc = USART_CLK_SRC_INTERNCLK;
  uart_init.u32Baudrate = CFG_BOARD_UART_BAUDRATE;
  uart_init.u32CKOutput = USART_CK_OUTPUT_ENABLE;
  (void)USART_UART_Init(PRINT_UART, &uart_init, NULL);
  USART_FuncCmd(PRINT_UART, USART_TX | USART_RX, ENABLE);
}

static void usb_int_init(void) {
  const stc_irq_signin_config_t irq_config = {.enIntSrc    = USBFS_INT_SRC,
                                              .enIRQn      = USBFS_INT_IRQn,
                                              .pfnCallback = usbfs_irq_handler};

  FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USBFS, ENABLE);
  (void)INTC_IrqSignIn(&irq_config);
  NVIC_ClearPendingIRQ(USBFS_INT_IRQn);
  NVIC_SetPriority(USBFS_INT_IRQn, DDL_IRQ_PRIO_03);
}

void board_init(void) {
  __disable_irq();

  LL_PERIPH_WE(BOARD_PERIPH_WE);

  board_clock_init();
  SystemCoreClockUpdate();

#if CFG_TUSB_OS == OPT_OS_NONE
  SysTick_Config(SystemCoreClock / 1000U);
#endif

  gpio_init();
  uart_init();
  usb_int_init();

  __enable_irq();
}

void board_led_write(bool state) {
  const bool pin_state = (state ? LED_STATE_ON : (1 - LED_STATE_ON));

  if (pin_state) {
    GPIO_SetPins(LED_PORT, LED_PIN);
  } else {
    GPIO_ResetPins(LED_PORT, LED_PIN);
  }
}

uint32_t board_button_read(void) {
  const bool pin_state = (GPIO_ReadInputPins(BUTTON_PORT, BUTTON_PIN) == PIN_SET);

  return (pin_state == BUTTON_STATE_ACTIVE) ? 1U : 0U;
}

int board_uart_read(uint8_t *buf, int len) {
  int count = 0;

  while ((count < len) && (USART_GetStatus(PRINT_UART, USART_FLAG_RX_FULL) == SET)) {
    buf[count++] = (uint8_t)USART_ReadData(PRINT_UART);
  }

  return count;
}

int board_uart_write(const void *buf, int len) {
  const uint8_t *buf8 = (const uint8_t *)buf;

  for (int i = 0; i < len; i++) {
    while (USART_GetStatus(PRINT_UART, USART_FLAG_TX_EMPTY) != SET) {}
    USART_WriteData(PRINT_UART, buf8[i]);
  }

  return len;
}

#if CFG_TUSB_OS == OPT_OS_NONE
void SysTick_Handler(void) {
  system_ticks++;
}

uint32_t tusb_time_millis_api(void) {
  return system_ticks;
}
#endif

void _init(void) {
}
