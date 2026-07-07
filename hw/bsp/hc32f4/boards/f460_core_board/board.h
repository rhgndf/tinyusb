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
   name: F460 Core Board
   url: https://vcc-gnd.com
*/

#ifndef BOARD_H_
#define BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LED_PORT            GPIO_PORT_B
#define LED_PIN             GPIO_PIN_04
#define LED_STATE_ON        1

#define BUTTON_PORT         GPIO_PORT_B
#define BUTTON_PIN          GPIO_PIN_03
#define BUTTON_STATE_ACTIVE 0

#define XTAL_PORT           GPIO_PORT_H
#define XTAL_IN_PIN         GPIO_PIN_01
#define XTAL_OUT_PIN        GPIO_PIN_00

#define PRINT_UART          CM_USART2
#define PRINT_UART_CLOCK    FCG1_PERIPH_USART2
#define PRINT_UART_TX_PORT  GPIO_PORT_A
#define PRINT_UART_TX_PIN   GPIO_PIN_02
#define PRINT_UART_TX_FUNC  GPIO_FUNC_36
#define PRINT_UART_RX_PORT  GPIO_PORT_A
#define PRINT_UART_RX_PIN   GPIO_PIN_03
#define PRINT_UART_RX_FUNC  GPIO_FUNC_37

#define USBFS_INT_IRQn      INT003_IRQn
#define USBFS_INT_SRC       INT_SRC_USBFS_GLB

#ifdef __cplusplus
}
#endif

#endif
