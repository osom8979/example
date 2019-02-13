#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import math
import json

import tensorflow as tf
import numpy as np
import skimage.io
import mask

from flask import Flask, request


APP_NAME = 'Mask'
ME_DEFAULT = 0

app = Flask(APP_NAME)
app.config['MAX_CONTENT_LENGTH'] = (3*1920*1080)+(1024*1024)

model = None
graph = None


def get_json_response(source_name, width, height, pixel, boxes, masks, class_ids):
    N = boxes.shape[0] # Number of instances
    if not N:
        print('*** No instances to display ***\n')
    else:
        assert boxes.shape[0] == masks.shape[-1] == class_ids.shape[0]

    pos_list = list()

    for i in range(N):
        class_id = class_ids[i]
        if class_id == mask.CAR_NUM:
            class_name = 'car'
        elif class_id == mask.BUS_NUM:
            class_name = 'bus'
        elif class_id == mask.TRUCK_NUM:
            class_name = 'truck'
        else:
            continue

        # Bounding Box.
        if not np.any(boxes[i]):
            # Skip this instance. Has no bbox. Likely lost in image cropping.
            continue

        y1, x1, y2, x2 = boxes[i]
        w = int(math.fabs(float(x2-x1)))
        h = int(math.fabs(float(y2-y1)))
        pos_list.append({'class': class_name, 'x': int(x1), 'y': int(y1), 'w': w, 'h': h})

    result = dict()
    result['code'] = 0
    result['type'] = 'detection'
    result['name'] = source_name
    result['data'] = dict()
    result['data']['width'] = width
    result['data']['height'] = height
    result['data']['pixel'] = pixel
    result['data']['list'] = pos_list
    return json.dumps(result)


@app.route('/')
def index_main():
    return '{"code":0}'


@app.route('/request', methods=['POST'])
def request_main():
    global model
    if model is None:
        model = mask.init_model()

    global graph
    if graph is None:
        graph = tf.get_default_graph()

    if request.method != 'POST':
        print('Only POST Method is allowed.')
        return '{"code":1}'

    if 'C2M-Event' not in request.headers:
        print('Not found C2M-Event header.')
        return '{"code":1}'

    if 'C2M-Protocol' not in request.headers:
        print('Not found C2M-Protocol header.')
        return '{"code":1}'

    event = int(request.headers['C2M-Event'])
    protocol = request.headers['C2M-Protocol']

    if event != ME_DEFAULT:
        print('Only ME_DEFAULT(0) Event is supported.')
        return '{"code":1}'

    info = request.files['info']
    buff = request.files['buff']

    info_content = info.read()
    buff_content = buff.read()

    info_json = json.loads(info_content)
    source_name = info_json['name']
    type_name = info_json['type']

    if type_name == 'image':
        width = info_json['data']['width']
        height = info_json['data']['height']
        pixel = info_json['data']['pixel']
        print('Request image data: {}x{}@{}'.format(width, height, pixel))

        if pixel == 'rgb24':
            channels = 3
        else:
            channels = 3

        if protocol == 'file_upload':
            print('Read image buffer')
            img = np.frombuffer(buff_content, dtype=np.uint8)
            img.shape = (width, height, channels)
        elif protocol == 'local_path':
            local_path = buff_content.decode('utf-8')
            print('Read image path: {}'.format(local_path))
            img = skimage.io.imread(local_path)

        with graph.as_default():
            try:
                results = model.detect([img], verbose=1)
                r = results[0]
                json_result = get_json_response(source_name, width, height, pixel, r['rois'], r['masks'], r['class_ids'])
                print('JSON RESULT: {}'.format(json_result))
                return json_result
            except:
                print('Unexpected error: {}'.format(sys.exc_info()[0]))
                return '{"code":1}'

    else:
        print('Unsupported type: {}'.format(type_name))
        return '{"code":1}'


if __name__ == '__main__':
    app.run(debug=False)

