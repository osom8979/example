/**
 * @file   DefaultSource.cpp
 * @brief  DefaultSource class implementation.
 * @author zer0
 * @date   2018-10-01
 * @date   2018-10-04 (Rename: MemorySource -> DefaultSource)
 */

#include <libc2rtsp/server/DefaultSource.hpp>
#include <libc2rtsp/log/Log.hpp>
#include <libc2rtsp/source/SourceFactory.hpp>

#include <InputFile.hh>
#include <GroupsockHelper.hh>

#include <cassert>

namespace libc2rtsp {
namespace server    {

DefaultSource::DefaultSource(UsageEnvironment & env, char const * source_url)
        : FramedSource(env), _reader(libc2rtsp::source::SourceFactory().gen(source_url))
{
    // EMPTY.
}

DefaultSource::~DefaultSource()
{
    // EMPTY.
}

DefaultSource * DefaultSource::createNew(UsageEnvironment & env, char const * source_url)
{
    return new DefaultSource(env, source_url);
}

void DefaultSource::doGetNextFrame()
{
    auto const CODE = _reader->read(fTo, fMaxSize, &fFrameSize, &fPresentationTime);
    if (isFailure(CODE)) {
        handleClosure();
        return;
    }

    // Inform the reader that he has data:
    // To avoid possible infinite recursion, we need to return to the event loop to do this:
    nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)FramedSource::afterGetting, this);
}

void DefaultSource::doStopGettingFrames()
{
    envir().taskScheduler().unscheduleDelayedTask(nextTask());
}

} // namespace server
} // namespace libc2rtsp

