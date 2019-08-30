#### Teste


Sender

> ffmpeg -re -f mp3 -i test.mp3 -acodec libmp3lame -ar 8000 -f mulaw -f rtp rtp://127.0.0.1:1234



> ffmpeg -re -acodec pcm_s16le -i test.wav  -f rtp rtp://127.0.0.1:1234


Reciver


> ffplay rtp://127.0.0.1:1234




Ver 

> https://trac.ffmpeg.org/wiki/audio%20types
> https://www.wowza.com/docs/how-to-restream-using-ffmpeg-with-wowza-streaming-engine