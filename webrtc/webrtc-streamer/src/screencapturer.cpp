/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** screencapturer.cpp
**
** -------------------------------------------------------------------------*/

#ifdef USE_X11

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#include "rtc_base/logging.h"

#include "screencapturer.h"

void DesktopCapturer::OnCaptureResult(webrtc::DesktopCapturer::Result result, std::unique_ptr<webrtc::DesktopFrame> frame) {
	
	RTC_LOG(INFO) << "DesktopCapturer:OnCaptureResult";
	
	if (result == webrtc::DesktopCapturer::Result::SUCCESS) {
		int width = frame->rect().width();
		int height = frame->rect().height();
		rtc::scoped_refptr<webrtc::I420Buffer> I420buffer = webrtc::I420Buffer::Create(width, height);

		const int conversionResult = libyuv::ConvertToI420(frame->data(), frame->stride()*webrtc::DesktopFrame::kBytesPerPixel,
			I420buffer->MutableDataY(), I420buffer->StrideY(),
			I420buffer->MutableDataU(), I420buffer->StrideU(),
			I420buffer->MutableDataV(), I420buffer->StrideV(),
			0, 0,
			width, height,
			width, height,
			libyuv::kRotate0, ::libyuv::FOURCC_ARGB);									
				
		if (conversionResult >= 0) {
			webrtc::VideoFrame videoFrame(I420buffer, webrtc::VideoRotation::kVideoRotation_0, rtc::TimeMicros());
			if ( (m_height == 0) && (m_width == 0) ) {
				broadcaster_.OnFrame(videoFrame);	

			} else {
				int height = m_height;
				int width = m_width;
				if (height == 0) {
					height = (videoFrame.height() * width) / videoFrame.width();
				}
				else if (width == 0) {
					width = (videoFrame.width() * height) / videoFrame.height();
				}
				int stride_y = width;
				int stride_uv = (width + 1) / 2;
				rtc::scoped_refptr<webrtc::I420Buffer> scaled_buffer = webrtc::I420Buffer::Create(width, height, stride_y, stride_uv, stride_uv);
				scaled_buffer->ScaleFrom(*videoFrame.video_frame_buffer()->ToI420());
				webrtc::VideoFrame frame = webrtc::VideoFrame(scaled_buffer, webrtc::kVideoRotation_0, rtc::TimeMicros());
						
				broadcaster_.OnFrame(videoFrame);
			}
		} else {
			RTC_LOG(LS_ERROR) << "DesktopCapturer:OnCaptureResult conversion error:" << conversionResult;
		}				

	} else {
		RTC_LOG(LS_ERROR) << "DesktopCapturer:OnCaptureResult capture error:" << (int)result;
	}
}
		
void DesktopCapturer::Run() {
	RTC_LOG(INFO) << "DesktopCapturer:Run start";
	while (IsRunning()) {
		m_capturer->CaptureFrame();
	}
	RTC_LOG(INFO) << "DesktopCapturer:Run exit";
}
bool DesktopCapturer::Start() {
	m_isrunning = true;
	rtc::Thread::Start();
	m_capturer->Start(this);
	return true;
}
		
void DesktopCapturer::Stop() {
	m_isrunning = false;
	rtc::Thread::Stop();
}
		
#endif

