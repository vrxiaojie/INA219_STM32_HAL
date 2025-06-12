#include "ina219_conf.h"
#if ENABLE_INA219 == 1
#include "ina219.h"
#if USE_FREERTOS == 1
#include "FreeRTOS.h"
#include "semphr.h"

// 全局信号量句柄
SemaphoreHandle_t INA219_Semaphore=NULL;
#else
volatile uint8_t INA219_data_ready = 0;
#endif

volatile enum {
    INA219_IDLE,
    INA219_READ_BUS_VOLTAGE,
    INA219_READ_CURRENT,
    INA219_READ_SHUNT_VOLTAGE,
    INA219_READ_POWER
} INA219_Request = INA219_IDLE;
volatile uint8_t INA219_data[2];
volatile double INA219_last_result = 0.0;

double getDataFromIT(void)
{
#if USE_FREERTOS == 1
    if (xSemaphoreTake(INA219_Semaphore, 100) == pdTRUE) {
        return INA219_last_result;
    }
#else
    while (!INA219_data_ready) {
    }
    INA219_data_ready = 0;
#endif
    return INA219_last_result;
}
/**
 * @brief 设置INA219配置寄存器
 * @param config 配置参数。请在ina219_conf.h中修改宏INA219_CONFIG
 */
void INA219_setConfig(uint16_t config)
{
    uint8_t cfg[2] = {config/256, config%256};
    HAL_I2C_Mem_Write(&INA219_I2C_PORT, INA219_ADDR, CONFIG_REG, I2C_MEMADD_SIZE_8BIT, cfg, 2, 100);
}

/**
 * @brief 设置INA219校准值
 * @param calibration 校准值。请在ina219_conf.h中修改INA219_CURRENT_LSB、INA219_RS、INA219_CALIBRATION_CO
 */
void INA219_setCalibration(uint16_t calibration)
{
    uint8_t cal[2] = {calibration/256,calibration%256};
    HAL_I2C_Mem_Write(&INA219_I2C_PORT, INA219_ADDR, CALIBRATION_REG, I2C_MEMADD_SIZE_8BIT, cal, 2, 100);
}

/**
 * @brief 初始化INA219，请在ina219_conf.h中 "配置INA219"
 */
void INA219_Init(void)
{
#if USE_FREERTOS == 1
    if (INA219_Semaphore == NULL) {
        INA219_Semaphore = xSemaphoreCreateBinary();
    }
#endif

    INA219_setConfig(INA219_CONFIG);
    INA219_setCalibration(INA219_CALIBRATION_VALUE);
}

/**
 * @brief  获取母线电压
 * @return 母线电压值(Unit:V)
 */
double INA219_getBusVoltage(void)
{
    uint8_t data[2];
    HAL_I2C_Mem_Read(&INA219_I2C_PORT, INA219_ADDR, BUS_VOLTAGE_REG, I2C_MEMADD_SIZE_8BIT, data, 2, 100);
    uint16_t rawdata = ((data[0] << 8) | data[1])>> 3;
    return (double)rawdata * INA219_BUS_VOLT_LSB;
}

/**
 * @brief 获取采样电流
 * @return 采样电流值(Unit:A)
 */
double INA219_getCurrent(void)
{
    uint8_t data[2];
    HAL_I2C_Mem_Read(&INA219_I2C_PORT, INA219_ADDR, CURRENT_REG, I2C_MEMADD_SIZE_8BIT, data, 2, 100);
    int16_t rawdata = (int16_t)((data[0] << 8) | data[1]);
    return (double)rawdata * INA219_CURRENT_LSB;

}

/**
 * @brief 获取分流电阻上的电压
 * @return 分流电阻上的电压(Unit:V)
 */
double INA219_getShuntVoltage(void)
{
    uint8_t data[2];
    HAL_I2C_Mem_Read(&INA219_I2C_PORT, INA219_ADDR, SHUNT_VOLTAGE_REG, I2C_MEMADD_SIZE_8BIT, data, 2, 100);
    int16_t rawdata = (int16_t)((data[0] << 8) | data[1]);
    return (double)rawdata * INA219_SHUNT_VOLT_LSB;

}

/**
 * @brief 从INA219内部功率寄存器获取功率
 * @return 功率值(Unit:W)
 */
double INA219_getPower(void)
{
    uint8_t data[2];
    HAL_I2C_Mem_Read(&INA219_I2C_PORT, INA219_ADDR, POWER_REG, I2C_MEMADD_SIZE_8BIT, data, 2, 100);
    uint16_t rawdata = (data[0] << 8) | data[1];
    return (double)rawdata * 20 * INA219_CURRENT_LSB;
}

/**
 * @brief 用中断方式获取母线电压
 * @return 电压值(Unit:V)
 */
double INA219_getBusVoltage_IT(void)
{
    INA219_Request = INA219_READ_BUS_VOLTAGE;
    HAL_I2C_Mem_Read_IT(&INA219_I2C_PORT, INA219_ADDR, BUS_VOLTAGE_REG, I2C_MEMADD_SIZE_8BIT, (uint8_t *)INA219_data, 2);
    getDataFromIT();
    return -1.0;
}

/**
 * @brief 用中断方式获取采样电流
 * @return 采样电流值(Unit:A)
 */
double INA219_getCurrent_IT(void)
{
    INA219_Request = INA219_READ_CURRENT;
    HAL_I2C_Mem_Read_IT(&INA219_I2C_PORT, INA219_ADDR, CURRENT_REG, I2C_MEMADD_SIZE_8BIT, (uint8_t *)INA219_data, 2);
    getDataFromIT();
    return -1.0;
}

/**
 * @brief 用中断方式获取分流电阻上的电压
 * @return 分流电阻上的电压(Unit:V)
 */
double INA219_getShuntVoltage_IT(void)
{
    INA219_Request = INA219_READ_SHUNT_VOLTAGE;
    HAL_I2C_Mem_Read_IT(&INA219_I2C_PORT, INA219_ADDR, SHUNT_VOLTAGE_REG, I2C_MEMADD_SIZE_8BIT, (uint8_t *)INA219_data, 2);
    getDataFromIT();
    return -1.0;
}

/**
 * @brief 用中断方式从INA219内部功率寄存器获取功率
 * @return 功率值(Unit:W)
 */
double INA219_getPower_IT(void)
{
    INA219_Request = INA219_READ_POWER;
    HAL_I2C_Mem_Read_IT(&INA219_I2C_PORT, INA219_ADDR, POWER_REG, I2C_MEMADD_SIZE_8BIT, (uint8_t *)INA219_data, 2);
    getDataFromIT();
    return -1.0;
}

/**
 * @brief 用DMA方式获取母线电压
 * @return 电压值(Unit:V)
 */
double INA219_getBusVoltage_DMA(void)
{
    uint8_t data[2];
    if (HAL_I2C_Mem_Read_DMA(&INA219_I2C_PORT, INA219_ADDR, BUS_VOLTAGE_REG, I2C_MEMADD_SIZE_8BIT, data, 2) != HAL_OK)
    {
        return -1.0;
    }

#if USE_FREERTOS == 1
    if (xSemaphoreTake(INA219_Semaphore, 100) != pdTRUE)
    {
        return -2.0;
    }
#endif

    uint16_t rawdata = ((data[0] << 8) | data[1]) >> 3;
    return (double)rawdata * INA219_BUS_VOLT_LSB;
}

/**
 * @brief 用DMA方式获取采样电流
 * @return 采样电流值(Unit:A)
 */
double INA219_getCurrent_DMA(void)
{
    uint8_t data[2];
    if (HAL_I2C_Mem_Read_DMA(&INA219_I2C_PORT, INA219_ADDR, CURRENT_REG, I2C_MEMADD_SIZE_8BIT, data, 2) != HAL_OK)
    {
        return -1.0;
    }
#if USE_FREERTOS == 1
    if (xSemaphoreTake(INA219_Semaphore, 100) != pdTRUE)
    {
        return -2.0;
    }
#endif

    int16_t rawdata = (int16_t)((data[0] << 8) | data[1]);
    return (double)rawdata * INA219_CURRENT_LSB;
}

/**
 * @brief 用DMA方式获取分流电阻上的电压
 * @return 分流电阻上的电压(Unit:V)
 */
double INA219_getShuntVoltage_DMA(void)
{
    uint8_t data[2];
    if (HAL_I2C_Mem_Read_DMA(&INA219_I2C_PORT, INA219_ADDR, SHUNT_VOLTAGE_REG, I2C_MEMADD_SIZE_8BIT, data, 2) != HAL_OK)
    {
        return -1.0;
    }
#if USE_FREERTOS == 1
    if (xSemaphoreTake(INA219_Semaphore, 100) != pdTRUE)
    {
        return -2.0;
    }
#endif
    int16_t rawdata = (int16_t)((data[0] << 8) | data[1]);
    return (double)rawdata * INA219_SHUNT_VOLT_LSB;
}

/**
 * @brief 用DMA方式从INA219内部功率寄存器获取功率
 * @return 功率值(Unit:W)
 */
double INA219_getPower_DMA(void)
{
    uint8_t data[2];

    if (HAL_I2C_Mem_Read_DMA(&INA219_I2C_PORT, INA219_ADDR, POWER_REG, I2C_MEMADD_SIZE_8BIT, data, 2) != HAL_OK)
    {
        return -1.0;
    }
#if USE_FREERTOS == 1
    if (xSemaphoreTake(INA219_Semaphore, 100) != pdTRUE)
    {
        return -2.0;
    }
#endif
    uint16_t rawdata = (data[0] << 8) | data[1];
    return (double)rawdata * 20 * INA219_CURRENT_LSB;
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c == &INA219_I2C_PORT) {
#if USE_FREERTOS == 1
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
#endif

        switch(INA219_Request)
        {
            case INA219_READ_BUS_VOLTAGE:
            {
                uint16_t raw = ((INA219_data[0] << 8) | INA219_data[1]) >> 3;
                INA219_last_result = (double)raw * INA219_BUS_VOLT_LSB;
                break;
            }
            case INA219_READ_CURRENT:
            {
                int16_t raw = (int16_t)((INA219_data[0] << 8) | INA219_data[1]);
                INA219_last_result = (double)raw * INA219_CURRENT_LSB;
                break;
            }
            case INA219_READ_SHUNT_VOLTAGE:
            {
                int16_t raw = (int16_t)((INA219_data[0] << 8) | INA219_data[1]);
                INA219_last_result = (double)raw * INA219_SHUNT_VOLT_LSB;
                break;
            }
            case INA219_READ_POWER:
            {
                uint16_t raw = (INA219_data[0] << 8) | INA219_data[1];
                INA219_last_result = (double)raw * 20 * INA219_CURRENT_LSB;
                break;
            }
            default:
                break;
        }
        INA219_Request = INA219_IDLE;
#if USE_FREERTOS == 1
        xSemaphoreGiveFromISR(INA219_Semaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
#else
        INA219_data_ready = 1;
#endif
    }
}

#endif