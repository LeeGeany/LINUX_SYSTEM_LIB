/**
 * @file File.h
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __EWLIB_FILESYSTEM_FILE_CFILE_H__
#define __EWLIB_FILESYSTEM_FILE_CFILE_H__

#include "ewlib/stdC++.h"

namespace sys
{
    bool Exists(const std::filesystem::path& _path) noexcept;
    std::size_t Size(const std::filesystem::path& _path) noexcept;
    bool Copy(const std::filesystem::path& _src, const std::filesystem::path& _dst, bool _overwrite = false) noexcept;
    bool Rename(const std::filesystem::path& src, const std::filesystem::path& dst) noexcept;
    bool Remove(const std::filesystem::path& filePath) noexcept;
    std::filesystem::file_time_type LastWriteTime(const std::filesystem::path& filePath) noexcept;
    std::filesystem::perms GetPermissions(const std::filesystem::path& filePath) noexcept;
    void SetPermissions(const std::filesystem::path& filePath, std::filesystem::perms perms) noexcept;
} // namespace sys
#endif // __EWLIB_FILESYSTEM_FILE_CFILE_H__
