// #include "configuration.h"
// #include <SPI.h>
// #include <Wire.h>
// #include <assert.h>
// #include <memory.h>
// #include <stdio.h>

// #include "error.h"

// #ifdef BQ25703A_ADDR
// #include "BQ25713.h"
// #endif

// static inline void debugger_break(void)
// {
//     __asm volatile("bkpt #0x01\n\t"
//                    "mov pc, lr\n\t");
// }

// bool loopCanSleep() {
//     // turn off sleep only while connected via USB
//     // return true;
//     return !Serial; // the bool operator on the nrf52 serial class returns true if connected to a PC currently
//     // return !(TinyUSBDevice.mounted() && !TinyUSBDevice.suspended());
// }

// // handle standard gcc assert failures
// void __attribute__((noreturn)) __assert_func(const char *file, int line, const char *func, const char *failedexpr)
// {
//     DEBUG_MSG("assert failed %s: %d, %s, test=%s\n", file, line, func, failedexpr);
//     // debugger_break(); FIXME doesn't work, possibly not for segger
//     while (1)
//         ; // FIXME, reboot!
// }

// void getMacAddr(uint8_t *dmac)
// {
//     const uint8_t *src = (const uint8_t *)NRF_FICR->DEVICEADDR;
//     dmac[5] = src[0];
//     dmac[4] = src[1];
//     dmac[3] = src[2];
//     dmac[2] = src[3];
//     dmac[1] = src[4];
//     dmac[0] = src[5] | 0xc0; // MSB high two bits get set elsewhere in the bluetooth stack
// }

// static void initBrownout()
// {
//     auto vccthresh = POWER_POFCON_THRESHOLD_V17;

//     auto err_code = sd_power_pof_enable(POWER_POFCON_POF_Enabled);
//     assert(err_code == NRF_SUCCESS);

//     err_code = sd_power_pof_threshold_set(vccthresh);
//     assert(err_code == NRF_SUCCESS);

//     // We don't bother with setting up brownout if soft device is disabled - because during production we always use softdevice
// }

// /**
//  * Override printf to use the SEGGER output library (note - this does not effect the printf method on the debug console)
//  */
// int printf(const char *fmt, ...)
// {
//     va_list args;
//     va_start(args, fmt);
//     auto res = SEGGER_RTT_vprintf(0, fmt, &args);
//     va_end(args);
//     return res;
// }

// void checkSDEvents()
// {
//     if (useSoftDevice) {
//         uint32_t evt;
//         while (NRF_SUCCESS == sd_evt_get(&evt)) {
//             switch (evt) {
//             case NRF_EVT_POWER_FAILURE_WARNING:
//                 RECORD_CRITICALERROR(CriticalErrorCode_Brownout);
//                 break;

//             default:
//                 DEBUG_MSG("Unexpected SDevt %d\n", evt);
//                 break;
//             }
//         }
//     } else {
//         if (NRF_POWER->EVENTS_POFWARN)
//             RECORD_CRITICALERROR(CriticalErrorCode_Brownout);
//     }
// }

// void nrf52Loop()
// {
//     checkSDEvents();
// }

// void nrf52Setup()
// {
//     auto why = NRF_POWER->RESETREAS;
//     // per
//     // https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.nrf52832.ps.v1.1%2Fpower.html
//     DEBUG_MSG("Reset reason: 0x%x\n", why);

//     // Per
//     // https://devzone.nordicsemi.com/nordic/nordic-blog/b/blog/posts/monitor-mode-debugging-with-j-link-and-gdbeclipse
//     // This is the recommended setting for Monitor Mode Debugging
//     NVIC_SetPriority(DebugMonitor_IRQn, 6UL);

// #ifdef BQ25703A_ADDR
//     auto *bq = new BQ25713();
//     if (!bq->setup())
//         DEBUG_MSG("ERROR! Charge controller init failed\n");
// #endif
//         DEBUG_MSG("FIXME, call randomSeed\n");
// }

// void cpuDeepSleep(uint64_t msecToWake)
// {
//     // FIXME, configure RTC or button press to wake us
//     // FIXME, power down SPI, I2C, RAMs
// #ifndef NO_WIRE
//     Wire.end();
// #endif
//     SPI.end();
//     // This may cause crashes as debug messages continue to flow.
//     Serial.end();

// #ifdef PIN_SERIAL_RX1
//     Serial1.end();
// #endif
//     auto ok = sd_power_system_off();
//     if (ok != NRF_SUCCESS) {
//         DEBUG_MSG("FIXME: Ignoring soft device (EasyDMA pending?) and forcing "
//                   "system-off!\n");
//         NRF_POWER->SYSTEMOFF = 1;
//     }

//     // The following code should not be run, because we are off
//     while (1) {
//         delay(5000);
//         DEBUG_MSG(".");
//     }
// }
