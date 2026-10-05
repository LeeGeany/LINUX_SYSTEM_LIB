/**
 * @file CINIReader.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-09-12
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef __EWLIB_FILESYSTEM_FILEIO_CINIREADER_H__
#define __EWLIB_FILESYSTEM_FILEIO_CINIREADER_H__

#include "ewlib/stdC++.h"

namespace sys
{
    class CINIReader final
    {
    public:

        static constexpr std::size_t MAX_SECTION_COUNT = 32U;
        static constexpr std::size_t MAX_KEY_COUNT     = 64U;
        static constexpr std::size_t MAX_SECTION_NAME = 32U;
        static constexpr std::size_t MAX_KEY_NAME     = 32U;
        static constexpr std::size_t MAX_VALUE_SIZE   = 256U;
        static constexpr std::size_t MAX_LINE_SIZE    = 512U;

        enum class Result : std::uint8_t
        {
            OK = 0U,
            FILE_OPEN_ERROR,
            FILE_READ_ERROR,
            INVALID_SECTION,
            INVALID_KEY,
            INVALID_VALUE,
            TOO_MANY_SECTIONS,
            TOO_MANY_KEYS,
            BUFFER_OVERFLOW,
            NOT_FOUND,
            INVALID_NUMBER,
            INVALID_BOOLEAN
        };

        CINIReader() noexcept;
        ~CINIReader() noexcept;

        CINIReader(const CINIReader&) = delete;
        CINIReader& operator=(const CINIReader&) = delete;

        CINIReader(CINIReader&&) = delete;
        CINIReader& operator=(CINIReader&&) = delete;

        Result load(const char* filename) noexcept;

        void clear() noexcept;

        bool hasSection(const char* section) const noexcept;

        bool hasKey(const char* section,
                    const char* key) const noexcept;

        const char* getString(const char* section,
                            const char* key) const noexcept;

        Result getInt32(const char* section,
                        const char* key,
                        std::int32_t& value) const noexcept;

        Result getInt64(const char* section,
                        const char* key,
                        std::int64_t& value) const noexcept;

        Result getBool(const char* section,
                    const char* key,
                    bool& value) const noexcept;

        Result setString(const char* section,
                        const char* key,
                        const char* value) noexcept;

        Result setInt32(const char* section,
                        const char* key,
                        std::int32_t value) noexcept;

        Result setInt64(const char* section,
                        const char* key,
                        std::int64_t value) noexcept;

        Result setBool(const char* section,
                    const char* key,
                    bool value) noexcept;

        std::size_t getSectionCount() const noexcept;

        std::size_t getKeyCount(const char* section) const noexcept;

    private:
        struct KeyValue
        {
            bool used;
            char key[MAX_KEY_NAME];
            char value[MAX_VALUE_SIZE];
        };

        struct Section
        {
            bool used;
            char name[MAX_SECTION_NAME];
            KeyValue keyValues[MAX_KEY_COUNT];
        };

        Section m_sections[MAX_SECTION_COUNT];

        std::size_t m_sectionCount;

        static bool isSpace(char value) noexcept;

        static bool isDigit(char value) noexcept;

        static void clearBuffer(char* buffer,
                                std::size_t size) noexcept;

        static bool copyString(char* destination,
                            std::size_t destinationSize,
                            const char* source) noexcept;

        static char* trim(char* value) noexcept;

        static bool stringEqual(const char* lhs,
                                const char* rhs) noexcept;

        static bool parseInt32(const char* value,
                            std::int32_t& result) noexcept;

        static bool parseInt64(const char* value,
                            std::int64_t& result) noexcept;

        static bool parseBool(const char* value,
                            bool& result) noexcept;

        Section* findSection(const char* section) noexcept;

        const Section* findSection(const char* section) const noexcept;

        KeyValue* findKey(Section& section,
                        const char* key) noexcept;

        const KeyValue* findKey(const Section& section,
                                const char* key) const noexcept;

        Result addSection(const char* name,
                        Section*& section) noexcept;

        Result addKey(Section& section,
                    const char* key,
                    const char* value) noexcept;

        Result parseLine(char* line,
                        Section*& currentSection) noexcept;
    };
} /* namespace sys */
#endif /* __EWLIB_FILESYSTEM_FILEIO_CINIREADER_H__ */