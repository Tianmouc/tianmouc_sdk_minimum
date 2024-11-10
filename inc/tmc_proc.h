#ifndef TIANMOUC_PROC_H
#define TIANMOUC_PROC_H

#ifdef USB_SDK
#else
    // #include "tmc_dev.h"
#endif
#include <opencv2/opencv.hpp>
#include "hardware_def.h"
#include <vector>
namespace tianmouc
{
    namespace process
    {
        // digital threshold filter, process AER packets directly withoud decode first
        std::vector<uint> rod_digi_th_filter(int* pvalue, int threshold);

        #ifdef USB_SDK
            int rod_decoder(int *pvalue, cv::Mat& temp_diff,  cv::Mat& spat_diff_left,  cv::Mat& spat_diff_right,
                    int height, int width, int rodsize);
            int cone_reader(int* pvalue, cv::Mat& raw, int height, int width);
            uint64_t get_rod_timestamp(int* pvalue);
            uint64_t get_cone_timestamp(int* pvalue);
            uint32_t get_rod_counter(int* pvalue);
            uint32_t get_cone_counter(int* pvalue);
        #else
            int rod_decoder(int *pvalue, BYTE* temp_diff, BYTE* spat_diff_left, BYTE* spat_diff_right, int height, int width);
            // Overload for opencv user, better performance
            int rod_decoder(int *pvalue, cv::Mat& temp_diff,  cv::Mat& spat_diff_left,  cv::Mat& spat_diff_right, int height, int width);
            int cone_reader(int *pvalue, DBYTE* raw, int height, int width);
            // Overload for opencv user, better performance
            int cone_reader(int *pvalue, cv::Mat& raw, int raw_height, int raw_width);
        #endif 
    } // namespace process
    
    
} // namespace tianmouc



#endif
