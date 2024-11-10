#ifndef TIANMOUC_API_H
#define TIANMOUC_API_H

#include <iostream>
#include <string>
#include <fstream>
#include "hardware_def.h"
//#include <opencv2/opencv.hpp>
#include "xilinx_pcie_xdma/tool/dmatool.h"
#include <mutex>
#include <thread>
#include <sstream>
namespace tianmouc
{
    namespace device
   {
    enum TMCDataType { COP_DATA = 1, AOP_DATA=2 };
    struct read_param{
            uint platform;//camera platform, 1 for single cam bit, 2 for dual cam bit
            uint adc_bit_prec;
            int one_frm_size;
            int one_frm_size_eff;
            int tot_frm_num_in_one_DDR_loop;
            uint frm_num_one_trans;
            uint frm_cnt; 
            uint64_t last_frm_addr;
            uint64_t size;
            uint64_t timestamp;
    };
    struct tmc_data{
        int type;
        int actutal_read_num;
        int effective_length;
        int buf_size;
        uint64_t timestamp;
        int* read_buffer;
    };

    class pathway
    {
    protected:
        /* data */
        int width;
        int height;
        int adc_prec;
        uint platform;
        uint camera_idx;
        uint interface;
        uint frm_num_one_trans;
        uint64_t ddr_base_addr;
        uint64_t bram_base_addr;
        uint frm_cnt;
        uint64_t timestamp;


        int rand_pattern;
        //int* read_buffer;
        tmc_data tdata;

        bool running_;
    public:
        read_param params;
        pathway(/* args */){};
        pathway(int width,  int height, int adc_prec,
                uint platform,
                uint camera_idx, uint interface);
        virtual ~pathway();
    };

    } // namespace device
    

} // namespace tianmouc



#endif
