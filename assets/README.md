<p align="right">
  <a href="README.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Assets

This directory stores reusable fonts, images, music, and sound effects, organized by asset type.

Keep each asset in the matching subdirectory and document its destination, naming, integration method, and source/license. Do not mix binary assets with Markdown documentation.

## Fonts

Store reusable font files and generated font sources in `fonts/`.

| File | Coverage and integration | Source and license |
| --- | --- | --- |
| [`fonts/antonym_font_16.c`](fonts/antonym_font_16.c) | LVGL 16 px, 4 bpp, uncompressed application subset. It covers printable ASCII (`0x20-0x7E`) plus the 79 non-ASCII code points used by `main/main.c` and `main/antonym_game.c`. Compiled into the `main` component as `antonym_font_16`. | Generated with `lv_font_conv` 1.5.3 from Adobe Source Han Sans CN Regular 2.005R. The source font is SIL Open Font License 1.1; see [`fonts/LICENSE-SourceHanSans.txt`](fonts/LICENSE-SourceHanSans.txt). |
| [`fonts/antonym_prompt_font_28.c`](fonts/antonym_prompt_font_28.c) | LVGL 28 px, 4 bpp, uncompressed subset containing the ten prompt characters. It gives the question word a clearly distinct visual hierarchy and is compiled as `antonym_prompt_font_28`. | Generated from the same Source Han Sans CN Regular source under SIL Open Font License 1.1. |

Reproduction command (the comma-separated non-ASCII list is the exact inventory used by this application):

```text
lv_font_conv --font SourceHanSansCN-Regular.otf \
  --range 0x20-0x7E,0x00B7,0x4E00,0x4E0A,0x4E0B,0x4E0D,0x4E49,0x4E86,0x4EAE,0x4F1A,0x4F4E,0x4F5C,0x4F7F,0x5173,0x518D,0x51B7,0x5206,0x5207,0x524D,0x52A0,0x53CD,0x53F3,0x548C,0x5566,0x574F,0x5802,0x591A,0x5927,0x592A,0x597D,0x5B8C,0x5B9A,0x5BBD,0x5BF9,0x5C0F,0x5DE6,0x5E93,0x5F00,0x5FEB,0x60F3,0x6162,0x6210,0x62E9,0x6309,0x6362,0x65E9,0x6696,0x66F4,0x672C,0x6765,0x68D2,0x6B21,0x6CB9,0x70ED,0x7528,0x767D,0x771F,0x77ED,0x786E,0x79BB,0x7B54,0x7EBF,0x7EDC,0x7EE7,0x7EED,0x7F51,0x8BCD,0x8BFE,0x8F6E,0x8FDB,0x8FDC,0x9009,0x91CC,0x957F,0x9898,0x9AD8,0x9ED1,0xFF01,0xFF0C,0xFF1A \
  --size 16 --bpp 4 --format lvgl --no-compress --no-kerning \
  --lv-include lvgl.h --lv-font-name antonym_font_16 \
  --output assets/fonts/antonym_font_16.c

lv_font_conv --font SourceHanSansCN-Regular.otf \
  --range 0x4E0A,0x51B7,0x5927,0x597D,0x5DE6,0x5F00,0x5FEB,0x957F,0x9AD8,0x9ED1 \
  --size 28 --bpp 4 --format lvgl --no-compress --no-kerning \
  --lv-include lvgl.h --lv-font-name antonym_prompt_font_28 \
  --output assets/fonts/antonym_prompt_font_28.c
```

- Use descriptive names that include the family, weight, size, and format when relevant.
- Document the source, license, character range, conversion command, and expected destination.
- Check Flash and internal-RAM impact before adding a font; the ESP32-C3 has no PSRAM.
- Do not commit fonts whose license does not permit redistribution.

## Images

Store reusable source images and generated display assets in `images/`.

| File | Dimensions and format | Use and source |
| --- | --- | --- |
| [`images/home.jpg`](images/home.jpg) | 3840 × 2160, JPEG | Product hero image embedded in both project README files to foreground AI Passport and its open, maker-oriented identity. |
| [`images/readme-hardware-specs.png`](images/readme-hardware-specs.png) | 2172 × 724, PNG RGBA | Optional technical infographic retained as a reference asset; it is no longer used as the homepage hero. Generated for this repository with the built-in image generation tool on 2026-09-17; the six labels and values were checked against the documented hardware contract. |
| [`images/logo-wordmark.png`](images/logo-wordmark.png) | 1648 × 336, PNG RGBA | Transparent black wordmark extracted from the repository's original `images/logo.png`; embedded in both project README files for light backgrounds. |
| [`images/logo-wordmark-dark.png`](images/logo-wordmark-dark.png) | 1648 × 336, PNG RGBA | White version of the extracted wordmark, used by the README `<picture>` element when GitHub is in dark mode. |

- Use descriptive names and document dimensions, pixel format, conversion steps, and destination.
- Prefer formats suitable for the 240 × 320 RGB565 display and account for Flash and internal RAM.
- Preserve editable sources where licensing permits, and record the source and license.
- Never commit device QR secrets, credentials, or personal data in images.

## Music and sound effects

Store reusable music and sound-effect sources in `music/`.

- Document the source, license, sample rate, bit depth, channels, conversion command, and destination.
- Prefer 16 kHz, 16-bit mono PCM when it matches the current BSP audio path.
- Check Flash and internal-RAM cost before embedding audio; stream or chunk long recordings.
- Do not commit media without redistribution permission.
