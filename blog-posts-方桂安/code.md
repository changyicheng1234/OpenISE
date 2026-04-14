---
title: "conda/linux/git/正则表达式常用命令"
date: 2022-04-28
authors:
  - name: 方桂安 (Enderfga)
    link: https://github.com/Enderfga
tags: [conda, linux, git, 正则表达式, 命令行]
---

一些常用的命令，每次忘了都得搜，记录一下


![](https://img.enderfga.cn/img/image-20220428094832340.png)


# Anaconda&python


![](https://img.enderfga.cn/img/image-20220428093957780.png)


**pip安装（tensorflow-gpu为例）**


```shell
pip  install tensorflow-gpu
```


**conda安装**


```shell
conda  install tensorflow-gpu
```


**pip3安装（指定版本号只需在命令末尾添加==1.12.0版本号）**


```shell
pip3  install tensorflow-gpu==1.12.0
```


**使用清华镜像下载**


```shell
pip install tensorflow-gpu==1.10 -i https://pypi.tuna.tsinghua.edu.cn/simple
```


**指定目录安装**


```shell
pip install -t D:\ProgramData\Anaconda3\Lib\site-packages torch-1.0.1-cp36-cp36m-win_amd64.whl
```


**卸载安装（pip3只需将conda换成pip）**


```shell
conda  uninstall tensorflow-gpu
```


**创建虚拟环境（conda为例）**


```shell
conda create -n py36 python=3.6  #py36虚拟环境的名字  python=3.6  python版本
```


**删除虚拟环境**


```shell
conda remove -n py36 --all
```


**激活虚拟环境**


```shell
conda activate py36
```


**退出虚拟环境**


```shell
conda deactivate
```


**查看所有创建的虚拟环境**


```shell
conda env list
```


**用virtualenv创建虚拟环境**


```bash
VENV_DIR=venv
pip install virtualenv
virtualenv $VENV_DIR
source $VENV_DIR/bin/activate
deactivate
```


**nohup送入后台运行**


```shell
nohup python train.py >nohup 2>&1 &      #train.py运行的文件  nohup生成的日志文件
```


**CUDA指定GPU**


```shell
CUDA_VISIBLE_DEVICES=0 nohup python train.py  > nohup.log 2>&1 &
```


导出requirements.txt


```bash
python3 -m pip freeze > requirements.txt
```


**查看GPU使用情况**


```shell
nvidia-smi
```


**查看进程号**


```shell
ps aux
```


**根据进程号杀死进程**


```shell
kill -9 进程号
```


![](https://img.enderfga.cn/img/200301122312741.jpg)


# linux


linux不像Windows 分了盘，它根目录下有如下常用文件夹:


*home* ---------- 用户的家


*root* ---------- 超级管理员root的家


*etc* ---------- 存放配置文件


*usr* ---------- 存放共享资源


## 1、cd命令:


**①、进入某一个目录** `cd 目录名`


**②、进入多级目录** `cd 目录名/目录名`


**③、返回上一级目录** `cd ..`


**④、返回根目录** `cd /`


**⑤、返回根目录下的某一个目录**
`cd /目录名`


**⑥、回家** `cd ~`


## 2、创建、删除目录:


**①、创建目录** `mkdir 目录名`


**②、创建多级目录** `mkdir -p a/b/c`


**③、删除目录(只能删除空目录)**
`rmdir 目录名`


**④、删除目录(可删除非空目录，带询问)**
`rm -r`


**⑤、删除目录(不带询问，谨慎使用)**
`rm -rf`


## 3、对文件的操作:


**①、创建空白文件** `touch 文件名`


**②、复制文件**


`cp a.txt b.txt` *表示复制a文件并重命名为b。*


`cp a.txt dir/b.txt`
*表示把a复制到dir文件夹下并重命名为b。*


**③、移动文件** `mv a.txt dir/b.txt`
*把a.txt移动到dir目录下并重命名为b.txt。*


**④、重命名文件** `mv a.txt b.txt`
*把a.txt重命名为b.txt。*


**⑤、删除文件**


`rm 文件名` *带询问的删除*


`rm -f 文件名` *不带询问的删除。*


**⑥、浏览文件**


`cat 文件名` *显示文件所有内容*


`more 文件名`
*分页显示，空格键下一页，回车键下一行。*


`less 文件名`
*分页显示，pgup上一页，pgdn下一页。*


`tail -5 a.txt` *显示a.txt文件的最后5行。*


`tail -f 文件名` *动态的查看。*


## 4、查看目录下的文件:


**①、查看所有文件和目录名称** `ls`


**②、查看所有文件和目录名称(包括隐藏的)**
`ls -a`


**③、查看文件并显示详细信息(最常用)**
`ll`


**④、友好的显示** `ll -h`
*比如显示的文件大小是kb而不是字节。*


## 5、tar打包命令:


**①、将当前目录所有文件打包成haha.tar**
`tar -cvf haha.tar ./*`


**②、将当前目录下所有文件打包并压缩成haha.tar**
`tar -zcvf haha.tar.gz ./*`


**③、将haha.tar解压到当前目录**
`tar -xvf haha.tar`


**④、将haha.tar解压到b目录**
`tar -xvf haha.tar -C b` *注意C是大写的！*


## 6、其他常用命令:


**①、grep命令**


`grep category a.txt`
*表示在a.txt中查找category字符串所在的行，前提是打开了a.txt文件。*


`grep category a.txt -A2`
*在a.txt中查找category字符串的前两行。*


`grep category a.txt -B2`
*在a.txt中查找category字符串的后两行。*


**②、查看当前目录** `pwd`


**③、wget下载命令** `wget www.baidu.com`
*下载百度首页*


## 7、vi/vim编辑器:


**①、编辑器有三种模式，分别是:**
**命令行模式:**
此模式无法编辑文件，`yy`复制行，`p`粘贴，`dd`删除行，按如下键都可以进入插入模式:


`i` 当前位置前插入;


`I` 当前行行首插入;


`a` 当前位置后插入;


`A` 当前行行尾插入;


`o` 当前行之后插入一行;


`O` 当前的之前插入一行


**插入模式:**此模式下可以对文件进行编辑。按
`esc`退出插入模式，回到命令行模式。
**底行模式:**命令行模式下按
`:`，即可进入底行模式。底行模式有如下常用命令:


`q` 不保存退出;


`q！` 不保存强制退出;


`wq` 保存退出


## 8、管道:


**管道:`|`，将一个命令的输出作为另一个命令的输入。例如:**
**在 `ip addr`的输出结果中查找
`192.168`字符串:**
`ip addr | grep 192.168`


## 9、系统管理命令:


**①、查看系统时间** `date` 查看系统时间
`date -s "2018-05-15 22:22:22"`将系统时间设置为引号里面的时间


**②、查看磁盘信息** `df` 查看磁盘信息
`df -h` 友好地展示磁盘信息


**③、清屏** `clear`或者按
`ctr L`


**④、进程** `ps -ef`查看所有进程
`ps -ef | grep ssh`查找ssh进程


**⑤、杀掉进程** `kill 9527`杀掉9527号进程
`kill -9 9527` 强制杀掉9527号进程


**⑥、查看网络端口**
`netstat -an | grep 3306`查看3306端口占用情况


**⑦、ping命令**
`ping xx.xx.xxx`测试网络连通性


![](https://img.enderfga.cn/img/bg2015120901.png)


# GIT


下面是常用 Git 命令清单。几个专用名词的译名如下。


> - Workspace：工作区

> - Index / Stage：暂存区

> - Repository：仓库区（或本地仓库）

> - Remote：远程仓库

## 一、新建代码库


```bash
# 在当前目录新建一个Git代码库
$ git init

# 新建一个目录，将其初始化为Git代码库
$ git init [project-name]

# 下载一个项目和它的整个代码历史
$ git clone [url]
```


## 二、配置


Git的设置文件为
`.gitconfig`，它可以在用户主目录下（全局配置），也可以在项目目录下（项目配置）。


```bash
# 显示当前的Git配置
$ git config --list

# 编辑Git配置文件
$ git config -e [--global]

# 设置提交代码时的用户信息
$ git config [--global] user.name "[name]"
$ git config [--global] user.email "[email address]"
```


## 三、增加/删除文件


```bash
# 添加指定文件到暂存区
$ git add [file1] [file2] ...

# 添加指定目录到暂存区，包括子目录
$ git add [dir]

# 添加当前目录的所有文件到暂存区
$ git add .

# 添加每个变化前，都会要求确认
# 对于同一个文件的多处变化，可以实现分次提交
$ git add -p

# 删除工作区文件，并且将这次删除放入暂存区
$ git rm [file1] [file2] ...

# 停止追踪指定文件，但该文件会保留在工作区
$ git rm --cached [file]

# 改名文件，并且将这个改名放入暂存区
$ git mv [file-original] [file-renamed]
```


## 四、代码提交


```bash
# 提交暂存区到仓库区
$ git commit -m [message]

# 提交暂存区的指定文件到仓库区
$ git commit [file1] [file2] ... -m [message]

# 提交工作区自上次commit之后的变化，直接到仓库区
$ git commit -a

# 提交时显示所有diff信息
$ git commit -v

# 使用一次新的commit，替代上一次提交
# 如果代码没有任何新变化，则用来改写上一次commit的提交信息
$ git commit --amend -m [message]

# 重做上一次commit，并包括指定文件的新变化
$ git commit --amend [file1] [file2] ...
```


## 五、分支


```bash
# 列出所有本地分支
$ git branch

# 列出所有远程分支
$ git branch -r

# 列出所有本地分支和远程分支
$ git branch -a

# 新建一个分支，但依然停留在当前分支
$ git branch [branch-name]

# 新建一个分支，并切换到该分支
$ git checkout -b [branch]

# 新建一个分支，指向指定commit
$ git branch [branch] [commit]

# 新建一个分支，与指定的远程分支建立追踪关系
$ git branch --track [branch] [remote-branch]

# 切换到指定分支，并更新工作区
$ git checkout [branch-name]

# 切换到上一个分支
$ git checkout -

# 建立追踪关系，在现有分支与指定的远程分支之间
$ git branch --set-upstream [branch] [remote-branch]

# 合并指定分支到当前分支
$ git merge [branch]

# 选择一个commit，合并进当前分支
$ git cherry-pick [commit]

# 删除分支
$ git branch -d [branch-name]

# 删除远程分支
$ git push origin --delete [branch-name]
$ git branch -dr [remote/branch]
```


## 六、标签


```bash
# 列出所有tag
$ git tag

# 新建一个tag在当前commit
$ git tag [tag]

# 新建一个tag在指定commit
$ git tag [tag] [commit]

# 删除本地tag
$ git tag -d [tag]

# 删除远程tag
$ git push origin :refs/tags/[tagName]

# 查看tag信息
$ git show [tag]

# 提交指定tag
$ git push [remote] [tag]

# 提交所有tag
$ git push [remote] --tags

# 新建一个分支，指向某个tag
$ git checkout -b [branch] [tag]
```


## 七、查看信息


```bash
# 显示有变更的文件
$ git status

# 显示当前分支的版本历史
$ git log

# 显示commit历史，以及每次commit发生变更的文件
$ git log --stat

# 搜索提交历史，根据关键词
$ git log -S [keyword]

# 显示某个commit之后的所有变动，每个commit占据一行
$ git log [tag] HEAD --pretty=format:%s

# 显示某个commit之后的所有变动，其"提交说明"必须符合搜索条件
$ git log [tag] HEAD --grep feature

# 显示某个文件的版本历史，包括文件改名
$ git log --follow [file]
$ git whatchanged [file]

# 显示指定文件相关的每一次diff
$ git log -p [file]

# 显示过去5次提交
$ git log -5 --pretty --oneline

# 显示所有提交过的用户，按提交次数排序
$ git shortlog -sn

# 显示指定文件是什么人在什么时间修改过
$ git blame [file]

# 显示暂存区和工作区的差异
$ git diff

# 显示暂存区和上一个commit的差异
$ git diff --cached [file]

# 显示工作区与当前分支最新commit之间的差异
$ git diff HEAD

# 显示两次提交之间的差异
$ git diff [first-branch]...[second-branch]

# 显示今天你写了多少行代码
$ git diff --shortstat "@{0 day ago}"

# 显示某次提交的元数据和内容变化
$ git show [commit]

# 显示某次提交发生变化的文件
$ git show --name-only [commit]

# 显示某次提交时，某个文件的内容
$ git show [commit]:[filename]

# 显示当前分支的最近几次提交
$ git reflog
```


## 八、远程同步


```bash
# 下载远程仓库的所有变动
$ git fetch [remote]

# 显示所有远程仓库
$ git remote -v

# 显示某个远程仓库的信息
$ git remote show [remote]

# 增加一个新的远程仓库，并命名
$ git remote add [shortname] [url]

# 取回远程仓库的变化，并与本地分支合并
$ git pull [remote] [branch]

# 上传本地指定分支到远程仓库
$ git push [remote] [branch]

# 强行推送当前分支到远程仓库，即使有冲突
$ git push [remote] --force

# 推送所有分支到远程仓库
$ git push [remote] --all
```


## 九、撤销


```bash
# 恢复暂存区的指定文件到工作区
$ git checkout [file]

# 恢复某个commit的指定文件到暂存区和工作区
$ git checkout [commit] [file]

# 恢复暂存区的所有文件到工作区
$ git checkout .

# 重置暂存区的指定文件，与上一次commit保持一致，但工作区不变
$ git reset [file]

# 重置暂存区与工作区，与上一次commit保持一致
$ git reset --hard

# 重置当前分支的指针为指定commit，同时重置暂存区，但工作区不变
$ git reset [commit]

# 重置当前分支的HEAD为指定commit，同时重置暂存区和工作区，与指定commit一致
$ git reset --hard [commit]

# 重置当前HEAD为指定commit，但保持暂存区和工作区不变
$ git reset --keep [commit]

# 新建一个commit，用来撤销指定commit
# 后者的所有变化都将被前者抵消，并且应用到当前分支
$ git revert [commit]

# 暂时将未提交的变化移除，稍后再移入
$ git stash
$ git stash pop
```


## 十、其他


```bash
# 生成一个可供发布的压缩包
$ git archive
```


一个常用的实例


```bash
git remote add origin xxx(复制的SSH链接)
git branch -m master main
```


```bash
git add .
git commit -m "注释"
git pull --rebase origin main
git push origin main
```


# 正则表达式

> 原文此处嵌入了正则表达式速查PDF，请参考原始博客页面查看。
