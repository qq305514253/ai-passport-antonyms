<p align="right">
  <a href="README.md">English</a> · <strong>简体中文</strong>
</p>

# 反义词小课堂

一款运行在 FoloToy AI Passport 上的离线中文反义词练习应用。每轮包含 10 道适合小学生的基础题，每题提供两个候选词，作答后立即显示颜色反馈和鼓励语，完成后展示本轮得分。

## 使用方法

1. 开机后直接进入课堂，无需配网或登录。
2. 按**上键**或**下键**，在两个候选词之间移动蓝色选框。
3. 按**确定键**提交答案。正确词会变绿；选错时，已选词变红，同时以绿色显示正确答案。
4. 再按一次**确定键**进入下一题。完成 10 题后，长按**确定键**重新开始。

页面顶部显示题目进度、当前得分和设备电量。题目词使用独立的大号中文字库，与候选词形成清晰的视觉层级。

## 构建

项目面向 FoloToy AI Passport，并沿用上游 ESP-IDF 5.5.3 构建流程。激活 ESP-IDF 后，按照 [`docs/development/engineering/build-and-test.zh_CN.md`](docs/development/engineering/build-and-test.zh_CN.md) 运行仓库验证脚本。

验证后的合并固件位于 `build/FoloToy-AI-Passport-full.bin`，从 `0x0` 偏移刷写。生成的构建产物不会提交到本仓库。

## 按键

| 操作 | 功能 |
| --- | --- |
| 短按上键 / 下键 | 切换候选答案 |
| 短按确定键 | 提交答案或继续下一题 |
| 长按确定键 | 重新开始本轮练习 |

## 致谢

本项目基于 [FoloToy AI Passport](https://github.com/FoloToy/ai-passport) 开发。内置中文字库子集由 Adobe 思源黑体生成，采用 SIL Open Font License 1.1；详见 [`assets/fonts/LICENSE-SourceHanSans.txt`](assets/fonts/LICENSE-SourceHanSans.txt)。
