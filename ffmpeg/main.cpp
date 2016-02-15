
#include <cassert>

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
}

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "SharedQueue.hpp"

#define _FPS (1000 / 60)

#define _RGB_CHANNEL_SIZE  3
#define _GRAY_CHANNEL_SIZE 1

IplImage * createRgb24IplImage(uint8_t const * src, int width, int height, int linesize)
{
    uint8_t const * src_cursor = NULL;
    uint8_t * dest_cursor = NULL;

    IplImage * image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, _RGB_CHANNEL_SIZE);
    for (int y = 0; y < height; y++) {
        src_cursor = (src + y * linesize);
        dest_cursor = (uint8_t *) (image->imageData + y * image->widthStep);

        for (int x = 0; x < width; x++) {
            // BGR24 Format Copy.
            dest_cursor[3 * x + 2] = src_cursor[3 * x + 2]; // r
            dest_cursor[3 * x + 1] = src_cursor[3 * x + 1]; // g
            dest_cursor[3 * x + 0] = src_cursor[3 * x + 0]; // b
        }
    }

    return image;
}

void copyRgb24IplImage(uint8_t const * src, int width, int height, int linesize, IplImage * dest)
{
    assert(src != nullptr);
    assert(dest != nullptr);

    if (linesize == dest->widthStep && width == dest->width && height == dest->height) {
        memcpy(dest->imageData, src, linesize * height);
        return;
    }

    uint8_t const * src_cursor = NULL;
    uint8_t * dest_cursor = NULL;

    for (int y = 0; y < height; y++) {
        src_cursor = (src + y * linesize);
        dest_cursor = (uint8_t *) (dest->imageData + y * dest->widthStep);

        for (int x = 0; x < width; x++) {
            // BGR24 Format Copy.
            dest_cursor[3 * x + 2] = src_cursor[3 * x + 2]; // r
            dest_cursor[3 * x + 1] = src_cursor[3 * x + 1]; // g
            dest_cursor[3 * x + 0] = src_cursor[3 * x + 0]; // b
        }
    }
}


/**
 * A/V Decoder class prototype.
 */
class AvDecoder
{
public:
    enum class ErrorCode : int
    {
        NOTING = 0,
        ERROR_OR_EOF,
        ERROR_MISMATCH_STREAM_INDEX,
        NO_FRAME,
    };

public:
    /// @brief Not found video stream index.
    static int const NOT_FOUND_VIDEO_STREAM_INDEX;

private:
    std::size_t _stream_index;
    bool _ready;

private:
    AVFormatContext * _format_context;
    AVCodecContext * _codec_context;
    SwsContext * _scaler;

private:
    AVFrame * _original_frame;
    AVFrame * _scaled_frame;

private:
    int _output_width;
    int _output_height;
    int _output_format; ///< AVPixelFormat

public:
    AvDecoder();
    ~AvDecoder();

public:
    void set_stream_index(std::size_t const & index)
    {   _stream_index = index;  }
    std::size_t get_stream_index() const
    {   return _stream_index;   }
    bool isReady() const
    {   return _ready;          }
    AVFrame * get_original_frame() const
    {   return _original_frame; }
    AVFrame * get_scaled_frame() const
    {   return _scaled_frame;   }
    int get_output_width() const
    {   return _output_width;   }
    int get_output_height() const
    {   return _output_height;  }
    int get_output_format() const
    {   return _output_format;  }

// Buffer operators.
private:
    std::size_t _buffer_size;
    uint8_t * _buffer;

private:
    bool initBuffer(std::size_t const & size);
    void releaseBuffer();

// stream methods.
public:
    std::size_t get_stream_count() const
    {
        if (_format_context != nullptr) {
            return _format_context->nb_streams;
        }
        return 0;
    }

    AVStream * get_stream(std::size_t const & index) const
    {
        if (_format_context != nullptr || index >= _format_context->nb_streams) {
            return _format_context->streams[index];
        }
        return nullptr;
    }

public:
    std::vector<int> getIndexList(int const & av_media_type)
    {
        std::vector<int> result;
        if (_format_context == nullptr) {
            return result;
        }

        std::size_t const kStreamCount = get_stream_count();
        for (std::size_t cursor = 0; cursor < kStreamCount; ++cursor) {
            AVStream * stream = get_stream(cursor);

            if (stream != nullptr && stream->codec->codec_type == av_media_type) {
                result.push_back(cursor);
            }
        }
        return std::move(result);
    }

    std::vector<int> getVideoIndexList()
    { return getIndexList(AVMEDIA_TYPE_VIDEO); }
    std::vector<int> getAudioIndexList()
    { return getIndexList(AVMEDIA_TYPE_AUDIO); }
    std::vector<int> getSubtitleIndexList()
    { return getIndexList(AVMEDIA_TYPE_SUBTITLE); }

public:
    double getFrameRate(int const & stream_index)
    {
        AVRational frame_rate = _format_context->streams[stream_index]->r_frame_rate;
        return static_cast<double>(frame_rate.num) / static_cast<double>(frame_rate.den);
    }

    int64_t getFrameStartTime(int const & stream_index)
    {   return _format_context->streams[stream_index]->start_time;  }
    int64_t getFrameDuration(int const & stream_index)
    {   return _format_context->streams[stream_index]->duration;    }
    int64_t getFrameCount(int const & stream_index)
    {   return _format_context->streams[stream_index]->nb_frames;   }

public:
    double getFrameRate()
    {   return getFrameRate(_stream_index);         }
    int64_t getFrameStartTime()
    {   return getFrameStartTime(_stream_index);    }
    int64_t getFrameDuration()
    {   return getFrameDuration(_stream_index);     }
    int64_t getFrameCount()
    {   return getFrameCount(_stream_index);        }
    int64_t getGeneralDuration()
    {   return _format_context->duration;           }

// Scaler methods.
private:
    void set_output_info(int const & width, int const & height, int const & format);
    bool initScaler(int const & width, int const & height, int const & format);
    void releaseScaler();

// User's common operators.
public:
    bool open(const char * path);
    void close();

public:
    ErrorCode readNextFrame(int const & stream_index);
    ErrorCode readNextFrame();

private:
    SharedQueue<AVPacket> _packet_queue;
    std::thread _input_thread;
    std::thread _scale_thread;
    std::atomic_bool _exit;

public:
    void readPacket();
    void startPacketReader();
    void stopPacketReader();
};

// ---------------
// Implementation.
// ---------------

int const AvDecoder::NOT_FOUND_VIDEO_STREAM_INDEX = -1;

AvDecoder::AvDecoder()
        : _stream_index(NOT_FOUND_VIDEO_STREAM_INDEX), _ready(false)
        , _format_context(nullptr), _codec_context(nullptr), _scaler(nullptr)
        , _original_frame(nullptr), _scaled_frame(nullptr)
        , _exit(false)
{
    static std::once_flag register_flag;
    std::call_once(register_flag, [](){
                av_register_all();
                avformat_network_init();
            });

    _output_width = 0;
    _output_height = 0;
    _output_format = 0;

    // Buffer operators.
    _buffer_size = 0;
    _buffer = nullptr;
}

AvDecoder::~AvDecoder()
{
    this->stopPacketReader();
    this->close();
}

// ----------------
// Buffer operator.
// ----------------

bool AvDecoder::initBuffer(std::size_t const & size)
{
    if (size == 0) {
        return false;
    }

    _buffer = static_cast<uint8_t*>(av_malloc(size * sizeof(uint8_t)));
    _buffer_size = size;
    return true;
}

void AvDecoder::releaseBuffer()
{
    if (_buffer != nullptr) {
        av_free(_buffer);
        _buffer = nullptr;
    }
    _buffer_size = 0;
}

// ---------------
// Scaler methods.
// ---------------

void AvDecoder::set_output_info(int const & width, int const & height, int const & format)
{
    _output_width  = width;
    _output_height = height;
    _output_format = format;
}

bool AvDecoder::initScaler(int const & width, int const & height, int const & format)
{
    if (_codec_context == nullptr || width == 0 || height == 0) {
        return false;
    }

    auto const kFalseCallback = [&]() {
        set_output_info(0, 0, 0);
        releaseScaler();
    };

    set_output_info(width, height, format);

    // Find the decoder for the video stream.
    AVCodec * codec = avcodec_find_decoder(_codec_context->codec_id);
    if (codec == nullptr) {
        kFalseCallback();
        return false;
    }

    // Open codec.
    AVDictionary * options = nullptr;
    if (avcodec_open2(_codec_context, codec, &options) < 0) {
        kFalseCallback();
        return false;
    }

    int source_width = _codec_context->width;
    int source_height = _codec_context->height;
    AVPixelFormat source_format = _codec_context->pix_fmt;
    AVPixelFormat destination_format = static_cast<AVPixelFormat>(_output_format);

    int kDefaultFlags = SWS_BILINEAR;
    SwsFilter * kSrcFilter = nullptr;
    SwsFilter * kDstFilter = nullptr;
    const double * kParam = nullptr;

    _scaler = sws_getContext(source_width, source_height, source_format, _output_width,
            _output_height, static_cast<AVPixelFormat>(_output_format), kDefaultFlags, kSrcFilter,
            kDstFilter, kParam);

    // Allocate video frame
    _original_frame = av_frame_alloc();
    _scaled_frame = av_frame_alloc();

    if (_original_frame == nullptr || _scaled_frame == nullptr) {
        kFalseCallback();
        return false;
    }

    releaseBuffer();
    int picture_size = avpicture_get_size(destination_format, _output_width, _output_height);
    if (initBuffer(picture_size * 2) == true) {
        picture_size = avpicture_fill((AVPicture*)_scaled_frame, _buffer,
                destination_format, _output_width, _output_height);
    }

    if (picture_size < 0) {
        kFalseCallback();
        return false;
    }

    return true;
}

void AvDecoder::releaseScaler()
{
    if (_original_frame != nullptr) {
        av_free(_original_frame);
        _original_frame = nullptr;
    }
    if (_scaled_frame != nullptr) {
        av_free(_scaled_frame);
        _scaled_frame = nullptr;
    }
    if (_scaler != nullptr) {
        sws_freeContext(_scaler);
        _scaler = nullptr;
    }
}

// ------------------------
// User's common operators.
// ------------------------

bool AvDecoder::open(const char * path)
{
    _ready = false;
    _stream_index = NOT_FOUND_VIDEO_STREAM_INDEX;

    AVDictionary * options = nullptr;
    //av_dict_set(&options, "rtsp_transport", "tcp", 0);
    if (avformat_open_input(&_format_context, path, nullptr, &options) != 0) {
        return false;
    }
    //av_dict_free(&options);

    // Retrieve stream information.
    if (avformat_find_stream_info(_format_context, nullptr) < 0) {
        return false;
    }

    // dump information about file onto standard error.
    // av_dump_format(_format_context, 0, path, 0);

    // Find the first video stream.
    std::vector<int> video_index_list = getVideoIndexList();
    if (video_index_list.size() > 0) {
        // first stream index setting.
        _stream_index = video_index_list.front();
    } else {
        // Can't find a video stream.
        return false;
    }

    //{ // Setup CodecContext & Scaler.
    _codec_context = _format_context->streams[_stream_index]->codec;
    _ready = initScaler(_codec_context->width, _codec_context->height, PIX_FMT_BGR24);
    //}

    return _ready;
}

void AvDecoder::close()
{
    _ready = false;
    _stream_index = NOT_FOUND_VIDEO_STREAM_INDEX;

    releaseScaler();
    releaseBuffer();

    if (_codec_context != nullptr) {
        avcodec_close(_codec_context);
        _codec_context = nullptr;
    }
    if (_format_context != nullptr) {
        avformat_close_input(&_format_context);
        _format_context = nullptr;
    }
}

AvDecoder::ErrorCode AvDecoder::readNextFrame(int const & stream_index)
{
    if (_exit == true) {
        return ErrorCode::ERROR_OR_EOF;
    }

    using Packet = SharedQueue<AVPacket>::SharedType;
    Packet current = _packet_queue.front(true);
    //printf("Pop packet queue: %ld\n", _packet_queue.size());

    if (static_cast<bool>(current) == false) {
        return ErrorCode::NO_FRAME;
    }

    // Assert!
    if (stream_index != current->stream_index) {
        return ErrorCode::ERROR_MISMATCH_STREAM_INDEX;
    }

    int got_picture_ptr = 0;

    avcodec_decode_video2(_codec_context, _original_frame, &got_picture_ptr, current.get());

    if (got_picture_ptr == 0) {
        return ErrorCode::NO_FRAME;
    }

    sws_scale(_scaler, (uint8_t const * const *) _original_frame->data,
            _original_frame->linesize, 0, _codec_context->height
            , _scaled_frame->data, _scaled_frame->linesize);

    // av_free_packet(_packet.get());
    return ErrorCode::NOTING;
}

AvDecoder::ErrorCode AvDecoder::readNextFrame()
{
    return readNextFrame(_stream_index);
}

void AvDecoder::readPacket()
{
    while (_exit == false) {
        using Packet = SharedQueue<AVPacket>::SharedType;
        Packet current(new (std::nothrow) AVPacket(), [](AVPacket * packet){
            av_free_packet(packet);
        });

        if (av_read_frame(_format_context, current.get()) < 0) {
            _exit = true;
            break;
        }

        _packet_queue.push(current);
        //printf("Push packet queue: %ld\n", _packet_queue.size());

        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void AvDecoder::startPacketReader()
{
    _exit = false;
    _input_thread = std::thread(&AvDecoder::readPacket, this);
}

void AvDecoder::stopPacketReader()
{
    _exit = true;

    if (_input_thread.joinable()) {
        _input_thread.join();
    }
}

int main(int argc, char ** argv)
{
    if (argc <= 1) {
        std::cerr << "Usage: " << argv[0] << " {path|url}\n";
        return 1;
    }

    using SharedImage = std::shared_ptr<IplImage>;
    using ImageVector = std::vector<SharedImage>;

    std::string const WINDOW_TITLE = "CAMERA";
    std::string const PATH = argv[1];
    ImageVector buffer;

    AvDecoder av;
    bool is_open = av.open(PATH.c_str());
    if (is_open == false) {
        std::cerr << "Not found file: " << PATH << std::endl;
        return 1;
    }

    using ErrorCode = AvDecoder::ErrorCode;

    AVFrame * frame = nullptr;
    int width = av.get_output_width();
    int height = av.get_output_height();
    int linesize = 0;
    // int depth = 3;

    bool exit_flag = false;

    buffer.clear();

    int const PREVIEW_WIDTH  = 320;
    int const PREVIEW_HEIGHT = 240;
    cvNamedWindow(WINDOW_TITLE.c_str(), 0);
    cvResizeWindow(WINDOW_TITLE.c_str(), PREVIEW_WIDTH, PREVIEW_HEIGHT);

    SharedImage frame_image = SharedImage(cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, _RGB_CHANNEL_SIZE)
            , [](IplImage * image){
        cvReleaseImage(&image);
    });
    SharedImage preview_image = SharedImage(cvCreateImage(cvSize(PREVIEW_WIDTH, PREVIEW_HEIGHT), IPL_DEPTH_8U, _RGB_CHANNEL_SIZE)
            , [](IplImage * image){
        cvReleaseImage(&image);
    });

    av.startPacketReader();

    while (exit_flag == false) {
        //std::cout << "Read next frame.\n";
        ErrorCode code = av.readNextFrame();

        if (code == ErrorCode::NOTING) {
            frame = av.get_scaled_frame();
            linesize = frame->linesize[0];

            copyRgb24IplImage(frame->data[0], width, height, linesize, frame_image.get());

            //std::cout << "Read image #" << count << " (" << image->width << "x" << image->height << ")!\n";

            cvResize(frame_image.get(), preview_image.get());
            cvShowImage(WINDOW_TITLE.c_str(), preview_image.get());
            if (cvWaitKey(_FPS) >= 0) { // Any key.
                exit_flag = true;
            }
        } else {
            assert(code != ErrorCode::NOTING);

            std::string message;
            switch (code) {
            case ErrorCode::ERROR_MISMATCH_STREAM_INDEX:
                message = "Error mismatch stream index.";
                break;

            case ErrorCode::NO_FRAME:
                message = "No frame.";
                break;

            case ErrorCode::ERROR_OR_EOF:
                message = "Error or eof.";
                exit_flag = true;
                av.stopPacketReader();
                break;

            default:
                message = "Unknown code.";
                exit_flag = true;
                av.stopPacketReader();
                break;
            }

            std::cout << message << std::endl;
        }
    }

    av.stopPacketReader();
    av.close();
    cvDestroyWindow(WINDOW_TITLE.c_str());
    return 0;
}

