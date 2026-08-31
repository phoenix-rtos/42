docker run -it \
    --device /dev/dri:/dev/dri \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v "$(pwd)":/app \
    -w /app \
    -p 20001:20001 \
    -p 30001:30001 \
    opengl-env