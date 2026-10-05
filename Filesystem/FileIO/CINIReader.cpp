/**
 * @file CINIReader.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-09-12
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "CINIReader.h"

namespace sys
    {
    CINIReader::CINIReader() noexcept
        : m_sections{},
        m_sectionCount(0U)
    {
        clear();
    }

    CINIReader::~CINIReader() noexcept
    {
    }

    void CINIReader::clear() noexcept
    {
        std::size_t sectionIndex = 0U;

        m_sectionCount = 0U;

        while (sectionIndex < MAX_SECTION_COUNT)
        {
            Section& section = m_sections[sectionIndex];

            section.used = false;

            clearBuffer(section.name,
                        MAX_SECTION_NAME);

            std::size_t keyIndex = 0U;

            while (keyIndex < MAX_KEY_COUNT)
            {
                KeyValue& keyValue =
                    section.keyValues[keyIndex];

                keyValue.used = false;

                clearBuffer(keyValue.key,
                            MAX_KEY_NAME);

                clearBuffer(keyValue.value,
                            MAX_VALUE_SIZE);

                ++keyIndex;
            }

            ++sectionIndex;
        }
    }

    bool CINIReader::isSpace(char value) noexcept
    {
        return ((value == ' ') ||
                (value == '\t') ||
                (value == '\r') ||
                (value == '\n'));
    }

    bool CINIReader::isDigit(char value) noexcept
    {
        return ((value >= '0') &&
                (value <= '9'));
    }

    void CINIReader::clearBuffer(char* buffer,
                            std::size_t size) noexcept
    {
        if ((buffer == nullptr) || (size == 0U))
        {
            return;
        }

        std::size_t index = 0U;

        while (index < size)
        {
            buffer[index] = '\0';
            ++index;
        }
    }

    bool CINIReader::copyString(char* destination,
                            std::size_t destinationSize,
                            const char* source) noexcept
    {
        if ((destination == nullptr) ||
            (source == nullptr) ||
            (destinationSize == 0U))
        {
            return false;
        }

        std::size_t index = 0U;

        while ((source[index] != '\0') &&
            ((index + 1U) < destinationSize))
        {
            destination[index] = source[index];
            ++index;
        }

        if (source[index] != '\0')
        {
            destination[0U] = '\0';
            return false;
        }

        destination[index] = '\0';

        return true;
    }

    char* CINIReader::trim(char* value) noexcept
    {
        if (value == nullptr)
        {
            return nullptr;
        }

        char* begin = value;

        while (isSpace(*begin))
        {
            ++begin;
        }

        char* end = begin;

        while (*end != '\0')
        {
            ++end;
        }

        if (end != begin)
        {
            --end;

            while ((end > begin) &&
                isSpace(*end))
            {
                *end = '\0';
                --end;
            }

            if (isSpace(*end))
            {
                *end = '\0';
            }
        }

        return begin;
    }

    bool CINIReader::stringEqual(const char* lhs,
                            const char* rhs) noexcept
    {
        if ((lhs == nullptr) ||
            (rhs == nullptr))
        {
            return false;
        }

        std::size_t index = 0U;

        while ((lhs[index] != '\0') &&
            (rhs[index] != '\0'))
        {
            if (lhs[index] != rhs[index])
            {
                return false;
            }

            ++index;
        }

        return (lhs[index] == rhs[index]);
    }

    CINIReader::Section*
    CINIReader::findSection(const char* section) noexcept
    {
        if (section == nullptr)
        {
            return nullptr;
        }

        std::size_t index = 0U;

        while (index < MAX_SECTION_COUNT)
        {
            if (m_sections[index].used)
            {
                if (stringEqual(m_sections[index].name,
                                section))
                {
                    return &m_sections[index];
                }
            }

            ++index;
        }

        return nullptr;
    }

    const CINIReader::Section*
    CINIReader::findSection(const char* section) const noexcept
    {
        if (section == nullptr)
        {
            return nullptr;
        }

        std::size_t index = 0U;

        while (index < MAX_SECTION_COUNT)
        {
            if (m_sections[index].used)
            {
                if (stringEqual(m_sections[index].name,
                                section))
                {
                    return &m_sections[index];
                }
            }

            ++index;
        }

        return nullptr;
    }

    CINIReader::KeyValue*
    CINIReader::findKey(Section& section,
                    const char* key) noexcept
    {
        if (key == nullptr)
        {
            return nullptr;
        }

        std::size_t index = 0U;

        while (index < MAX_KEY_COUNT)
        {
            if (section.keyValues[index].used)
            {
                if (stringEqual(section.keyValues[index].key,
                                key))
                {
                    return &section.keyValues[index];
                }
            }

            ++index;
        }

        return nullptr;
    }

    const CINIReader::KeyValue*
    CINIReader::findKey(const Section& section,
                    const char* key) const noexcept
    {
        if (key == nullptr)
        {
            return nullptr;
        }

        std::size_t index = 0U;

        while (index < MAX_KEY_COUNT)
        {
            if (section.keyValues[index].used)
            {
                if (stringEqual(section.keyValues[index].key,
                                key))
                {
                    return &section.keyValues[index];
                }
            }

            ++index;
        }

        return nullptr;
    }

    CINIReader::Result
    CINIReader::addSection(const char* name,
                        Section*& section) noexcept
    {
        section = nullptr;

        if (name == nullptr)
        {
            return Result::INVALID_SECTION;
        }

        Section* existing = findSection(name);

        if (existing != nullptr)
        {
            section = existing;
            return Result::OK;
        }

        if (m_sectionCount >= MAX_SECTION_COUNT)
        {
            return Result::TOO_MANY_SECTIONS;
        }

        std::size_t index = 0U;

        while (index < MAX_SECTION_COUNT)
        {
            if (!m_sections[index].used)
            {
                if (!copyString(m_sections[index].name,
                                MAX_SECTION_NAME,
                                name))
                {
                    return Result::BUFFER_OVERFLOW;
                }

                m_sections[index].used = true;

                ++m_sectionCount;

                section = &m_sections[index];

                return Result::OK;
            }

            ++index;
        }

        return Result::TOO_MANY_SECTIONS;
    }

    CINIReader::Result
    CINIReader::addKey(Section& section,
                    const char* key,
                    const char* value) noexcept
    {
        if ((key == nullptr) ||
            (value == nullptr))
        {
            return Result::INVALID_VALUE;
        }

        KeyValue* existing = findKey(section,
                                    key);

        if (existing != nullptr)
        {
            if (!copyString(existing->value,
                            MAX_VALUE_SIZE,
                            value))
            {
                return Result::BUFFER_OVERFLOW;
            }

            return Result::OK;
        }

        std::size_t index = 0U;

        while (index < MAX_KEY_COUNT)
        {
            if (!section.keyValues[index].used)
            {
                if (!copyString(section.keyValues[index].key,
                                MAX_KEY_NAME,
                                key))
                {
                    return Result::BUFFER_OVERFLOW;
                }

                if (!copyString(section.keyValues[index].value,
                                MAX_VALUE_SIZE,
                                value))
                {
                    section.keyValues[index].key[0U] = '\0';

                    return Result::BUFFER_OVERFLOW;
                }

                section.keyValues[index].used = true;

                return Result::OK;
            }

            ++index;
        }

        return Result::TOO_MANY_KEYS;
    }

    CINIReader::Result
    CINIReader::parseLine(char* line,
                        Section*& currentSection) noexcept
    {
        if (line == nullptr)
        {
            return Result::INVALID_VALUE;
        }

        char* text = trim(line);

        if ((text == nullptr) ||
            (*text == '\0'))
        {
            return Result::OK;
        }

        /*
        * Comment
        */
        if ((*text == '#') ||
            (*text == ';'))
        {
            return Result::OK;
        }

        /*
        * Section
        */
        if (*text == '[')
        {
            char* close = text;

            while ((*close != '\0') &&
                (*close != ']'))
            {
                ++close;
            }

            if (*close != ']')
            {
                return Result::INVALID_SECTION;
            }

            *close = '\0';

            char* sectionName = trim(text + 1);

            if ((sectionName == nullptr) ||
                (*sectionName == '\0'))
            {
                return Result::INVALID_SECTION;
            }

            return addSection(sectionName,
                            currentSection);
        }

        /*
        * Key = Value
        */
        if (currentSection == nullptr)
        {
            return Result::INVALID_KEY;
        }

        char* equal = text;

        while ((*equal != '\0') &&
            (*equal != '='))
        {
            ++equal;
        }

        if (*equal != '=')
        {
            return Result::INVALID_KEY;
        }

        *equal = '\0';

        char* key = trim(text);
        char* value = trim(equal + 1);

        if ((key == nullptr) ||
            (*key == '\0'))
        {
            return Result::INVALID_KEY;
        }

        if (value == nullptr)
        {
            return Result::INVALID_VALUE;
        }

        return addKey(*currentSection,
                    key,
                    value);
    }

    CINIReader::Result
    CINIReader::load(const char* filename) noexcept
    {
        if (filename == nullptr)
        {
            return Result::FILE_OPEN_ERROR;
        }

        std::FILE* file = std::fopen(filename,
                                    "r");

        if (file == nullptr)
        {
            return Result::FILE_OPEN_ERROR;
        }

        clear();

        Section* currentSection = nullptr;

        char line[MAX_LINE_SIZE];

        Result result = Result::OK;

        while (std::fgets(line,
                        static_cast<int>(MAX_LINE_SIZE),
                        file) != nullptr)
        {
            result = parseLine(line,
                            currentSection);

            if (result != Result::OK)
            {
                break;
            }
        }

        if ((result == Result::OK) &&
            (std::ferror(file) != 0))
        {
            result = Result::FILE_READ_ERROR;
        }

        const int closeResult = std::fclose(file);

        if ((result == Result::OK) &&
            (closeResult != 0))
        {
            result = Result::FILE_READ_ERROR;
        }

        if (result != Result::OK)
        {
            clear();
        }

        return result;
    }

    bool CINIReader::hasSection(const char* section) const noexcept
    {
        return (findSection(section) != nullptr);
    }

    bool CINIReader::hasKey(const char* section,
                        const char* key) const noexcept
    {
        const Section* sectionPtr =
            findSection(section);

        if (sectionPtr == nullptr)
        {
            return false;
        }

        return (findKey(*sectionPtr,
                        key) != nullptr);
    }

    const char*
    CINIReader::getString(const char* section,
                        const char* key) const noexcept
    {
        const Section* sectionPtr =
            findSection(section);

        if (sectionPtr == nullptr)
        {
            return nullptr;
        }

        const KeyValue* keyPtr =
            findKey(*sectionPtr,
                    key);

        if (keyPtr == nullptr)
        {
            return nullptr;
        }

        return keyPtr->value;
    }

    bool CINIReader::parseInt32(const char* value,
                            std::int32_t& result) noexcept
    {
        if (value == nullptr)
        {
            return false;
        }

        if (*value == '\0')
        {
            return false;
        }

        std::size_t index = 0U;
        bool negative = false;

        if (value[index] == '-')
        {
            negative = true;
            ++index;
        }
        else if (value[index] == '+')
        {
            ++index;
        }
        else
        {
            /* No sign. */
        }

        if (value[index] == '\0')
        {
            return false;
        }

        std::int64_t number = 0;

        while (value[index] != '\0')
        {
            if (!isDigit(value[index]))
            {
                return false;
            }

            const std::int64_t digit =
                static_cast<std::int64_t>(
                    value[index] - '0');

            number = (number * 10) + digit;

            if (number > 2147483648LL)
            {
                return false;
            }

            ++index;
        }

        if (negative)
        {
            if (number == 2147483648LL)
            {
                result =
                    static_cast<std::int32_t>(
                        -2147483647) - 1;
            }
            else
            {
                result =
                    -static_cast<std::int32_t>(number);
            }
        }
        else
        {
            if (number > 2147483647LL)
            {
                return false;
            }

            result =
                static_cast<std::int32_t>(number);
        }

        return true;
    }

    bool CINIReader::parseInt64(const char* value,
                            std::int64_t& result) noexcept
    {
        if (value == nullptr)
        {
            return false;
        }

        if (*value == '\0')
        {
            return false;
        }

        std::size_t index = 0U;
        bool negative = false;

        if (value[index] == '-')
        {
            negative = true;
            ++index;
        }
        else if (value[index] == '+')
        {
            ++index;
        }
        else
        {
            /* No sign. */
        }

        if (value[index] == '\0')
        {
            return false;
        }

        std::uint64_t number = 0U;

        while (value[index] != '\0')
        {
            if (!isDigit(value[index]))
            {
                return false;
            }

            const std::uint64_t digit =
                static_cast<std::uint64_t>(
                    value[index] - '0');

            if (number >
                ((9223372036854775808ULL - digit) /
                10ULL))
            {
                return false;
            }

            number = (number * 10ULL) + digit;

            ++index;
        }

        if (negative)
        {
            if (number == 9223372036854775808ULL)
            {
                result =
                    static_cast<std::int64_t>(
                        -9223372036854775807LL) - 1LL;
            }
            else
            {
                result =
                    -static_cast<std::int64_t>(number);
            }
        }
        else
        {
            if (number > 9223372036854775807ULL)
            {
                return false;
            }

            result =
                static_cast<std::int64_t>(number);
        }

        return true;
    }

    bool CINIReader::parseBool(const char* value,
                            bool& result) noexcept
    {
        if (value == nullptr)
        {
            return false;
        }

        if (stringEqual(value, "true") ||
            stringEqual(value, "TRUE") ||
            stringEqual(value, "True") ||
            stringEqual(value, "1"))
        {
            result = true;
            return true;
        }

        if (stringEqual(value, "false") ||
            stringEqual(value, "FALSE") ||
            stringEqual(value, "False") ||
            stringEqual(value, "0"))
        {
            result = false;
            return true;
        }

        return false;
    }

    CINIReader::Result
    CINIReader::getInt32(const char* section,
                    const char* key,
                    std::int32_t& value) const noexcept
    {
        const char* text =
            getString(section,
                    key);

        if (text == nullptr)
        {
            return Result::NOT_FOUND;
        }

        if (!parseInt32(text,
                        value))
        {
            return Result::INVALID_NUMBER;
        }

        return Result::OK;
    }

    CINIReader::Result
    CINIReader::getInt64(const char* section,
                    const char* key,
                    std::int64_t& value) const noexcept
    {
        const char* text =
            getString(section,
                    key);

        if (text == nullptr)
        {
            return Result::NOT_FOUND;
        }

        if (!parseInt64(text,
                        value))
        {
            return Result::INVALID_NUMBER;
        }

        return Result::OK;
    }

    CINIReader::Result
    CINIReader::getBool(const char* section,
                    const char* key,
                    bool& value) const noexcept
    {
        const char* text =
            getString(section,
                    key);

        if (text == nullptr)
        {
            return Result::NOT_FOUND;
        }

        if (!parseBool(text,
                    value))
        {
            return Result::INVALID_BOOLEAN;
        }

        return Result::OK;
    }

    CINIReader::Result
    CINIReader::setString(const char* section,
                        const char* key,
                        const char* value) noexcept
    {
        if ((section == nullptr) ||
            (key == nullptr) ||
            (value == nullptr))
        {
            return Result::INVALID_VALUE;
        }

        Section* sectionPtr =
            findSection(section);

        Result result = Result::OK;

        if (sectionPtr == nullptr)
        {
            result = addSection(section,
                                sectionPtr);
        }

        if (result == Result::OK)
        {
            result = addKey(*sectionPtr,
                            key,
                            value);
        }

        return result;
    }

    CINIReader::Result
    CINIReader::setInt32(const char* section,
                    const char* key,
                    std::int32_t value) noexcept
    {
        char buffer[MAX_VALUE_SIZE];

        const int result =
            std::snprintf(buffer,
                        MAX_VALUE_SIZE,
                        "%ld",
                        static_cast<long>(value));

        if ((result < 0) ||
            (static_cast<std::size_t>(result) >=
            MAX_VALUE_SIZE))
        {
            return Result::BUFFER_OVERFLOW;
        }

        return setString(section,
                        key,
                        buffer);
    }

    CINIReader::Result
    CINIReader::setInt64(const char* section,
                    const char* key,
                    std::int64_t value) noexcept
    {
        char buffer[MAX_VALUE_SIZE];

        const int result =
            std::snprintf(buffer,
                        MAX_VALUE_SIZE,
                        "%lld",
                        static_cast<long long>(value));

        if ((result < 0) ||
            (static_cast<std::size_t>(result) >=
            MAX_VALUE_SIZE))
        {
            return Result::BUFFER_OVERFLOW;
        }

        return setString(section,
                        key,
                        buffer);
    }

    CINIReader::Result
    CINIReader::setBool(const char* section,
                    const char* key,
                    bool value) noexcept
    {
        const char* text =
            value ? "true" : "false";

        return setString(section,
                        key,
                        text);
    }

    std::size_t
    CINIReader::getSectionCount() const noexcept
    {
        return m_sectionCount;
    }

    std::size_t
    CINIReader::getKeyCount(const char* section) const noexcept
    {
        const Section* sectionPtr =
            findSection(section);

        if (sectionPtr == nullptr)
        {
            return 0U;
        }

        std::size_t count = 0U;
        std::size_t index = 0U;

        while (index < MAX_KEY_COUNT)
        {
            if (sectionPtr->keyValues[index].used)
            {
                ++count;
            }

            ++index;
        }

        return count;
    }
} /* namespace sys */ 
