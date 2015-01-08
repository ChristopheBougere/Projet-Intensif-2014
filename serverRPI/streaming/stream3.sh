#raspivid -o - -t 0 -w 800 -h 800 | cvlc -vvv stream:///dev/stdin --sout '#rtp{sdp=rtsp://:8554/}' :demux=h264
raspivid -o - -t 0 -w 800 -h 800 | cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8554}' :demux=h264
