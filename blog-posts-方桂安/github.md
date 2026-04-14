---
title: "How to Use GitHub?"
date: 2022-09-24
authors:
  - name: 方桂安 (Enderfga)
    link: https://github.com/Enderfga
tags: [GitHub, 教程, 深度学习, 笔记]
---

作为全球最大的同性交友网站gayhub，应该怎么用

<!--more-->

## 开始之前

最好学会科学上网，我没办法教这个，虽然GitHub在国内是可以访问的，但有时候确实慢得难受

不过加速GitHub的方法千奇百怪，甚至可以开个游戏加速器，这里就不再赘述了

## 介绍

GitHub是全球最大的开源代码网站，这里充满了最前沿的学术、最沙雕的整活、最无私的分享（突然的感慨）。

科研领域的论文源码发这里，网友们的女装照片发这里，各种各样的知识整合也发在这里，相较于国内那些c\*\*n啥的网站，软件安装包、电子书下载都要钱（发布者都不是原作者），真是天壤之别。

## 教程？

虽然但是，每当我写一个博客想分享点什么的时候还是不禁会怀疑自己，发展到现在这个时代，无论什么内容都能在网上找到很优秀的案例，相较于bilibili这种视频媒体，写出来的文字确实略显苍白，更优质的内容比比皆是。

所以，想简单地了解一下如何使用的话，不妨从这里看起，浏览一些播放量高跟时间新的视频：

https://search.bilibili.com/all?keyword=GitHub

![](https://img.enderfga.cn/img/image-20220925001854942.png)

我写教程前也看了几个，蛮不错的！

（或许我也应该尝试使用视频录制来分享知识emmmm）

## 案例

### 任务内容

写都写了，总不能啥也不教吧，我决定以一个小案例来介绍一下我院学子都是怎么用GitHub炼丹的。

假设我们现在准备参与某个比赛或者完成某项作业，首先我们手里肯定先有的是数据集，以计算机视觉的第一次作业为例：

![](https://img.enderfga.cn/img/image-20220925113333330.png)

![](https://img.enderfga.cn/img/image-20220925113344247.png)

![](https://img.enderfga.cn/img/image-20220925113401643.png)

这是一个图像分类任务，评分依据是test集上的结果，所以我们肯定希望追求最好的模型，可以在paperwithcode的imageclassification对应的[sota](https://paperswithcode.com/task/image-classification)页面下了解目前哪些模型比较先进：

![](https://img.enderfga.cn/img/image-20220925111228440.png)

如果手里的数据集是cifar10/100，imagenet等经典数据，那榜单就有很高的参考价值了。鉴于我们手里的plant pathology-2021不在榜单上，我们从作业建议的模型，也是cv领域比较著名的Lenet、VGG16、ResNet50、VIT等试起。

### 白嫖代码

我选择经典到不能再经典的ResNet50来作为本次案例的范本，作为15年提出来的里程碑级别的模型，想要跑resnet实在是太简单不过了。无论是直接百度就能查到网友们各种各样的实现，还是后续的改进优化版本，甚至是直接使用keras.application torch.hub里直接调库，总之，实现目标的途径有很多。

直接在GitHub上搜索resnet，可以得到以下结果：

![](https://img.enderfga.cn/img/image-20220925112920348.png)

根据经验我会选择最下面蓝色框中的仓库，因为上面的内容都与我的需求不太相适配（AI不是分类，MXNet、tensorflow、keras不是熟悉的框架，Lua不是熟悉的语言）

Pytorch-cifar100这个库看标题是使用torch在cifar100数据集上的分类，所以我们只需要下载下来，修改读取数据的部分的代码便可以跑通，得到自己的分类结果。

如果是用自己的电脑在code处下载代码的zip比较方便，但如果是云服务器，一般都装有git，直接

```shell
git clone https://github.com/weiaicunzai/pytorch-cifar100.git
```

### 准备数据

我点开train.py阅读源码，找到读取数据的代码，一般这些工具函数都写在utils.py中，可以看到这里是利用pytorch自带的cifar100数据的

![](https://img.enderfga.cn/img/image-20220925113534402.png)

![](https://img.enderfga.cn/img/image-20220925113713799.png)

![](https://img.enderfga.cn/img/image-20220925121714856.png)

读到这里，只要改好我红框圈出来的代码其实就能开始在自己的数据集上训练了，我个人比较习惯使用的是Imagefolder函数，这个函数读取的数据集需要按照类别整理好数据，既train/第一类/第一类的图片，train/第二类/第二类的图片...以此类推。显然我们拿到的数据集不是这样，属于图片和对应的label分开存储的情况，所以我一般会写一个pre_data.py来整理数据，如下所示：

```python
# 数据集路径（这是我电脑上的，使用时也需要修改）
path = r'C:\Users\User\Downloads\Compressed\plant_dataset\train' #训练集
# path = r'C:\Users\User\Downloads\Compressed\plant_dataset\test' #测试集
# path = r'C:\Users\User\Downloads\Compressed\plant_dataset\val' #验证集
# 读取csv中的label
import pandas as pd
# 记得把train作对应修改
df = pd.read_csv(path+'/train'+'/train_label.csv')
# 给每一种label创建文件夹，若已存在则跳过
import os
for i in df['labels'].unique():
    if not os.path.exists(path+'/'+i):
        os.makedirs(path+'/'+i)
# 将图片移动到对应的文件夹中
import shutil
for i in range(len(df)):
    shutil.copy(path+'/images/'+df['images'][i],path+'/'+df['labels'][i]+'/'+df['images'][i])
# 注意 这里用的是copy不是move，原来的图片还都保存在images文件夹中，训练前需要移走
```

### 配置环境

这一步根据打开的仓库操作，如果你的电脑上已经装好常用的torch，tensorflow等包可能也不需要

我一般的经验是使用anaconda创建一个虚拟环境

```shell
conda create -n myenv python=3.6 # 3.6是这个仓库Requirements里写的，注意修改
conda activate myenv
```

有些readme里Requirements写完了一两个需要包，直接pip install xxx就好，有些则在仓库里准备了requirements.txt，使用

```shell
pip install -r requirements.txt
```

总之配置环境这步得对症下药，有时候还需要一些其他工作，具体查看readme对症下药。

---

注意，cv作业建议使用mindspore框架，而不是torch，所以情况会有所不同。

## All in All

暂时想不到什么可以写的内容，有什么问题不如评论或者私信我吧，GitHub上的精品项目太多了，天天都在疯狂star。例如以上提到的模型，我推荐在这个[仓库](https://github.com/WZMIAOMIAO/deep-learning-for-image-processing)里查看代码，还附带有讲解

![](https://img.enderfga.cn/img/image-20220925130630621.png)

希望看到这里的同学打开我的GitHub主页给我一个[star](https://github.com/Enderfga/Enderfga)。

![](https://img.enderfga.cn/img/image-20220925130725261.png)
