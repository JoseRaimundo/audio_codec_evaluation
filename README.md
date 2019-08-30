#### Teste


Sender

> ffmpeg -re -f mp3 -i test.mp3 -acodec libmp3lame -ar 8000 -f mulaw -f rtp rtp://127.0.0.1:1234



Reciver


> ffplay rtp://127.0.0.1:1234