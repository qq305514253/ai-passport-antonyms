<p align="right">
  <strong>English</strong> · <a href="README.zh_CN.md">简体中文</a>
</p>

# Antonym Mini Classroom

An offline Chinese antonym practice app for FoloToy AI Passport. Each round contains ten elementary-level questions with two choices, immediate color feedback, encouraging messages, and a final score.

## How to play

1. Power on to enter the classroom. No network setup or sign-in is required.
2. Press **Up** or **Down** to move the blue selection highlight between the two choices.
3. Press **OK** to submit. The correct answer turns green; a wrong selection turns red while the correct answer is revealed in green.
4. Press **OK** again for the next question. After ten questions, hold **OK** to start a new round.

The header shows question progress, score, and device battery level. The prompt word uses a larger dedicated Chinese font so it remains visually distinct from the answer choices.

## Build

The project targets the FoloToy AI Passport and follows the upstream ESP-IDF 5.5.3 workflow. After activating ESP-IDF, run the repository validation scripts described in [`docs/development/engineering/build-and-test.md`](docs/development/engineering/build-and-test.md).

The validated merged image is generated as `build/FoloToy-AI-Passport-full.bin` and is intended to be flashed from offset `0x0`. Generated build output is not committed to this repository.

## Controls

| Input | Action |
| --- | --- |
| Up / Down click | Change the selected answer |
| OK click | Submit the answer or continue |
| OK long press | Restart the round |

## Credits

Built on the [FoloToy AI Passport](https://github.com/FoloToy/ai-passport) project. The embedded Chinese font subsets are generated from Adobe Source Han Sans CN Regular under the SIL Open Font License 1.1; see [`assets/fonts/LICENSE-SourceHanSans.txt`](assets/fonts/LICENSE-SourceHanSans.txt).
