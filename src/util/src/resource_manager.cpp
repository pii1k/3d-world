#include "resource_manager.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace util
{
void ResourceManager::setRootPath(const std::filesystem::path &root_path)
{
    root_path_ = root_path;
}

const std::filesystem::path &ResourceManager::getRootPath() const
{
    return root_path_;
}

std::filesystem::path ResourceManager::resolve(const std::string_view &relative) const
{
    std::filesystem::path path(relative);
    if (path.is_absolute())
    {
        return path;
    }
    return root_path_ / path;
}

std::string ResourceManager::readTextFile(const std::string_view &relative) const
{
    const auto path = resolve(relative);
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open resource: " + path.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
} // namespace util