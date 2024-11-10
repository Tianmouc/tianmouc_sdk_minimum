#ifndef UTILS_H
#define UTILS_H

#include <tmc_proc.h>
#ifdef USB_SDK

#else
#include <tianmouc.h>
#endif
#include <api_utils.h>
#include <topdef.h>

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sys/time.h>
#include <sys/stat.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/version.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#if CV_MAJOR_VERSION >= 3
#    include <opencv2/imgcodecs.hpp>
#else
#    include <opencv2/highgui/highgui.hpp>
#endif

using namespace std;
#ifdef USB_SDK

#else
    extern int init_dma_control(int, int);
    extern int release_dma_control(ENGINE*,int);
    extern int read_from_dma(char*, unsigned long, unsigned long);
    int read_dma_value(int addr,int bias, BYTE* pvalue,bool if_print);
    void release_dma();
#endif

bool safelyCreateFolder(const char* dir);
int getFileSize(const char* fileName);

class CameraConfig{

public:
       int      cop_width;
       int      cop_height;
       int      aop_width;
       int     aop_height;
       int     cop_adc_prec;
       int     RODADCprecision;
       int     platform;
       int     cameraConfigMode;
       uint     interface;
       uint     sensor_interface;
       void  setValue(int cop_width, int cop_height, int aop_width,int aop_height,
                      int cop_adc_prec, int RODADCprecision,
                      int platform, int cameraConfigMode,int interface){
           this->cop_width     =    cop_width  ;
           this->cop_height    =    cop_height ;
           this->aop_width     =    aop_width  ;
           this->aop_height     =   aop_height  ;
           this->cop_adc_prec    =  cop_adc_prec ;
           this->RODADCprecision    =   RODADCprecision ;
           this->platform       =   platform    ;
           this->cameraConfigMode    =   cameraConfigMode ;
           this->interface      =   interface   ;
           this->sensor_interface = interface;
       }
};

class detect_result
{
public:
    int classId;
    float confidence;
    cv::Rect_<float> box;
};

struct alignas(float) Detection{
    //center_x center_y w h
    int classId;
    float confidence;  // bbox_conf * cls_conf
    float bbox[4];
    float oldbbox[4];
};


template <class T>
class DataNode{
    public:
        DataNode<T>();
        ~DataNode();

        void copyData(const T* pvalue, int size, uint64_t timeStamp, uint64_t systemTimeStamp);
        uint64_t readData(T* pvalue, uint64_t* systemTimeStamp);//return timestamps
        void copyData(const T* pvalue, int size, uint64_t timeStamp,uchar* psrc1,uchar* psrc2, uint64_t systemTimeStamp);
        uint64_t readData(T* pvalue, uchar* psrc1,uchar* psrc2, uint64_t* systemTimeStamp);//return timestamps
        void init(bool isRod);
        bool hasbeenRead();
        int getDataSize(){return this->dataSize;}

    private:
        T* pvalue;
        uchar* psrc1;
        uchar* psrc2;
        uint64_t timeStamp;
        uint64_t systemTimeStamp;
        bool isRod;
        bool Read;
        int MAXLEN;
        int dataSize=-1;

};

/******************************************************************************
 * Data Node
*****************************************************************************/
template <class T>
DataNode<T>::DataNode(){
    this->timeStamp = TIMESTAMP_INVALID;
    this->isRod = true;
    this->Read = true;
    this->MAXLEN = 0;
    if(std::is_same<T,uchar>::value){
        this->MAXLEN  = SINGLE_FILE_SIZE * PCIE_SEND_ROD_BATCH_SIZE * 5;
        this->pvalue = (T*)new int[this->MAXLEN];
    }
    if(std::is_same<T,int>::value){
        this->MAXLEN  = SINGLE_FILE_SIZE * PCIE_SEND_ROD_BATCH_SIZE * 5;
        this->pvalue = (T*)new int[this->MAXLEN];
    }
    if(std::is_same<T,Detection>::value){
        this->MAXLEN = 1000;
        this->pvalue = (T*)new Detection[this->MAXLEN];
        this->psrc1 = new uchar[IMGSIZE];
        this->psrc2 = new uchar[IMGSIZE];
    }

}

template <class T>
DataNode<T>::~DataNode(){
    free(this->pvalue);
    if(std::is_same<T,Detection>::value){
        free(this->psrc1);
        free(this->psrc2);
    }
}

template <class T>
void DataNode<T>::copyData(const T* pvalue, int size, uint64_t timeStamp, uint64_t systemTimeStamp){
    if (size > this->MAXLEN){
        if(ENABAL_WARINING) std::cout<<">>>>>>>>>[Fatal Error]: witre too much data per buffering for class:"<< sizeof(T)<<std::endl;
        memcpy(this->pvalue,pvalue,this->MAXLEN * sizeof(T));
        this->timeStamp = timeStamp;
        this->Read = false;
        this->dataSize = size;
        return;
    }
    memcpy(this->pvalue,pvalue,size*sizeof(T));
    this->timeStamp = timeStamp;
    this->systemTimeStamp = systemTimeStamp;
    this->Read = false;
    this->dataSize = size;
}

template <class T>
void DataNode<T>::copyData(const T* pvalue, int size, uint64_t timeStamp, uchar* psrc1, uchar*psrc2, uint64_t systemTimeStamp){
    if (size > this->MAXLEN){
        if(ENABAL_WARINING) std::cout<<">>>>>>>>>[Fatal Error]: witre too much data per buffering for CONE"<<std::endl;
        memcpy(this->pvalue,pvalue,this->MAXLEN * sizeof(T));
        this->timeStamp = timeStamp;
        this->Read = false;
        this->dataSize = size;
         return;
    }
    memcpy(this->pvalue,pvalue,size*sizeof(T));
    memcpy(this->psrc1,psrc1,IMGSIZE * sizeof(uchar));
    memcpy(this->psrc2,psrc2,IMGSIZE * sizeof(uchar));
    this->timeStamp = timeStamp;
    this->systemTimeStamp = systemTimeStamp;
    this->Read = false;
    this->dataSize = size;

}

template <class T>
uint64_t DataNode<T>::readData(T* pvalue, uint64_t* systemTimeStamp){

    if(this->dataSize>0){
        memcpy(pvalue,this->pvalue,this->dataSize*sizeof(T));
        this->Read = true;
        uint64_t tmpTstemp = this->timeStamp;
        this->timeStamp = TIMESTAMP_INVALID;//reinit but not released;
        if (systemTimeStamp){
            *systemTimeStamp = this->systemTimeStamp;
        }
        this->systemTimeStamp=TIMESTAMP_INVALID;
        this->dataSize = -1;
        return tmpTstemp;
    }else{
        return TIMESTAMP_INVALID;
    }
}

template <class T>
uint64_t DataNode<T>::readData(T* pvalue, uchar* psrc1, uchar*psrc2, uint64_t* systemTimeStamp){
    if(this->dataSize>0){
        memcpy(pvalue,this->pvalue,dataSize*sizeof(T));
        memcpy(psrc1,this->psrc1,IMGSIZE * sizeof(uchar));
        memcpy(psrc2,this->psrc2,IMGSIZE * sizeof(uchar));
        this->Read = true;
        uint64_t tmpTstemp = this->timeStamp;
        this->timeStamp = TIMESTAMP_INVALID;//reinit but not released;
        if (systemTimeStamp){
            *systemTimeStamp = this->systemTimeStamp;
        }
        this->systemTimeStamp=TIMESTAMP_INVALID;
        this->dataSize = -1;
        return tmpTstemp;
    }else{
        return TIMESTAMP_INVALID;
    }
}

template <class T>
void DataNode<T>::init(bool isRod){
    this->isRod = isRod;
    this->Read = true;
    this->timeStamp = TIMESTAMP_INVALID;
    this->dataSize = -1;
}

template <class T>
bool DataNode<T>::hasbeenRead(){
    return Read;
}


template <class T>
class TwoStramQueue{
private:
    int rod_readpt;
    int rod_writept;
    int cone_readpt;
    int cone_writept;
    int rodSize =-1;//init when read first frame
    int coneSize =-1;//init when read first frame
    uint64_t rodFirstTimeStamp = TIMESTAMP_INVALID;
    uint64_t coneFirstTimeStamp = TIMESTAMP_INVALID;
    uint64_t _read_time_rod = TIMESTAMP_INVALID;
    bool first_batch_ready;
    DataNode<T>* rodBuffer;
    DataNode<T>* coneBuffer;
    int maxLenR = 0;
    int maxLenC = 0;

public:
    TwoStramQueue<T>(int l1=MAX_QUEUE_LEN_ROD,int l2=MAX_QUEUE_LEN_CONE);
    ~TwoStramQueue();
    bool writeRodData(T* pvalue, int size, uint64_t timestamp, uint64_t systemStamp=TIMESTAMP_INVALID);
    bool writeConeData(T* pvalue, int size, uint64_t timestamp, uint64_t systemStamp=TIMESTAMP_INVALID);
    bool writeConeData(T* pvalue, int size, uint64_t timestamp,uchar* pSrc1,uchar* pSrc2, uint64_t systemStamp=TIMESTAMP_INVALID);
    uint64_t ReadRodData(T* pvalue,bool debug=false, uint64_t* systemStamp=NULL);
    uint64_t ReadConeData(T* pvalue,bool debug=false, uint64_t* systemStamp=NULL);
    uint64_t ReadConeData(T* pvalue, uchar* pSrc1,uchar* pSrc2, uint64_t* systemStamp=NULL);
    bool checkFirstBatch();
    void reset();
    int GetRodSize(){return this->rodBuffer[rod_readpt].getDataSize();}
    int GetConeSize(){return this->coneBuffer[cone_readpt].getDataSize();}
    //TODO: rodsize should be read out from data node
};



/******************************************************************************
 *TwoStramQueue
*****************************************************************************/
template <class T>
TwoStramQueue<T>::TwoStramQueue(int l1,int l2){
    this->rod_readpt = 0;
    this->rod_writept = 0;
    this->cone_readpt = 0;
    this->cone_writept = 0;
    this->rodFirstTimeStamp = TIMESTAMP_INVALID;
    this->coneFirstTimeStamp = TIMESTAMP_INVALID;
    this->first_batch_ready = false;
    maxLenR = MAX_QUEUE_LEN_ROD;
    maxLenC = MAX_QUEUE_LEN_CONE;
    this->rodBuffer = new DataNode<T>[maxLenR];
    this->coneBuffer = new DataNode<T>[maxLenC];
    for(int i =0;i<maxLenR;i++){
        this->rodBuffer[i].init(true);
    }
    for(int i =0;i<maxLenC;i++){
        this->coneBuffer[i].init(false);
    }

}

template <class T>
void TwoStramQueue<T>::reset(){
    this->rod_readpt = 0;
    this->rod_writept = 0;
    this->cone_readpt = 0;
    this->cone_writept = 0;
    this->rodFirstTimeStamp = TIMESTAMP_INVALID;
    this->coneFirstTimeStamp = TIMESTAMP_INVALID;
    this->first_batch_ready = false;
    for(int i =0;i<maxLenR;i++){
        this->rodBuffer[i].init(true);
    }
    for(int i =0;i<maxLenC;i++){
        this->coneBuffer[i].init(false);
    }
}

template <class T>
bool TwoStramQueue<T>::writeRodData(T* pvalue, int size, uint64_t timestamp, uint64_t systemTimeStamp){
    if(!this->rodBuffer[this->rod_writept].hasbeenRead()){
        if(std::is_same<T,int>::value){
            if(ENABAL_WARINING)std::cout<<">>>>>>>>>[Fatal Error]: ROD raw data is lost"<<std::endl;
        }else{
            if(ENABAL_WARINING)std::cout<<">>>>>>>>>[Warning]: some rod det result is lost"<<std::endl;
        }
        return false;
    }
    if(rodFirstTimeStamp==TIMESTAMP_INVALID) rodFirstTimeStamp = timestamp;
    this->rodBuffer[this->rod_writept].copyData(pvalue,size,timestamp,systemTimeStamp);
    this->rod_writept++;
    if(this->rod_writept==maxLenR){
        this->rod_writept = 0;
    }
    return true;
}

template <class T>
bool TwoStramQueue<T>::writeConeData(T* pvalue, int size, uint64_t timestamp, uint64_t systemTimeStamp){
    if(!this->coneBuffer[this->cone_writept].hasbeenRead()){
       if(std::is_same<T,int>::value){
           if(ENABAL_WARINING)std::cout<<">>>>>>>>>[Fatal Error]: Cone raw data is lost"<<std::endl;
       }else{
           if(ENABAL_WARINING)std::cout<<">>>>>>>>>[Warning]: some cone det result is lost"<<std::endl;
       }
       return false;
    }
    if(coneFirstTimeStamp==TIMESTAMP_INVALID) coneFirstTimeStamp = timestamp;
    this->coneBuffer[this->cone_writept].copyData(pvalue,size,timestamp,systemTimeStamp);
    this->cone_writept++;
    if(this->cone_writept==maxLenC){
        this->cone_writept = 0;
    }
    return true;
}

template <class T>
bool TwoStramQueue<T>::writeConeData(T* pvalue, int size, uint64_t timestamp,uchar* pSrc1,uchar* pSrc2, uint64_t systemTimeStamp){
    if(!this->coneBuffer[this->cone_writept].hasbeenRead()){
       if(std::is_same<T,int>::value){
           if(ENABAL_WARINING)std::cout<<">>>>>>>>>[Fatal Error]: Cone data processing is too slow"<<std::endl;
       }else{
           if(ENABAL_WARINING)std::cout<<">>>>>>>>>[Warning]: some cone det result is loss"<<std::endl;
       }
       return false;
    }
    if(coneFirstTimeStamp==TIMESTAMP_INVALID) coneFirstTimeStamp = timestamp;
    this->coneBuffer[this->cone_writept].copyData(pvalue,size,timestamp,pSrc1,pSrc2,systemTimeStamp);
    this->cone_writept++;
    if(this->cone_writept==maxLenC){
        this->cone_writept = 0;
    }
    return true;
}

template <class T>
uint64_t TwoStramQueue<T>::ReadRodData(T* pvalue,bool debug, uint64_t* systemStamp){
    if(!this->rodBuffer[rod_readpt].hasbeenRead()){
        uint64_t tsp =  this->rodBuffer[rod_readpt].readData(pvalue,systemStamp);
        if(debug)std::cout<<"TwoStramQueue::ReadRodData succeed; write:"<<rod_writept<<"--read:"<<rod_readpt<<" tsp:"<<tsp/100.0<<"ms"<<std::endl;
        _read_time_rod = tsp;
        rod_readpt++;
        if(rod_readpt==maxLenR){
            rod_readpt=0;
        }
        return tsp;
    }else{
        return TIMESTAMP_INVALID;
    }
}

template <class T>
uint64_t TwoStramQueue<T>::ReadConeData(T* pvalue,bool debug, uint64_t* systemStamp){
    if(!this->coneBuffer[cone_readpt].hasbeenRead()){
        uint64_t tsp =  this->coneBuffer[cone_readpt].readData(pvalue,systemStamp);
        if(debug)std::cout<<"TwoStramQueue::ReadConeData succeed; write:"<<cone_writept<<"--read:"<<cone_readpt<<" tsp:"<<tsp/100.0<<"ms, delta:"<<((long)_read_time_rod-(long)tsp)/100.0<<"ms"<<std::endl;
        cone_readpt++;
        if(cone_readpt==maxLenC){
            cone_readpt=0;
        }
        return tsp;
    }else{
        return TIMESTAMP_INVALID;
    }
}

template <class T>
uint64_t TwoStramQueue<T>::ReadConeData(T* pvalue,uchar* pSrc1,uchar* pSrc2, uint64_t* systemStamp){
    if(!this->coneBuffer[cone_readpt].hasbeenRead()){
        uint64_t tsp =  this->coneBuffer[cone_readpt].readData(pvalue,pSrc1,pSrc2,systemStamp);
        cone_readpt++;
        if(cone_readpt==maxLenC){
            cone_readpt=0;
        }
        return tsp;
    }else{
        return TIMESTAMP_INVALID;
    }
}

template <class T>
bool TwoStramQueue<T>::checkFirstBatch(){
    if(this->first_batch_ready){
        return first_batch_ready;
    }else if(cone_writept>0 && rod_writept>0){
            this->first_batch_ready = true;
            return this->first_batch_ready;
    }else{
        return false;
    }
}
class DMAConfig{
public:
    DMAConfig(){};//default
    DMAConfig(int mode);//default
    DMAConfig(const DMAConfig& );//deep copy
    int mode = 0;
    bool ifArgsLegal();
};



bool isFileExists(std::string name);
void load_recorded_file(string filename, std::vector<uint64_t>& readStartPtrList,std::vector<uint64_t>& readEndPtrList);

#endif // UTILS_H
