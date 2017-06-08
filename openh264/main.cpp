
#include <iostream>
#include <wels/codec_api.h>

int runEncode(int argc, char ** argv)
{
    // Step1 :setup encoder.
    ISVCEncoder * encoder = nullptr;
    int result = WelsCreateSVCEncoder(&encoder);

    if (result != 0) {
        std::cerr << "Error: " << result << std::endl;
        return 1;
    }
    if (encoder == nullptr) {
        std::cerr << "Error encoder is nullptr.\n";
        return 1;
    }

    int const DEFAULT_WIDTH  = 1920;
    int const DEFAULT_HEIGHT = 1080;
    int const DEFAULT_FPS    =   30;

    // Step2: initilize with basic parameter.
    SEncParamBase param;
    memset(&param, 0, sizeof(SEncParamBase));
    param.iUsageType     = CAMERA_VIDEO_REAL_TIME;
    param.fMaxFrameRate  = DEFAULT_FPS;
    param.iPicWidth      = DEFAULT_WIDTH;
    param.iPicHeight     = DEFAULT_HEIGHT;
    param.iTargetBitrate = 5000000;
    result = encoder->Initialize(&param);
    if (result != 0) {
        std::cerr << "Initialize error: " << result << std::endl;
        return 1;
    }

    // Step3: set option, set option during encoding process
    int level = WELS_LOG_ERROR;
    encoder->SetOption(ENCODER_OPTION_TRACE_LEVEL, &level);

    int video_format = videoFormatI420;
    encoder->SetOption(ENCODER_OPTION_DATAFORMAT, &video_format);

    // Step4: encode and store ouput bistream.
    int frameSize = DEFAULT_WIDTH * DEFAULT_HEIGHT * 3 / 2;

    //BufferedData buf;
    //buf.SetLength(frameSize);
    //ASSERT_TRUE(buf.Length() == (size_t)frameSize);

    SFrameBSInfo info;
    memset(&info, 0, sizeof (SFrameBSInfo));

    SSourcePicture pic;
    memset(&pic, 0, sizeof(SSourcePicture));
    pic.iPicWidth  = DEFAULT_WIDTH;
    pic.iPicHeight = DEFAULT_HEIGHT;
    pic.iColorFormat = videoFormatI420;
    pic.iStride[0] = pic.iPicWidth;
    pic.iStride[1] = pic.iStride[2] = pic.iPicWidth >> 1;
    //pic.pData[0] = buf.data();
    pic.pData[1] = pic.pData[0] + DEFAULT_WIDTH * DEFAULT_HEIGHT;
    pic.pData[2] = pic.pData[1] + (DEFAULT_WIDTH * DEFAULT_HEIGHT >> 2);

    //for (int num = 0; num< total_num; num++) {
    //    //prepare input data
    //    rv = encoder_->EncodeFrame (&pic, &info);
    //    ASSERT_TRUE (rv == cmResultSuccess);
    //    if (info.eFrameType != videoFrameTypeSkip && cbk != NULL) {
    //        //output bitstream
    //    }
    //}

    // Step5: teardown encoder.
    if (encoder != nullptr) {
        encoder->Uninitialize();
        WelsDestroySVCEncoder(encoder);
    }

    return 0;
}

int main(int argc, char ** argv)
{
    return runEncode(argc, argv);
}

