#ifndef INA219_H
#define INA219_H
#include "ina219_conf.h"
#if ENABLE_INA219 == 1

//--------------------------------------------------
#if defined(STM32WB)
#include "stm32wbxx_hal.h"
#elif defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32L1)
#include "stm32l1xx_hal.h"
#elif defined(STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32L5)
#include "stm32l5xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#elif defined(STM32C0)
#include "stm32c0xx_hal.h"
#else
#error "INA219 library was tested only on STM32F0, STM32F1, STM32F3, STM32F4, STM32F7, STM32L0, STM32L1, STM32L4, STM32H7, STM32G0, STM32G4, STM32WB, STM32C0 MCU families. Please modify ssd1306.h if you know what you are doing. Also please send a pull request if it turns out the library works on other MCU's as well!"
#endif

//--------------------------------------------------
//---- 寄存器定义 ----
//--------------------------------------------------
#define CONFIG_REG          0x00
#define SHUNT_VOLTAGE_REG   0x01
#define BUS_VOLTAGE_REG     0x02
#define POWER_REG           0x03
#define CURRENT_REG         0x04
#define CALIBRATION_REG     0x05

//--------------------------------------------------
// CONFIG_REG 部分

/* Bit15 - Reset */
#define CONFIG_RESET            (1 << 15)

/* 配置1 Bit13 - Bus Voltage Range (BRNG) */
#define BVR_16V                 (0 << 13)    // 0 = 16V Range
#define BVR_32V                 (1 << 13)    // 1 = 32V Range

/* 配置2 Bit12-11 - PGA Gain / Shunt Voltage Range (PG)
 * Gain=1 (±40mV), Gain=1/2 (±80mV), Gain=1/4 (±160mV), Gain=1/8 (±320mV)
 */
#define GAIN_40MV               (0 << 11)
#define GAIN_80MV               (1 << 11)
#define GAIN_160MV              (2 << 11)
#define GAIN_320MV              (3 << 11)

/* 配置3 Bit10-7 - Bus ADC Resolution/Averaging (BADC)
 * Conversion time / resolution for Bus Voltage ADC
 */
#define BADC_9BIT               (0 << 7)
#define BADC_10BIT              (1 << 7)
#define BADC_11BIT              (2 << 7)
#define BADC_12BIT              (3 << 7)
#define BADC_12BIT_2SAMPLES     (9 << 7)
#define BADC_12BIT_4SAMPLES     (10 << 7)
#define BADC_12BIT_8SAMPLES     (11 << 7)
#define BADC_12BIT_16SAMPLES    (12 << 7)
#define BADC_12BIT_32SAMPLES    (13 << 7)
#define BADC_12BIT_64SAMPLES    (14 << 7)
#define BADC_12BIT_128SAMPLES   (15 << 7)

/* 配置4 Bit6-3 - Shunt ADC Resolution/Averaging (SADC)
 * Conversion time / resolution for Shunt Voltage ADC
 */
#define SADC_9BIT               (0 << 3)
#define SADC_10BIT              (1 << 3)
#define SADC_11BIT              (2 << 3)
#define SADC_12BIT              (3 << 3)
#define SADC_12BIT_2SAMPLES     (9 << 3)
#define SADC_12BIT_4SAMPLES     (10 << 3)
#define SADC_12BIT_8SAMPLES     (11 << 3)
#define SADC_12BIT_16SAMPLES    (12 << 3)
#define SADC_12BIT_32SAMPLES    (13 << 3)
#define SADC_12BIT_64SAMPLES    (14 << 3)
#define SADC_12BIT_128SAMPLES   (15 << 3)

/* 配置5 Bit2-0 - Operating Mode (MODE) */
#define MODE_POWER_DOWN         0x00
#define MODE_SHUNT_TRIGGERED    0x01
#define MODE_BUS_TRIGGERED      0x02
#define MODE_SHUNT_BUS_TRIGGERED 0x03
#define MODE_ADC_OFF            0x04
#define MODE_SHUNT_CONTINUOUS   0x05
#define MODE_BUS_CONTINUOUS     0x06
#define MODE_SHUNT_BUS_CONTINUOUS 0x07

//--------------------------------------------------
extern I2C_HandleTypeDef INA219_I2C_PORT;

//--------------------------------------------------
void INA219_Init(void);
void INA219_setConfig(uint16_t config);
void INA219_setCalibration(uint16_t calibration);
double INA219_getBusVoltage(void);
double INA219_getCurrent(void);
double INA219_getShuntVoltage(void);
double INA219_getPower(void);
double INA219_getBusVoltage_IT(void);
double INA219_getCurrent_IT(void);
double INA219_getShuntVoltage_IT(void);
double INA219_getPower_IT(void);
double INA219_getBusVoltage_DMA(void);
double INA219_getCurrent_DMA(void);
double INA219_getShuntVoltage_DMA(void);
double INA219_getPower_DMA(void);
#endif
#endif //INA219_H
