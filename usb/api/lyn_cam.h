#ifndef LYN_CAM_C_H
#define LYN_CAM_C_H


typedef void* lynCameraHandle_t;

typedef enum {
    LYN_NONE  = -1,
    LYN_CONE = 1,
    LYN_ROD = 2,
} lynDataType_t;

typedef struct {

    void* data; ///< the data pointer
    unsigned int  length; ///< data size
    lynDataType_t dataType; ///<  data type
    unsigned long long timeStamp;
    unsigned long long  countDev;
    unsigned long long sysTimeStamp;
} lynFrame_t;

// //Camer Device information
// typedef struct
// {
//     char acProductSeries[32];   // 产品系列
//     char acProductName[32];     // 产品名称
//     char acDriverVersion[32];   // 驱动版本
//     char acSensorType[32];      // sensor类型
//     char acPortType[32];        // 接口类型
//     char acSn[32];              // 产品唯一序列号
//     int cam_index;             // 该型号相机在该电脑上的实例索引号，用于区分同型号多相机

// } lynCamInfo_t;

/**
 * @brief Enumerate camera 
 * 
 * @return 返回设备个数
*/
int lynCameraEnumerate();

/**
 * @brief open camera
 *
 * @param[out]  pCameraHandle camera handle
 * @return 返回-1, 不需要任何处理
 */
int lynOpenCamera(lynCameraHandle_t* pCameraHandle);

/**
 * @brief init camera device
 *
 * @param[in]  pCameraHandle camera handle
 * @param[in]  iDeviceIndex 
 * @return 是否调用成功
 */
bool lynCameraInit(lynCameraHandle_t pCameraHandle, int iDeviceIndex);

/**
 * @brief config FPGA mode
 *
 * @param[in]  pCameraHandle camera handle
 * @param[in]  adcBitPrec: 8, 4, 2
 * @param[in]  outInterface: hex format of interface: (cone << 1 ) | rod; 1 for para, 0 for lvds; 
 *                          e.g.adcBitPrec = 8bit, outInterface = 3 -> cone para, rod para (30fps + 757fps); outInterface = 0 -> cone lvds, rod lvds (30fps + 1515fps)
 * @return 是否调用成功
 */
bool lynCameraConfigFPGAMode(lynCameraHandle_t pCameraHandle, unsigned int adcBitPrec, unsigned int outInterface);


/**
 * @brief start receive data
 *
 * @param[in]  pCameraHandle camera handle
 * @return 是否调用成功
 */
bool lynStartRecvData(lynCameraHandle_t pCameraHandle);


/**
 * @brief stop receive data
 *
 * @param[in]  pCameraHandle camera handle
 * @return 是否调用成功
 */
bool lynStopRecvData(lynCameraHandle_t pCameraHandle);

/**
 * @brief 释放camera资源
 *
 * @param[in]  pCameraHandle camera handle
 * @return 是否调用成功
 */
bool lynCameraUninit(lynCameraHandle_t pCameraHandle);

bool lynCameraCloseAll();
/**
 * @brief 获取数据帧
 *
 * @param[in]  pCameraHandle camera handle
 * @param[out]  frame
 * @return 是否调用成功
 */
bool lynGetFrame(lynFrame_t* frame, lynCameraHandle_t pCameraHandle);


/**
 * @brief 归还数据buffer
 *
 * @param[in]  pCameraHandle camera handle
 * @param[in]  frame
 * @return 是否调用成功
 */
bool lynPutFrame(lynFrame_t* frame, lynCameraHandle_t pCameraHandle);

/**
 * @brief 
 * 
 */
bool lynCameraReadIIC(lynCameraHandle_t pCameraHandle, unsigned int* address_list, int addr_list_len,
                                                       unsigned int* read_data, int* read_len);
/**
 * @brief 
 * 
 */
bool lynCameraWriteIIC(lynCameraHandle_t pCameraHandle,  unsigned int* iic_cfg_list, int list_len);
//bool cameraConfig(cameraParam config, CameraHandle* pCameraHandle)
/**
 * @brief wrtie the configurations to 
 * 
 */

bool LyncamWriteByte_ONCE(lynCameraHandle_t pCameraHandle, unsigned short Address, unsigned char Value );

/**
 * @brief 按照csv寄存器表文件 配置Tianmouc芯片
 *
 * @param[in]  pCameraHandle camera handle
 * @param[in]  cfg_path csv 格式文件 (csv format register file)
 * @return 是否调用成功
 */

bool lynCameraConfigSensorFull(lynCameraHandle_t pCameraHandle, const char* cfg_path);

/**
 * @brief 配置Cone (COP) 曝光时间 (Exposure time)
 *
 * @param[in]  pCameraHandle camera handle
 * @param[in]  exp_time exposure time, the unit must be microsecond! 必须用us作为单位！
 * @return 是否调用成功
 */
bool lynCameraConfigSensorConeExp(lynCameraHandle_t pCameraHandle, float exp_time);

/**
 * @brief 配置Cone (COP) 模拟增益（Analog Gain)
 *
 * @param[in]  pCameraHandle camera handle
 * @param[in]  ana_gain Analog gain, float number
 * @return 是否调用成功
 */
bool lynCameraConfigSensorConeAnaGain(lynCameraHandle_t pCameraHandle, float ana_gain);

/**
 * @brief Rod (AOP)曝光时间 (Rod Expore time) 
 *
 * @param[in]  pCameraHandle camera handle
 * @param[in]  exp_time exposure time, the unit must be microsecond! 必须用us作为单位！
 * @param[in]  adc_bit_prec Rod adc precision
 * @param[in]  interface Interface, lvds or gpio?
 * @return 是否调用成功
 */
bool lynCameraConfigSensorRodExp(lynCameraHandle_t pCameraHandle, float exp_time, int adc_bit_prec, int interface);

/**
 * @brief 配置Rod (AOP) 模拟增益（Analog Gain)
 *
 * @param[in]  pCameraHandle camera handle
 * @param[in]  ana_gain ana_gain Analog gain, float number
 * @return 是否调用成功
 */
bool lynCameraConfigSensorRodAnaGain(lynCameraHandle_t pCameraHandle, float ana_gain);

/**
 * @brief 配置Rod (AOP) 阈值 (Threshold)
 *
 * @param[in]  pCameraHandle camera handle
 * @param[in]  threshold Threshold int number, from 0 ~ 15!
 * @return 是否调用成功
 */
bool lynCameraConfigSensorRodTh(lynCameraHandle_t pCameraHandle, unsigned short threshold);

//lyh add it, for disable the sync mode
bool lynDisableSync(lynCameraHandle_t pCameraHandle, bool disable);

bool lynCameraAutoIICStart(lynCameraHandle_t pCameraHandle);

bool lynCameraAutoIICStop(lynCameraHandle_t pCameraHandle);

bool lynCameraConfigSensorRodDCG(lynCameraHandle_t pCameraHandle, int dcg);
bool lynCameraConfigSensorConeDCG(lynCameraHandle_t pCameraHandle, int dcg);

#endif
