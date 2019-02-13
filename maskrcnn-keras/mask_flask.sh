#!/usr/bin/env bash

# Install:
# https://github.com/matterport/Mask_RCNN

export LD_LIBRARY_PATH=/usr/local/cuda-9.0/lib64
export FLASK_ENV=development
export FLASK_APP=mask_flask.py
export FLASK_RUN_PORT=5000

EXIT_ALERT_FILE=alert.log
EXIT_ALERT_EMAIL=

while true; do
    opy3-flask run
    echo "Flask alert: $(date)" >> $EXIT_ALERT_FILE
    if [[ "$EXIT_ALERT_EMAIL" != "" ]]; then
        echo "Flask alert: $(date)" | mail "$EXIT_ALERT_EMAIL"
    fi
done

