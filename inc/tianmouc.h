#ifndef TIANMOUC_H
#define TIANMOUC_H
#ifdef USB_SDK

#else
#include "tmc_cop.h"
#include "tmc_aop.h"
#include "tmc_v1.h"
#endif
/************ Tianmouc Reader FLAG/Error Code************************/


extern std::mutex g_mutex_pcie;
extern std::mutex g_mutex_usb;
extern int _init_dma_status;
#endif