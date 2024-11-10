#ifndef TIANMOUC_ROD
#define TIANMOUC_ROD

#include "tmc_dev.h"
extern std::mutex g_mutex_pcie;
extern std::mutex g_mutex_usb;
extern int _init_dma_status;
namespace tianmouc
{
    namespace device
    {
        class aop : public pathway{
        private:
            void wait_init_done(int rand_pattern);
            void wait_continue(uint64_t addr, uint64_t addr_next, uint prev_frm_cnt, uint64_t prev_timestamp, uint already_readout_num, uint interface, uint max_frm_threshold, int one_trans_frm);
            void write_readout_flag(uint64_t addr, uint first_frm_head);
            std::mutex aop_buf_mutex;
        public:
        aop(/* args */):pathway(){};
        aop& operator=(const aop& other);
        aop(int width,  int height, int adc_prec,
                uint platform,
                uint camera_idx, uint interface);
        ~aop();
        int aop_reader_init();
        int aop_decoder(int *pvalue, BYTE* temp_diff,  BYTE* spat_diff_left,  BYTE* spat_diff_right, int height, int width);
        //int aop_decoder(int *pvalue, cv::Mat& temp_diff, cv::Mat& spat_diff_left, cv::Mat& spat_diff_right, int height, int width);
        //uint64_t get_cop_addr_from_idx();
        void start_stream_pcie(int rand_pattern);
        void stop_stream_pcie();
        void init_data_getter(tmc_data * pdata);
        int get_data(tmc_data* pdata, int timeout);        
        
        
        };
    } // namespace rod
    
    
}


#endif
