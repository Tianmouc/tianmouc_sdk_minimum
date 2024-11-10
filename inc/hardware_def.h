#ifndef HARDWARE_DEF
#define HARDWARE_DEF
//#define USB_SDK //please remove this after ,,,
#define USE_OPENCV
typedef unsigned int uint;
/*-------------------------Error Codes ----------------------------------------*/

#define DECODE_SUCCESS      0
#define INIT_SUCCESS       0
#define DATA_GET_SUCEESS        0
#define ERR_INIT_FAIL       1
#define ERR_ROW_ADDR_LOSS   2
#define ERR_WRONG_ADC_PREC  3
#define ERR_WRONG_CAM_IDX   4
#define ERR_TIMEOUT         5
#define TMC_BUF_INIT        0x12345678
#define FILER_ENC_ERR       0xABCDEF12
#define IIC_INIT_DONE   0
#define IIC_NOT_INIT    -1
/*-------------------------Cone hardware definitions ----------------------------------------*/
#define CONE_ONE_FRM 0x1A000 // (400KB + 64B) * 1024 / 4;//0x19010
#define CONE_ONE_FRM_EFF_SIZE 0x19010 // (400KB + 64B) * 1024 / 4;//0x19010
#define CONE_LAST_FRM_ADDR 0x17FFE8000
#define CONE_FRM_HEAD_CNT_TS_OFFSET 4
#define CONE_FRM_HEAD_OFFSET 16
#define CONE_FRM_HEAD_ADC_PREC_OFFSET 0
#define CONE_FRM_HEAD_FrmCount_OFFSET 3
#define CONE_FRM_HEAD_TimeStampMSB_OFFSET 1
#define CONE_FRM_HEAD_TimeStampLSB_OFFSET 2
#define CONE_FRM_HEAD_READOUT_FLAG_OFFSET 0
#define CONE_PIX_NUM_MAX 102400 // 320 * 320s
#define CONE_READOUT_FLAG_MASK 0x80000000
#define CONE_FRM_DELAY 29000// us
#define FIRST_FRAME_CNT     1
#define CONE_BASE_ADDR_1 0x100000000   // CONE base address in DRAM for camera 1 in dual cam bit!
#define CONE_BRAM_BASE_ADDR_1 0x0   // CONE base address in DRAM for camera 1 in dual cam bit!
#define CONE_BASE_ADDR_2 0x140000000   // CONE base address in DRAM for camera 2 in dual cam bit!
#define CONE_BRAM_BASE_ADDR_2 0x20000000   // CONE base address in BRAM for camera 2 in dual cam bit! 
#define CONE_RAND_PATTREN_OFFSET_ADDR 0x20
#define CONE_RAND_PATTREN_OFFSET 12

#define CONE_RAND_BRAM_ADDR_1 (CONE_BRAM_BASE_ADDR_1 + CONE_RAND_PATTREN_OFFSET_ADDR)
#define CONE_RAND_BRAM_ADDR_2 (CONE_BRAM_BASE_ADDR_2 + CONE_RAND_PATTREN_OFFSET_ADDR)

// get last frame address
    // 
#define CONE_GET_LAST_FRM_ADDR(BaseAddress, Offset) (BaseAddress + Offset)


/*-------------------------Rod hardware definitions ----------------------------------------*/
#define PKT_SIZE_MAX 38400//36800

 /************   Variable definations common **************/
#define ROD_FRM_HEAD_OFFSET 16
#define ROD_FRM_HEAD_CNT_TS_OFFSET 4
#define ROD_FRM_HEAD_ADC_PREC_OFFSET 0
#define ROD_FRM_HEAD_FrmCount_OFFSET 3
#define ROD_FRM_HEAD_TimeStampMSB_OFFSET 1
#define ROD_FRM_HEAD_TimeStampLSB_OFFSET 2
#define ROD_FRM_HEAD_READOUT_FLAG_OFFSET 0
#define ROD_RAND_PATTREN_OFFSET_ADDR 0x20
#define ROD_FRM_HEAD_RAND_PATTERN_OFFSET 12
#define ROD_8B_ONE_FRM 0x9e00       // 158KB * 1024 / 4;//0x9e00
#define ROD_4B_ONE_FRM 0x4D00 //
#define ROD_2B_ONE_FRM 0x1D00
#define ROD_READOUT_FLAG_MASK 0x80000000    //Already Readout flag, set the highest bit to 1
#define FIRST_FRAME     1
#define ROD_BASE_ADDR_1 0x120000000   // ROD base address in DRAM for camera 1 in dual cam bit!
#define ROD_BASE_ADDR_2 0x160000000   // ROD base address in DRAM for camera 2 in dual cam bit
#define RDO_BRAM_BASE_ADDR_1 0x10000000
#define RDO_BRAM_BASE_ADDR_2 0x30000000

#define ROD_RAND_BRAM_ADDR_1 (RDO_BRAM_BASE_ADDR_1 + ROD_RAND_PATTREN_OFFSET_ADDR)
#define ROD_RAND_BRAM_ADDR_2 (RDO_BRAM_BASE_ADDR_2 + ROD_RAND_PATTREN_OFFSET_ADDR)
// get last frame address
    // 
#define ROD_LAST_FRM_ADDR(BaseAddress, Offset) (BaseAddress + Offset)

// Rod one frame latency(duration), in us
#define ROD_LVDS_8bit_FRM_DELAY 660 
#define ROD_LVDS_4bit_FRM_DELAY 330
#define ROD_LVDS_2bit_FRM_DELAY 100
#define ROD_PARA_8bit_FRM_DELAY 1320 
#define ROD_PARA_4bit_FRM_DELAY 660
#define ROD_PARA_2bit_FRM_DELAY 300


typedef unsigned char BYTE;
typedef char BYTE_S;
typedef unsigned short DBYTE;
typedef short DBYTE_S;

/***************FPGA def***********************/
#define SYS_STOP 0x0 // sys stop
#define SYS_RST 0x1// sys reset
#define CONE_EN 0x10 // cone enable
#define PW_ON_GPIO  0x200 //
#define SYS_MODE_REG_ADDR_1 0x60000000// Sys reg base address in DRAM for camera 1 in dual cam bit!
#define SYS_MODE_REG_ADDR_2 0x70000000// Sys reg base address in DRAM for camera 1 in dual cam bit!

/***************USB def***********************/
//Cone
#define USB_CONE_TIMESTAMP_POS_H 1
#define USB_CONE_TIMESTAMP_POS_M 2
#define USB_CONE_TIMESTAMP_POS_L 3
#define USB_CONE_COUTNER_POS_H 4
#define USB_CONE_COUTNER_POS_L 5
//Rod
#define USB_ROD_TIMESTAMP_POS_H 1
#define USB_ROD_TIMESTAMP_POS_M 2
#define USB_ROD_TIMESTAMP_POS_L 3
#define USB_ROD_COUTNER_POS_H 4
#define USB_ROD_COUTNER_POS_L 5

// #define _DEBUG_CONE false
// #define _DEBUG_ROD true
#endif
