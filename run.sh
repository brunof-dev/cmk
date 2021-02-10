#!/bin/bash

source env.sh
bin/tracker --video data/MOT17-09.webm --xml data/model.xml --bin data/model.bin --cmk --kalman
