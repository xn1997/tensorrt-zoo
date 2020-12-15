<!--

 * @Author: zerollzeng
 * @Date: 2019-09-02 16:45:43
 * @LastEditors: zerollzeng
 * @LastEditTime: 2019-12-10 17:41:21
 -->
# 个人修改内容

1. 修改部分程序，使其可以在不改变原有使用方法的情况下，使用INT8量化

2. 修改`testopense.cpp`，使其进行INT8量化及推理

   1. INT8量化，需要在`model/resize_image/`文件夹内存入相应的图片，名称为`{1,2,3,...}.jpg`。

      数据的具体生成方法，参见`NoteBook/小程序/`下的参考程序。

      用于生成`calibration校准表`，用于INT8量化

## 使用方法

- 配合`elevator monitor`使用
  1. 在该项目`tensorrt-zoo`，生成`openpose`的`engine`文件
  2. 将`engine`文件拷贝至`elevator_monitor/main/build/openpose_model/`下，供程序调用即可。

# tensorrt-zoo
common computer vision models and some useful tools base on [tiny-tensorrt](https://github.com/zerollzeng/tiny-tensorrt).

**抱歉各位我现在没有太多精力维护这个项目，希望各位在提issue的时候能够尽量根据模板把信息补充完整，只是简单说明或者贴个失败的图我也无法定位问题**

**This project will not update with latest tiny-tensorrt, and I don't have much time maintain this repo, if you have any issue while modify this project with openpose and yolov3, please try dig into source code by your self, I don't remenber the details too :D**

# Roadmap
- [x] openpose :fire::fire::fire: --- 2019.10.17
- [x] yolov3
- [ ] find some interesting things can done with tiny tensorrt :dancer::dancer::dancer:

# Quick Start
for run sample you need to install opencv and TensorRT

```bash
git clone --recursive https://github.com/zerollzeng/tensorrt-zoo.git
mkdir build && cd build && cmake -D PYTHON_API ON .. && make
```
for yolo3 sample see docs/yolov3.md
for openpose sample see docs/openpose.md


# System requirements
CUDA version >= 10.0 is fully test

TensorRT 6.0+, 5.x you need to reset old commit(not recommend)

OPENCV version >= 3.0, only use for read/write images and some basic image processing. so version 2.x might work but have not been tested.

# About License
most of the openpose post processing was copy and modify slightly from official openpose [repo](https://github.com/CMU-Perceptual-Computing-Lab/openpose),maybe you should distribute it under openpose's [license](https://github.com/CMU-Perceptual-Computing-Lab/openpose/blob/master/LICENSE), so as for some other code. for the other I wrote, You just DO WHAT YOU WANT TO.


