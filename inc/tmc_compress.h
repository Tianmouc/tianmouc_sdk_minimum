#ifndef TIANMOUC_COMPRESS_H
#define TIANMOUC_COMPRESS_H

#include "tmc_io.hpp"
#include <opencv2/opencv.hpp>
namespace tianmouc{
    namespace io{
        class OutputEncoder{
            private:
            	/// Compression type.
                CompressionType compressionType;
                /// Support LZ4 compression.
                lz4CompressionSupport compressionLZ4;
                /// Support LZ4 compression.
                zstdCompressionSupport compressionZstd;
            public:
                
                CompressionType getCompressionType() {
                    return (compressionType);
                }
                //std::shared_ptr<const OutputData> processPacket();




        };
        const int zero_grp_size = 5;
        std::vector<unsigned char> simple_compress(CompressionType type,int *pvalue, int byte_len);

        std::vector<unsigned char> simple_decompress(std::vector<unsigned char> compressedData);

        std::vector<uint32_t > rod_adaptive_encoder(cv::Mat diff, int byte_len, int* header, int diff_type);

        int rod_adaptive_decoder(cv::Mat& diff, std::vector<uint32_t> packets);
    }
}

#endif