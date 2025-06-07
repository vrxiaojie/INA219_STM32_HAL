#include "ina219.h"
#include "ina219_conf.h"

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
    return (double)rawdata * INA219_VOLT_LSB;
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
    return (double)rawdata * 0.00001;

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