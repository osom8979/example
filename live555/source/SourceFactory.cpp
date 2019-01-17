/**
 * @file   SourceFactory.cpp
 * @brief  SourceFactory class implementation.
 * @author zer0
 * @date   2018-10-10
 */

#include <libc2rtsp/source/SourceFactory.hpp>
#include <libc2rtsp/source/FileReader.hpp>
#include <libc2rtsp/log/Log.hpp>

#include <libtbag/network/Uri.hpp>
#include <libtbag/string/StringUtils.hpp>

namespace libc2rtsp {
namespace source    {

TBAG_CONSTEXPR static char const * const FILE_SCHEMA = "file";

SourceFactory::SourceFactory()
{
    // EMPTY.
}

SourceFactory::~SourceFactory()
{
    // EMPTY.
}

SourceFactory::SharedReader SourceFactory::gen(std::string const & url)
{
    using namespace libtbag::network;
    using namespace libtbag::string;

    Uri const URI(url);
    auto const SCHEMA = lower(URI.getSchema());
    auto const HOST = URI.getHost();

    crLogN("SourceFactory::gen() Schema({}) Host({})", SCHEMA, HOST);
    if (SCHEMA == FILE_SCHEMA) {
        return SharedReader(new libc2rtsp::source::FileReader(HOST.c_str()));
    } else {
        crLogW("SourceFactory::gen() Not found schema: {}", url);
    }
    return SharedReader();
}

} // namespace source
} // namespace libc2rtsp

