# -*- coding: utf-8 -*-

from av import open as av_open  # noqa
from time import time
import cv2

video0 = "small.mp4"
video1 = "bus-fire.mp4"
video2 = "rtsp://192.168.0.50:554/media/1/1/Profile1"

video_src = video2
video_index = 0
frame_format = "bgr24"
frame_width = 1270
frame_height = 720
options = {
    "rtsp_transport": "udp",
    "fflags": "nobuffer"
}
container_options = {}
stream_options = []

container = av_open(video_src, options=options,
                    container_options=container_options,
                    stream_options=stream_options)
container.streams.video[video_index].thread_type = "AUTO"  # Go faster! (SLICE, AUTO, FRAME, NONE)
container.streams.video[video_index].codec_context.flags = "LOW_DELAY"
# frames = container.decode(video=video_index)

while True:
    for packet in container.demux(video=video_index):
        print(packet)
        for frame in packet.decode():
            print(frame)
            begin = time()
            last_frame = frame.to_ndarray(width=frame_width,
                                          height=frame_height,
                                          format=frame_format,
                                          interpolation="FAST_BILINEAR")
            last_index = frame.index
            last_pts = frame.pts

            end = time()
            elapsed = int((end-begin)*1000)
            print(f"index={last_index}, pts={last_pts}, shape={last_frame.shape}, elapsed={elapsed}ms")

            cv2.imshow("frame", last_frame)
            if cv2.waitKey(1) & 0xFF == ord("q"):
                break
