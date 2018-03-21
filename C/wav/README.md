## wav转pcm格式

ffmpeg -f s16le   -i out.pcm -ar 44100  out.wav

s16le: PCM signed 16-bit little-endian
ar: 采样频率

## fccType

ffcType 注意小写与大写

