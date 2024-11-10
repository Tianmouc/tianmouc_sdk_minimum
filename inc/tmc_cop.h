#ifndef TIANMOUC_CONE
#define TIANMOUC_CONE

#include "tmc_dev.h"
extern std::mutex g_mutex_pcie;
extern std::mutex g_mutex_usb;
extern int _init_dma_status;
namespace tianmouc
{
    namespace device
    {
        class cop : public pathway{
        private:
            void write_readout_flag(uint64_t addr, uint first_frm_head);
            void wait_init_done(int rand_pattern);
            void wait_continue(uint64_t addr, uint64_t addr_next, uint prev_frm_cnt, uint64_t prev_timestamp, uint already_readout_num, uint max_frm_threshold, int rand_pattern);
            
        public:
            cop(/* args */):pathway(){};
            cop(int width,  int height, int adc_prec,
                uint platform,
                uint camera_idx, uint interface);
            ~cop();
            int cop_reader_init();
            int  cop_convert(int *pvalue, DBYTE* raw, int height, int width);
            //int  cop_convert(int *pvalue, cv::Mat& raw, int raw_height, int raw_width);
           
            //uint64_t get_cop_addr_from_idx();
            void start_stream_pcie(int rand_pattern);
            void stop_stream_pcie();
            void init_data_getter(tmc_data * pdata);
            int get_data(tmc_data* pdata, int timeout);

            std::mutex cop_buf_mutex;

        };
    } // namespace cone
    
    
}

#endif
