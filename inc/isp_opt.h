/*!
    @Description : https://github.com/lyh983012
    @Author      : Yihan Lin
    @Date        : 2023-05-17
    @email       : 532109881@qq.com
*/

#ifndef UTILS_ISP_OPT_H
#define UTILS_ISP_OPT_H

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
#include <opencv2/core/hal/intrin.hpp>
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
    namespace isp_opt{
      
  void spatioReconstruct_opt(cv::Mat& SDL,cv::Mat& SDR, cv::Mat& dst);
  void spatioReconstruct_opt(cv::Mat& SDL,cv::Mat& SDR, cv::Mat& dst, cv::Mat& coldst,cv::Mat& rowdst);
  void spatioReconstruct_old(cv::Mat& SDL,cv::Mat& SDR, cv::Mat& dst, cv::Mat& coldst,cv::Mat& rowdst);
  void SDlSDR_2_SDxSDy_opt(cv::Mat& SDl,cv::Mat& SDr,cv::Mat& SDx,cv::Mat& SDy);
  void opticalFlowRecurrentMultipleScale_opt(cv::Mat& Ix_Mat, cv::Mat& Iy_Mat, cv::Mat& TD,cv::Mat& flowRGB,int accumulation);
  } // end of namespace isp_opt

}// end of namespace tianmouc


#endif // UTILS_ISP_H
