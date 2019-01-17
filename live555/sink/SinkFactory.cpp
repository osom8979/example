/**
 * @file   SinkFactory.cpp
 * @brief  SinkFactory class implementation.
 * @author zer0
 * @date   2018-10-10
 */

#include <libc2rtsp/sink/SinkFactory.hpp>
#include <libc2rtsp/sink/FileWriter.hpp>
#include <libc2rtsp/log/Log.hpp>

#include <libtbag/network/Uri.hpp>
#include <libtbag/string/StringUtils.hpp>

namespace libc2rtsp {
namespace sink      {

TBAG_CONSTEXPR static char const * const FILE_SCHEMA = "file";

SinkFactory::SinkFactory()
{
    // EMPTY.
}

SinkFactory::~SinkFactory()
{
    // EMPTY.
}

SinkFactory::SharedWriter SinkFactory::gen(std::string const & url)
{
    using namespace libtbag::network;
    using namespace libtbag::string;

    Uri const URI(url);
    auto const SCHEMA = lower(URI.getSchema());
    auto const HOST = URI.getHost();

    crLogN("SinkFactory::gen() Schema({}) Host({})", SCHEMA, HOST);
    if (SCHEMA == FILE_SCHEMA) {
        return SharedWriter(new libc2rtsp::sink::FileWriter(HOST.c_str()));
    } else {
        crLogW("SinkFactory::gen() Not found schema: {}", url);
    }
    return SharedWriter();
}

} // namespace sink
} // namespace libc2rtsp

