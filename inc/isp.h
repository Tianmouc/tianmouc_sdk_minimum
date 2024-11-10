/*!
    @Description : https://github.com/lyh983012
    @Author      : Yihan Lin
    @Date        : 2023-05-17
    @email       : 532109881@qq.com
*/

#ifndef UTILS_ISP_H
#define UTILS_ISP_H

//use opencv
#ifndef USE_OPEN_CV
#define USE_OPEN_CV
#endif
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/version.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#ifdef _CUDA_ISP
#include <cuda_op.h>
#endif

#include <stdlib.h>

#if CV_MAJOR_VERSION >= 3
#    include <opencv2/imgcodecs.hpp>
#else
#    include <opencv2/highgui/highgui.hpp>
#endif

//namespace lynisp{
namespace tianmouc{
namespace isp{
class RGBNormalizer{
    //lyh add, some default param
    public:
    RGBNormalizer(float scaling,float rm,float gm,float bm,
                  float rs,float gs,float bs):
      scaling(scaling),r_mean(rm),g_mean(gm),b_mean(bm),
      r_std(rs),g_std(gs),b_std(bs){}
      float scaling = 255.0;
      float r_mean =0.39556265;
      float g_mean =0.42169937;
      float b_mean =0.65865874;
      float r_std = 0.18127572356817837;
      float g_std = 0.261213080783536;
      float b_std = 0.6503947447629356;
};

//Bayer demosiac
cv::Mat prepocess(cv::Mat& src,int w, int h);
cv::Mat cone_isp(cv::Mat cone_raw);
bool compareFile(const int* pvalue,std::string filepath,bool isRod);
cv::Mat gray_hist_fast(cv::Mat gray_img, int gray_level = 255, int gray_max = 255);

//float AE_HistMSV(float mean_sample_value, float cur_exposure, float *err_i);
bool cone_AE_msv(cv::Mat cone_bgr, float exp_time, float gain, float* new_exp_time, float* new_gain, float* err_i);
bool prepocess(cv::Mat& src, int w, int h, std::vector<cv::Mat>& dst, cv::Mat& demosaicRaw, RGBNormalizer RGBnorm, bool useAWB=false);
void printMatrixInfo(cv::Mat& src);
void spatioReconstruct(cv::Mat& SDL,cv::Mat& SDR, cv::Mat& dst);
void temporalReconstruct(cv::Mat& TD, cv::Mat& dst);
void opticalFlowRecurrent(cv::Mat& SD1,cv::Mat& SD2,cv::Mat& TD,cv::Mat& opticalFlow3C,int accumulation);
void opticalFlowRecurrentMultipleScale(cv::Mat& SD1,cv::Mat& SD2,cv::Mat& TD,cv::Mat& opticalFlow3C,int accumulation);
void convertGraytoRGB(cv::Mat& Diff, cv::Mat& processed,int sf=4);
void convertGraytoRGB_Fast(cv::Mat& Diff, cv::Mat& processed,int sf=4,int th=1,int mode=0);
void convertGraytoRGB_SD_exchange(cv::Mat& DiffL,cv::Mat& DiffR, cv::Mat& SDuldr, cv::Mat& SDurdl, int scalingFactor=4,int th=1,int mode=0);
void denoiseAcum(cv::Mat& floatDiff, cv::Mat& denoiserAccum, int count);
void denoiseCorr(cv::Mat& dst, cv::Mat& old,int threshCorr=4);
void denoise_moving_ave(cv::Mat& Diff, std::vector<cv::Mat>& fifo, int count, int win_size);// add by taoyi
cv::Mat histogramSDTD(cv::Mat SD1,cv::Mat SD2,cv::Mat TD, int gray_level,int binScaling);
cv::Mat histogramRGB(cv::Mat &image,int color_level,int binScaling);
bool AWBeasy(std::vector<cv::Mat>& outputArray,std::vector<cv::Mat>& dst);
int easyMeanAE(cv::Mat& processed, float cur_exposure);
int easyDiffAE(cv::Mat& SD1,cv::Mat& SD2, int scaling, float cur_exposure) ;
float AE_SD_MAX(cv::Mat& sdl,cv::Mat& sdr, int scaling, float cur_exposure, float prev_exposure,float *prev_metric);
void FAST_kp(cv::Mat SDx,cv::Mat SDy,cv::Mat& Result,double k, double th);
void SDlSDR_2_SDxSDy(cv::Mat& SDl,cv::Mat& SDr,cv::Mat& SDx,cv::Mat& SDy);
void poisson_blend(cv::Mat& grayRecon, cv::Mat Ix, cv::Mat Iy, int iteration = 50);
}

}


#endif // UTILS_ISP_H
