#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace util
{
class ResourceManager
{
public:
    void setRootPath(const std::filesystem::path &root_path);
    const std::filesystem::path &getRootPath() const;

    std::filesystem::path resolve(const std::string_view &relative) const;
    std::string readTextFile(const std::string_view &relative) const;

private:
    std::filesystem::path root_path_;
};
} // namespace util