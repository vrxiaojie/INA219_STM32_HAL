#ifndef INA219_CONF_H
#define INA219_CONF_H
//--------------------------------------------------
// 选择MCU系列 取消注释你使用的MCU

// #define STM32F0
#define STM32F1
//#define STM32F4
//#define STM32L0
//#define STM32L1
//#define STM32L4
//#define STM32F3
//#define STM32H7
//#define STM32F7
//#define STM32G0
//#define STM32C0

//--------------------------------------------------
// 如使用FREERTOS，将值设为1
#define USE_FREERTOS 1

//--------------------------------------------------
/*INA219 配置
 *ina219.h中有CONFIG_REG部分，配置1~5用 | 隔开
 */
#define INA219_CONFIG               BVR_32V|GAIN_40MV|BADC_12BIT|SADC_12BIT_8SAMPLES|MODE_SHUNT_BUS_CONTINUOUS

// 在<<1左边写7位I2C地址
#define INA219_ADDR                 (0x40<<1)

// IN219连接到的I2C端口
#define INA219_I2C_PORT             hi2c2

// 电压LSB 4mV固定值 单位V
#define INA219_VOLT_LSB             0.004

// 电流LSB 单位A
#define INA219_CURRENT_LSB          0.00001

// 采样电阻RShunt值，单位ohm
#define INA219_RS                   0.1

/*校准系数(初始值为1.00000)
 *先在采样电路中串接电流表，记录电流表显示的实际电流Ir
 *再记录从INA219得到的测量电流Is
 *则校准系数CALIBRATION_CO=Ir/Is，结果保留5位小数
 */
#define INA219_CALIBRATION_CO      1.00000
// 校准值
#define INA219_CALIBRATION_VALUE   (uint16_t)(0.04096/(INA219_CURRENT_LSB*INA219_RS)*INA219_CALIBRATION_CO)

#endif //INA219_CONF_H
