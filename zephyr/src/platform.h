/**
 ******************************************************************************
 * @file    platform.h 
 * @author  MMY Application Team
 * @brief   Platform header file. Defining platform independent functionality. 
 *  
 *  This should contain all platform and hardware specifics such as 
 *  GPIO assignment, system resources, locks, IRQs, etc
 *  
 *  Each distinct platform/system/board must provide this definitions 
 *  for all SW layers to use
 ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

// Copyright © 2023 Vouch.io LLC

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L053xx
#include "stm32l0xx_hal.h"
#else 
//#include "stm32f4xx_hal.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "st_errno.h"
#include "timer.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

/** @addtogroup X-CUBE-NFC6_Applications
 *  @{
 */

/** @addtogroup PollingTagDetect
 *  @{
 */

/** @defgroup PTD_Platform
 *  @brief Demo functions containing the example code
 * @{
 */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup PTD_Platform_Exported_Constants
 *  @{
 */
#define ST25R_SS_PIN             BUS_SPI1_NSS_GPIO_PIN    /*!< GPIO pin used for ST25R SPI SS                */
#define ST25R_SS_PORT            BUS_SPI1_NSS_GPIO_PORT   /*!< GPIO port used for ST25R SPI SS port          */

gpio_pin_t platform_st25r_int_pin();
const struct device *platform_st25r_int_port();

#if CONFIG_RFAL_USE_I2C
#define RFAL_USE_I2C
#endif

#ifndef RFAL_USE_I2C
#define ST25R_INT_PIN            platform_st25r_int_pin()   /*!< GPIO pin used for ST25R IRQ                   */
#define ST25R_INT_PORT           platform_st25r_int_port()  /*!< GPIO port used for ST25R IRQ port             */
#else
#define ST25R_INT_PIN            platform_st25r_int_pin()   /*!< GPIO pin used for ST25R IRQ                   */
#define ST25R_INT_PORT           platform_st25r_int_port()  /*!< GPIO port used for ST25R IRQ port             */
#endif /* RFAL_USE_I2C */
#define IRQ_ST25R_EXTI_IRQn      EXTI0_1_IRQn

#ifdef DEFINE_LEDS_AND_BUTTONS

#ifdef LED_FIELD_Pin
#ifdef ST25R3916
#define PLATFORM_LED_FIELD_PIN       NFC06A1_LED6_PIN         /*!< GPIO pin used as field LED                        */
#endif /* ST25R3916 */
#ifdef ST25R3916B
#define PLATFORM_LED_FIELD_PIN       NFC08A1_LED6_PIN         /*!< GPIO pin used as field LED                        */
#endif /* ST25R3916B */
#endif

#ifdef LED_FIELD_GPIO_Port
#ifdef ST25R3916
#define PLATFORM_LED_FIELD_PORT      NFC06A1_LED6_PIN_PORT    /*!< GPIO port used as field LED                       */
#endif /* ST25R3916 */
#ifdef ST25R3916B
#define PLATFORM_LED_FIELD_PORT      NFC08A1_LED6_PIN_PORT    /*!< GPIO port used as field LED                       */
#endif /* ST25R3916B */
#endif

#ifdef ST25R3916
#define PLATFORM_LED_A_PIN           NFC06A1_LED3_PIN         /*!< GPIO pin used for LED A    */
#define PLATFORM_LED_A_PORT          NFC06A1_LED3_PIN_PORT    /*!< GPIO port used for LED A   */
#define PLATFORM_LED_B_PIN           NFC06A1_LED2_PIN         /*!< GPIO pin used for LED B    */
#define PLATFORM_LED_B_PORT          NFC06A1_LED2_PIN_PORT    /*!< GPIO port used for LED B   */
#define PLATFORM_LED_F_PIN           NFC06A1_LED1_PIN         /*!< GPIO pin used for LED F    */
#define PLATFORM_LED_F_PORT          NFC06A1_LED1_PIN_PORT    /*!< GPIO port used for LED F   */
#define PLATFORM_LED_V_PIN           NFC06A1_LED4_PIN         /*!< GPIO pin used for LED V    */
#define PLATFORM_LED_V_PORT          NFC06A1_LED4_PIN_PORT    /*!< GPIO port used for LED V   */
#define PLATFORM_LED_AP2P_PIN        NFC06A1_LED5_PIN         /*!< GPIO pin used for LED AP2P */
#define PLATFORM_LED_AP2P_PORT       NFC06A1_LED5_PIN_PORT    /*!< GPIO port used for LED AP2P*/
#endif /* ST25R3916 */
#ifdef ST25R3916B
#define PLATFORM_LED_A_PIN           NFC08A1_LED3_PIN         /*!< GPIO pin used for LED A    */
#define PLATFORM_LED_A_PORT          NFC08A1_LED3_PIN_PORT    /*!< GPIO port used for LED A   */
#define PLATFORM_LED_B_PIN           NFC08A1_LED2_PIN         /*!< GPIO pin used for LED B    */
#define PLATFORM_LED_B_PORT          NFC08A1_LED2_PIN_PORT    /*!< GPIO port used for LED B   */
#define PLATFORM_LED_F_PIN           NFC08A1_LED1_PIN         /*!< GPIO pin used for LED F    */
#define PLATFORM_LED_F_PORT          NFC08A1_LED1_PIN_PORT    /*!< GPIO port used for LED F   */
#define PLATFORM_LED_V_PIN           NFC08A1_LED4_PIN         /*!< GPIO pin used for LED V    */
#define PLATFORM_LED_V_PORT          NFC08A1_LED4_PIN_PORT    /*!< GPIO port used for LED V   */
#define PLATFORM_LED_AP2P_PIN        NFC08A1_LED5_PIN         /*!< GPIO pin used for LED AP2P */
#define PLATFORM_LED_AP2P_PORT       NFC08A1_LED5_PIN_PORT    /*!< GPIO port used for LED AP2P*/
#endif /* ST25R3916B */

#define PLATFORM_USER_BUTTON_PIN     USER_BUTTON_PIN          /*!< GPIO pin user button       */
#define PLATFORM_USER_BUTTON_PORT    USER_BUTTON_GPIO_PORT    /*!< GPIO port user button      */

#endif

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup PTD_Platform_Exported_Macro
 *  @{
 */

void platform_st25r_protect_comm();
void platform_st25r_unprotect_comm();
#define platformProtectST25RComm()                platform_st25r_protect_comm()
#define platformUnprotectST25RComm()              platform_st25r_unprotect_comm()

#define platformProtectST25RIrqStatus()           platformProtectST25RComm()                /*!< Protect unique access to IRQ status var - IRQ disable on single thread environment (MCU) ; Mutex lock on a multi thread environment */
#define platformUnprotectST25RIrqStatus()         platformUnprotectST25RComm()              /*!< Unprotect the IRQ status var - IRQ enable on a single thread environment (MCU) ; Mutex unlock on a multi thread environment         */

#define platformProtectWorker()                                                                     /* Protect RFAL Worker/Task/Process from concurrent execution on multi thread platforms   */
#define platformUnprotectWorker()                                                                   /* Unprotect RFAL Worker/Task/Process from concurrent execution on multi thread platforms */

#define platformIrqST25RSetCallback( cb )
#define platformIrqST25RPinInitialize()

#define platformLedsInitialize()                                                                    /*!< Initializes the pins used as LEDs to outputs*/

#define platformLedOff( port, pin )                                                                 /*!< Turns the given LED Off                     */
#define platformLedOn( port, pin )                                                                  /*!< Turns the given LED On                      */
#define platformLedToogle( port, pin )                                                              /*!< Toogle the given LED                        */

#define platformGpioSet( port, pin )                  gpio_pin_set(port, pin, 1)                    /*!< Turns the given GPIO High                   */
#define platformGpioClear( port, pin )                gpio_pin_set(port, pin, 0)                    /*!< Turns the given GPIO Low                    */
#define platformGpioToogle( port, pin )               gpio_pin_toggle(port, pin)                    /*!< Toogles the given GPIO                      */
#define platformGpioIsHigh( port, pin )               gpio_pin_get(port, pin)                       /*!< Checks if the given LED is High             */
#define platformGpioIsLow( port, pin )                (!platformGpioIsHigh(port, pin))              /*!< Checks if the given LED is Low              */

#define platformTimerCreate( t )                      timerCalculateTimer(t)                        /*!< Create a timer with the given time (ms)     */
#define platformTimerIsExpired( timer )               timerIsExpired(timer)                         /*!< Checks if the given timer is expired        */
#define platformTimerDestroy( timer )                                                               /*!< Stop and release the given timer            */
#define platformDelay( t )                            k_sleep(K_MSEC( t ))                                  /*!< Performs a delay for the given time (ms)    */

#define platformGetSysTick()                          k_uptime_get()                                /*!< Get System Tick ( 1 tick = 1 ms)            */

void platform_st25r_global_error(const char* file, long line);
#define platformErrorHandle()                         platform_st25r_global_error(__FILE__,__LINE__) /*!< Global error handler or trap                */

void platform_st25r_spi_select();
void platform_st25r_spi_deselect();
#define platformSpiSelect()                           platform_st25r_spi_select()                         /*!< SPI SS\CS: Chip|Slave Select                */
#define platformSpiDeselect()                         platform_st25r_spi_deselect()                         /*!< SPI SS\CS: Chip|Slave Deselect              */
void platform_st25r_spi_transceive(const uint8_t* txBuf, uint8_t* rxBuf, uint16_t len); 
#define platformSpiTxRx( txBuf, rxBuf, len )          platform_st25r_spi_transceive(txBuf, rxBuf, len)  /*!< SPI transceive                              */

void platform_st25r_i2c_send(uint16_t addr, uint8_t* txBuf, uint16_t len, bool last, bool txOnly);
void platform_st25r_i2c_recv(uint16_t addr, uint8_t* rxBuf, uint16_t len);
#define platformI2CTx( txBuf, len, last, txOnly )     platform_st25r_i2c_send((uint16_t)0xA0, (uint8_t *)(txBuf), (len), last, txOnly ) /*!< I2C Transmit                                */
#define platformI2CRx( txBuf, len )                   platform_st25r_i2c_recv((uint16_t)0xA0, rxBuf, len )           /*!< I2C Receive                                 */
#define platformI2CStart()                                                                          /*!< I2C Start condition                         */
#define platformI2CStop()                                                                           /*!< I2C Stop condition                          */
#define platformI2CRepeatStart()                                                                    /*!< I2C Repeat Start                            */
#define platformI2CSlaveAddrWR(add)                                                                 /*!< I2C Slave address for Write operation       */
#define platformI2CSlaveAddrRD(add)                                                                 /*!< I2C Slave address for Read operation        */

#define platformLog(...)                              printk(__VA_ARGS__)                         /*!< Log  method                                 */

/**
  * @}
  */

/*
******************************************************************************
* GLOBAL VARIABLES
******************************************************************************
*/
extern uint8_t globalCommProtectCnt;                      /* Global Protection Counter provided per platform - instantiated in main.c    */

/*
******************************************************************************
* RFAL FEATURES CONFIGURATION
******************************************************************************
*/

#define RFAL_FEATURE_LISTEN_MODE               IS_ENABLED(CONFIG_RFAL_FEATURE_LISTEN_MODE)             /*!< Enable/Disable RFAL support for Listen Mode                               */
#define RFAL_FEATURE_WAKEUP_MODE               IS_ENABLED(CONFIG_RFAL_FEATURE_WAKEUP_MODE)             /*!< Enable/Disable RFAL support for the Wake-Up mode                          */
#define RFAL_FEATURE_LOWPOWER_MODE             IS_ENABLED(CONFIG_RFAL_FEATURE_LOWPOWER_MODE)           /*!< Enable/Disable RFAL support for the Low Power mode                        */
#define RFAL_FEATURE_NFCA                      IS_ENABLED(CONFIG_RFAL_FEATURE_NFCA)                    /*!< Enable/Disable RFAL support for NFC-A (ISO14443A)                         */
#define RFAL_FEATURE_NFCB                      IS_ENABLED(CONFIG_RFAL_FEATURE_NFCB)                    /*!< Enable/Disable RFAL support for NFC-B (ISO14443B)                         */
#define RFAL_FEATURE_NFCF                      IS_ENABLED(CONFIG_RFAL_FEATURE_NFCF)                    /*!< Enable/Disable RFAL support for NFC-F (FeliCa)                            */
#define RFAL_FEATURE_NFCV                      IS_ENABLED(CONFIG_RFAL_FEATURE_NFCV)                    /*!< Enable/Disable RFAL support for NFC-V (ISO15693)                          */
#define RFAL_FEATURE_T1T                       IS_ENABLED(CONFIG_RFAL_FEATURE_T1T)                     /*!< Enable/Disable RFAL support for T1T (Topaz)                               */
#define RFAL_FEATURE_T2T                       IS_ENABLED(CONFIG_RFAL_FEATURE_T2T)                     /*!< Enable/Disable RFAL support for T2T                                       */
#define RFAL_FEATURE_T4T                       IS_ENABLED(CONFIG_RFAL_FEATURE_T4T)                     /*!< Enable/Disable RFAL support for T4T                                       */
#define RFAL_FEATURE_ST25TB                    IS_ENABLED(CONFIG_RFAL_FEATURE_ST25TB)                  /*!< Enable/Disable RFAL support for ST25TB                                    */
#define RFAL_FEATURE_ST25xV                    IS_ENABLED(CONFIG_RFAL_FEATURE_ST25xV)                  /*!< Enable/Disable RFAL support for  ST25TV/ST25DV                            */
#define RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG     IS_ENABLED(CONFIG_RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG)   /*!< Enable/Disable Analog Configs to be dynamically updated (RAM)             */
#define RFAL_FEATURE_DPO                       IS_ENABLED(CONFIG_RFAL_FEATURE_DPO)                     /*!< Enable/Disable RFAL Dynamic Power Output support                          */
#define RFAL_FEATURE_ISO_DEP                   IS_ENABLED(CONFIG_RFAL_FEATURE_ISO_DEP)                 /*!< Enable/Disable RFAL support for ISO-DEP (ISO14443-4)                      */
#define RFAL_FEATURE_ISO_DEP_POLL              IS_ENABLED(CONFIG_RFAL_FEATURE_ISO_DEP_POLL)            /*!< Enable/Disable RFAL support for Poller mode (PCD) ISO-DEP (ISO14443-4)    */
#define RFAL_FEATURE_ISO_DEP_LISTEN            IS_ENABLED(CONFIG_RFAL_FEATURE_ISO_DEP_LISTEN)          /*!< Enable/Disable RFAL support for Listen mode (PICC) ISO-DEP (ISO14443-4)   */
#define RFAL_FEATURE_NFC_DEP                   IS_ENABLED(CONFIG_RFAL_FEATURE_NFC_DEP)                 /*!< Enable/Disable RFAL support for NFC-DEP (NFCIP1/P2P)                      */

#define RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN    CONFIG_RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN  /*!< ISO-DEP I-Block max length. Please use values as defined by rfalIsoDepFSx */
#define RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN     CONFIG_RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN   /*!< NFC-DEP Block/Payload length. Allowed values: 64, 128, 192, 254           */
#define RFAL_FEATURE_NFC_RF_BUF_LEN            CONFIG_RFAL_FEATURE_NFC_RF_BUF_LEN          /*!< RF buffer length used by RFAL NFC layer                                   */

#define RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN      CONFIG_RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN    /*!< ISO-DEP APDU max length. Please use multiples of I-Block max length       */
#define RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN       CONFIG_RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN     /*!< NFC-DEP PDU max length.                                                   */

/*
******************************************************************************
* RFAL CUSTOM SETTINGS
******************************************************************************
  Custom analog configs are used to cope with Automatic Antenna Tuning (AAT)
  that are optimized differently for each board.
*/
#define RFAL_ANALOG_CONFIG_CUSTOM                         /*!< Use Custom Analog Configs when defined                                    */

/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_H */
