#ifndef TIANMOUC_V1_H
#define TIANMOUC_V1_H
#include "tmc_dev.h"
#include "tmc_cop.h"
#include "tmc_aop.h"
#include "iic/tmc_iic.h"
extern std::mutex g_mutex_pcie;
extern std::mutex g_mutex_usb;
extern int _init_dma_status;
namespace tianmouc{
    namespace device
    {
        void release_dma();
                            
        class tmc_v1
        {
        private:
            /* data */
            //int _init_dma_status = -1;

            std::thread aop_stream_thread_;
            std::thread cop_stream_thread_;
            int rand_pattern;
            int iic_status;
            uint cameraConfigMode;
            int aop_adc_prec;
            uint platform;
            int write_rand_pattern(uint camera_mode, int rand_pattern);
            void fpga_sys_reset(uint camera_mode, uint aop_adc_bit_prec, uint interface);
            int fpga_sys_init(uint camera_mode, uint aop_adc_bit_prec, uint interface);
            bool start_sensor(int camera_mode);
            bool close_sensor();
            bool initIIC(bool iic_init_status);
        public:
            tmc_v1(int cop_width, int cop_height,int aop_width, int aop_height,
                         int cop_adc_prec, int aop_adc_prec,
                         uint platform, uint camera_idx, uint interface
                         ):
                         m_cop_reader(cop_width, cop_height, cop_adc_prec, platform, camera_idx, interface),
                         m_aop_reader(aop_width,  aop_height, aop_adc_prec, platform, camera_idx, interface) {
                            this->aop_adc_prec = aop_adc_prec;
                            this->cameraConfigMode = camera_idx;
                            this->platform = platform;
                         };
            ~tmc_v1();
           
            aop m_aop_reader;
            cop m_cop_reader;
            void start_stream();
            void stop_stream();
            void cam_close();
            void init_data_getter(tmc_data * pdata_cop,tmc_data * pdata_aop);
            void destruct_data_getter(tmc_data * pdata_cop,tmc_data * pdata_aop);
            void get_cop_data(tmc_data * pdata_cop, int timeout);
            void get_aop_data(tmc_data * pdata_aop, int timeout);
                //this->m_aop_reader
            bool downloadIICConfig(std::string IICconfigPath,
                            int RODADCprecision,
                            int RODINTmode,
                            int CONEINTmode,
                            int cameraConfigMode);
            bool downloadAEParams( int update_flag,
                            int AEmode,
                            float rodAEtime,
                            float coneAEtime,
                            float rodGain,
                            float coneGain,
                            uint16_t rodThreshold,
                            int RODINTmode,
                            int cameraConfigMode);
            
            bool downloadAnaRegDum(int* config);
           
            int cam_init(uint camera_mode, uint aop_adc_bit_prec, uint interface);

        };
        
        

    } // namespace tmc_v1
    
}



#endif
