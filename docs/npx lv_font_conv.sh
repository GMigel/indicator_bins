npx lv_font_conv \
  --font Roboto-Regular.ttf \
  -r 0x20-0x7F,0xA0-0xFF, 0x400-0x45F \
  --size 18 \
  --format lvgl \
  --bpp 4 \
  --no-compress \
  -o lv_font_sans18_new.c
