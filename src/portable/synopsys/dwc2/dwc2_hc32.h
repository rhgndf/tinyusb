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
 *
 * This file is part of the TinyUSB stack.
 */

#ifndef DWC2_HC32_H_
#define DWC2_HC32_H_

#include "hc32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DWC2_EP_MAX 6

static const dwc2_controller_t _dwc2_controller[] = {{.reg_base        = CM_USBFS_BASE,
                                                      .irqnum          = INT003_IRQn,
                                                      .ep_count        = DWC2_EP_MAX,
                                                      .ep_in_count     = DWC2_EP_MAX,
                                                      .otg_dfifo_depth = 320}};

TU_ATTR_ALWAYS_INLINE static inline void dwc2_clock_init(uint8_t rhport, tusb_role_t role) {
  (void)rhport;
  (void)role;
}

TU_ATTR_ALWAYS_INLINE static inline void dwc2_dcd_int_enable(uint8_t rhport) {
  NVIC_EnableIRQ((IRQn_Type)_dwc2_controller[rhport].irqnum);
}

TU_ATTR_ALWAYS_INLINE static inline void dwc2_dcd_int_disable(uint8_t rhport) {
  NVIC_DisableIRQ((IRQn_Type)_dwc2_controller[rhport].irqnum);
}

static inline void dwc2_remote_wakeup_delay(void) {
  uint32_t count = SystemCoreClock / 1000U;
  while (count--) {
    __NOP();
  }
}

TU_ATTR_ALWAYS_INLINE static inline void dwc2_phy_init(dwc2_regs_t *dwc2, uint8_t hs_phy_type) {
  (void)dwc2;
  (void)hs_phy_type;
}

TU_ATTR_ALWAYS_INLINE static inline void dwc2_phy_deinit(dwc2_regs_t *dwc2, uint8_t hs_phy_type) {
  (void)dwc2;
  (void)hs_phy_type;
}

TU_ATTR_ALWAYS_INLINE static inline void dwc2_phy_update(dwc2_regs_t *dwc2, uint8_t hs_phy_type) {
  (void)dwc2;
  (void)hs_phy_type;
}

#ifdef __cplusplus
}
#endif

#endif
