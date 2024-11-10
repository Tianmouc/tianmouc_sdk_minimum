#ifndef TIANMOUC_IO_H
#define TIANMOUC_IO_H

#include <cerrno>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#	include <algorithm>
#	include <vector>
#	include <utility>
#	include <memory>
#	include <string>
#	include <stdexcept>
#	include <type_traits>

#include <iostream>
#include <fstream>
#include <lz4.h>
#include <lz4frame.h>
#include <lz4hc.h>
#include <zstd.h>
#include "hardware_def.h"
#ifndef LZ4F_HEADER_SIZE_MAX
#	define LZ4F_HEADER_SIZE_MAX 19
#endif

#ifndef ZSTD_CLEVEL_DEFAULT
#	define ZSTD_CLEVEL_DEFAULT 3
#endif

namespace tianmouc{
    namespace io{
    #define max_original_one_frame_size
    #define tmc_header_size_int 16
    #define tmc_header_size_byte tmc_header_size_int * 4
    // Compression options!
    enum class CompressionType : int32_t {
        NONE      = 0,
        LZ4       = 1,
        LZ4_HIGH  = 2,
        ZSTD      = 3,
        ZSTD_HIGH = 4,
        MIN       = NONE,
        MAX       = ZSTD_HIGH
    };

    inline const CompressionType (&EnumValuesCompressionType())[5] {
        static const CompressionType values[] = {CompressionType::NONE, CompressionType::LZ4, CompressionType::LZ4_HIGH,
            CompressionType::ZSTD, CompressionType::ZSTD_HIGH};
        return values;
    }

    inline const char *const *EnumNamesCompressionType() {
        static const char *const names[] = {"NONE", "LZ4", "LZ4_HIGH", "ZSTD", "ZSTD_HIGH", nullptr};
        return names;
    }

    inline const char *EnumNameCompressionType(CompressionType e) {
        if (e < CompressionType::NONE || e > CompressionType::ZSTD_HIGH)
            return "";
        const size_t index = static_cast<int>(e);
        return EnumNamesCompressionType()[index];
    }

    static constexpr LZ4F_preferences_t lz4CompressionPreferences = {
	{LZ4F_max64KB, LZ4F_blockLinked, LZ4F_noContentChecksum, LZ4F_frame},
	0, /* compression level; 0 == default fast level */
	0, /* autoflush */
    };

    static constexpr LZ4F_preferences_t lz4HighCompressionPreferences = {
        {LZ4F_max64KB, LZ4F_blockLinked, LZ4F_noContentChecksum, LZ4F_frame},
        9, /* compression level; 9 == default HC level */
        0, /* autoflush */
    };
    static constexpr size_t lz4CompressionChunkSize = 64 * 1024;

    struct lz4CompressionSupport {
        std::shared_ptr<struct LZ4F_cctx_s> context;
        const LZ4F_preferences_t *prefs;
        size_t chunkSize;
        size_t endSize;
    };

    struct lz4DecompressionSupport {
        std::shared_ptr<struct LZ4F_dctx_s> context;
    };

    struct zstdCompressionSupport {
        std::shared_ptr<struct ZSTD_CCtx_s> context;
        int level;
    };

    struct zstdDecompressionSupport {
        std::shared_ptr<struct ZSTD_DCtx_s> context;
    };

    static inline std::string to_upper_copy_self(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
            return std::toupper(c);
        });
        return result;
    }
    static inline CompressionType parseCompressionTypeFromString(const std::string &str) {
        // Convert to upper-case to ensure valid comparison with enum names.
        const auto upperStr = to_upper_copy_self(str);

        for (size_t i = 0; EnumNamesCompressionType()[i] != nullptr; i++) {
            if (upperStr == EnumNamesCompressionType()[i]) {
                return (EnumValuesCompressionType()[i]);
            }
        }

        return (CompressionType::NONE);
    }
     }
}

#endif