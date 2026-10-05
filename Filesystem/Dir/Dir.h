/**
 * @file Dir.h
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __EWLIB_FILESYTEM_DIR_CDIR_H__
#define __EWLIB_FILESYTEM_DIR_CDIR_H__

#include "ewlib/stdC++.h"

namespace sys
{
    bool isExist(std::string _path) noexcept;
    bool isDirectory(std::string _path) noexcept;
    bool CreateDirectory(std::string _path, bool recursive = false) noexcept;
    bool RemoveAll(std::string _path) noexcept;
    void ShowDirectory(std::string _path,  bool recursive = false) noexcept;
    std::filesystem::space_info Space(std::string _path);
} /* namespace sys */
#endif /* __EWLIB_FILESYTEM_DIR_CDIR_H__ */