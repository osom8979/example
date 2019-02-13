#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import random
import math
import numpy as np
import skimage.io
import matplotlib
import matplotlib.pyplot as plt

# Import Mask RCNN
from mrcnn import utils
import mrcnn.model as modellib
from mrcnn import visualize

# Root directory of the project
ROOT_DIR = os.getcwd()

# Import COCO config
sys.path.append(ROOT_DIR)
import coco

# Directory to save logs and trained model
MODEL_DIR = ROOT_DIR

# Local path to trained weights file
COCO_MODEL_PATH = os.path.join(ROOT_DIR, "mask_rcnn_coco.h5")
# Download COCO trained weights from Releases if needed
if not os.path.exists(COCO_MODEL_PATH):
    utils.download_trained_weights(COCO_MODEL_PATH)

class InferenceConfig(coco.CocoConfig):
    # Set batch size to 1 since we'll be running inference on
    # one image at a time. Batch size = GPU_COUNT * IMAGES_PER_GPU
    GPU_COUNT = 1
    IMAGES_PER_GPU = 1
    DETECTION_MIN_CONFIDENCE = 0.6
    #RPN_NMS_THRESHOLD = 0.5

config = InferenceConfig()
config.display()

# Create model object in inference mode.
model = modellib.MaskRCNN(mode="inference", model_dir=MODEL_DIR, config=config)

# Load weights trained on MS-COCO
model.load_weights(COCO_MODEL_PATH, by_name=True)


# COCO Class names
# Index of the class in the list is its ID. For example, to get ID of
# the teddy bear class, use: class_names.index('teddy bear')
class_names = ['BG', 'person', 'bicycle', 'car', 'motorcycle', 'airplane',
               'bus', 'train', 'truck', 'boat', 'traffic light',
               'fire hydrant', 'stop sign', 'parking meter', 'bench', 'bird',
               'cat', 'dog', 'horse', 'sheep', 'cow', 'elephant', 'bear',
               'zebra', 'giraffe', 'backpack', 'umbrella', 'handbag', 'tie',
               'suitcase', 'frisbee', 'skis', 'snowboard', 'sports ball',
               'kite', 'baseball bat', 'baseball glove', 'skateboard',
               'surfboard', 'tennis racket', 'bottle', 'wine glass', 'cup',
               'fork', 'knife', 'spoon', 'bowl', 'banana', 'apple',
               'sandwich', 'orange', 'broccoli', 'carrot', 'hot dog', 'pizza',
               'donut', 'cake', 'chair', 'couch', 'potted plant', 'bed',
               'dining table', 'toilet', 'tv', 'laptop', 'mouse', 'remote',
               'keyboard', 'cell phone', 'microwave', 'oven', 'toaster',
               'sink', 'refrigerator', 'book', 'clock', 'vase', 'scissors',
               'teddy bear', 'hair drier', 'toothbrush']
# car: 3
# bus: 6
# truck: 8

# Load a random image from the images folder
#image = skimage.io.imread(os.path.join(ROOT_DIR, 'zz.jpg'))

# Run detection
#results = model.detect([image], verbose=1)

# Visualize results
#r = results[0]
#visualize.display_instances(image, r['rois'], r['masks'], r['class_ids'], class_names, r['scores'])

import math

def drawImage(image, boxes, masks, class_ids, class_names, scores, show_bbox=False, show_label=False, show_seg=True, show_center=True):
    N = boxes.shape[0] # Number of instances
    if not N:
        print("\n*** No instances to display *** \n")
    else:
        assert boxes.shape[0] == masks.shape[-1] == class_ids.shape[0]

    masked_image = image.astype(np.uint32).copy()

    car_count = 0
    for i in range(N):
        class_id = class_ids[i]
        if class_id != 3 and class_id != 6 and class_id != 8:
            continue

        car_count = car_count + 1

        # Bounding Box.
        if not np.any(boxes[i]):
            # Skip this instance. Has no bbox. Likely lost in image cropping.
            continue
        y1, x1, y2, x2 = boxes[i]

        if show_bbox:
            cv2.rectangle(image, (x1, y1), (x2, y2), (0, 255, 0), 1, cv2.LINE_AA)
            #p = patches.Rectangle((x1, y1), x2 - x1, y2 - y1, linewidth=2, alpha=0.7, linestyle="dashed", edgecolor=color, facecolor='none')

        if show_center:
            cv2.circle(image, (x1 + int(math.fabs(float(x2 - x1))/2), y1 + int(math.fabs(float(y2 - y1))/2)), 4, (0, 0, 255), -1)

        if show_label:
            score = scores[i] if scores is not None else None
            label = class_names[class_id]
            caption = "{} {:.3f}".format(label, score) if score else label

            font = cv2.FONT_HERSHEY_SIMPLEX
            cv2.putText(image, caption, (x1, y1 + 8), font, 1, (255, 255, 255), 1, cv2.LINE_AA)
            #ax.text(x1, y1 + 8, caption, color='w', size=11, backgroundcolor="none")

        if show_seg:
            # Mask
            mask = masks[:, :, i]
            masked_image = visualize.apply_mask(masked_image, mask, (0, 0, 0))

            # Mask Polyline
            # Pad to ensure proper polygons for masks that touch image edges.
            padded_mask = np.zeros((mask.shape[0] + 2, mask.shape[1] + 2), dtype=np.uint8)
            padded_mask[1:-1, 1:-1] = mask
            contours = visualize.find_contours(padded_mask, 0.5)
            for verts in contours:
                # Subtract the padding and flip (y, x) to (x, y)
                verts = np.fliplr(verts) - 1
                cv2.polylines(image, np.int32([verts]), True, (0, 255, 255))
                #p = visualize.Polygon(verts, facecolor="none", edgecolor=())
                #ax.add_patch(p)

    font = cv2.FONT_HERSHEY_SIMPLEX
    cv2.putText(image, 'CAR: {}'.format(car_count), (5, 32), font, 1, (0, 0, 255), 2, cv2.LINE_AA)
    pass

import numpy as np
import cv2

INPUT_VIDEO_PATH = os.path.join(ROOT_DIR, 'video1.mp4')

cap = cv2.VideoCapture(INPUT_VIDEO_PATH)
VIDEO_WIDTH = cap.get(cv2.CAP_PROP_FRAME_WIDTH)
VIDEO_HEIGHT = cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
VIDEO_FRAMES = cap.get(cv2.CAP_PROP_FRAME_COUNT)
VIDEO_FPS = cap.get(cv2.CAP_PROP_FPS)

print('Video Open: ', INPUT_VIDEO_PATH)
print('Video size: ', VIDEO_WIDTH, 'x', VIDEO_HEIGHT)
print('Video frames: ', VIDEO_FRAMES)
print('Video FPS: ', VIDEO_FPS)

FOURCC = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi', FOURCC, float(VIDEO_FPS), (int(VIDEO_WIDTH), int(VIDEO_HEIGHT)))

frame_index = 0
SKIP_PREFIX_FRAMES = 0

show_video = True

while (cap.isOpened()):
    ret, frame = cap.read()

    if (frame_index < SKIP_PREFIX_FRAMES):
        frame_index = frame_index + 1
        continue

    results = model.detect([frame], verbose=1)
    print('Detect: ', frame_index, '/', VIDEO_FRAMES)
    frame_index = frame_index + 1

    r = results[0]
    drawImage(frame, r['rois'], r['masks'], r['class_ids'], class_names, r['scores'])

    if show_video:
        cv2.imshow('preview', frame)
    out.write(frame)

    key_code = cv2.waitKey(1)
    if key_code & 0xFF == ord('q'):
        break
    elif key_code & 0xFF == ord('s'):
        show_video = True
    elif key_code & 0xFF == ord('h'):
        show_video = False

cap.release()
out.release()
cv2.destroyAllWindows()

