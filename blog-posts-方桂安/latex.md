---
title: "Texlive+VScode"
date: 2021-10-18
authors:
  - name: 方桂安 (Enderfga)
    link: https://github.com/Enderfga
tags: [LaTeX, VSCode, 写作]
---

搭建Latex环境：Texlive+VScode 相关记录


## 1.安装 Texlive


鉴于我校没有（我知道的）可用开源软件镜像站，所以在到清华大学开源软件镜像站的[texlive](https://mirrors.tuna.tsinghua.edu.cn/CTAN/systems/texlive/Images/)页下载。


[![](https://img.enderfga.cn/img/5UYncq.png)](https://imgtu.com/i/5UYncq)


可能由于更新导致你打开之后的页面与上面的内容不一致，总之下载最新的texlive***.iso，虽然很大但各种宏包齐全，用起来能省去各种麻烦。


在带宽扩容之后的校园网加持下，不用几年就能下载完这个iso文件了。


如果你是windows
7甚至xp用户，我建议你把iso文件解压然后进行后续操作。


如果是windows 10/11，系统自带虚拟光驱，直接双击进入即可。


（ linux/macOS 我不了解，省略）效果如图：


[![](https://img.enderfga.cn/img/5UUe9H.png)](https://imgtu.com/i/5UUe9H)


双击或者右键以管理员身份运行install-tl-advanced.bat，可以点进**Advanced**进入高级安装，点击**Customize**来取消你不需要安装的宏包，比如非中英的语言包，这里我只修改了安装目录，最后开始漫长的等待。


[![](https://img.enderfga.cn/img/5UaCGQ.png)](https://imgtu.com/i/5UaCGQ)


（安装TeXworks前端也可以取消掉，毕竟都打算用vscode了，加上前面说的语言包之类的，可以省个1G左右，我想着留条后路就啥都没改，也不缺这点空间）


（在我的电脑上一共安装了57 min 56 s，教程都快写完了，还没有装好）


## 2. 安装 VSCode


到[官网](https://code.visualstudio.com/Download)根据你的系统选择下载安装即可，这部分应该大多数人都安装过了，没什么需要注意的。


[![](https://img.enderfga.cn/img/5Ud4hD.png)](https://imgtu.com/i/5Ud4hD)


安装完成之后可以在应用商店挑选各种提高使用体验的扩展，跟本文相关的主要是**Latex
Workshop**。


[![](https://img.enderfga.cn/img/5U0KJS.png)](https://imgtu.com/i/5U0KJS)


安装完成之后，可以创建或者打开一个tex文件，此时代码已经被高亮显示了。


[![](https://img.enderfga.cn/img/5U560K.png)](https://imgtu.com/i/5U560K)


按下快捷键Ctrl+Alt+B（build latex project），顺利生成，效果不错。


[![](https://img.enderfga.cn/img/5U5xcn.png)](https://imgtu.com/i/5U5xcn)


## 3. 配置 VSCode 的 插件


按下F1或者Ctrl＋shift＋P，输入setjson，选择第三个（如图所示）。


[![](https://img.enderfga.cn/img/5Uo9xA.png)](https://imgtu.com/i/5Uo9xA)


```
"latex-workshop.latex.tools": [
        {
   // 编译工具和命令
   "name": "xelatex",
   "command": "xelatex",
   "args": [
   "-synctex=1",
   "-interaction=nonstopmode",
   "-file-line-error",
   "-pdf",
   "%DOCFILE%"
            ]
        },
        {
   "name": "pdflatex",
   "command": "pdflatex",
   "args": [
   "-synctex=1",
   "-interaction=nonstopmode",
   "-file-line-error",
   "%DOCFILE%"
            ]
        },
        {
   "name": "bibtex",
   "command": "bibtex",
   "args": [
   "%DOCFILE%"
            ]
        }
    ],
   "latex-workshop.latex.recipes": [
      {
   "name": "xelatex",
   "tools": [
   "xelatex"
          ],
      },
      {
   "name": "pdflatex",
   "tools": [
   "pdflatex"
          ]
      },
      {
   "name": "xelatex->bibtex->xelatex->xelatex",
   "tools": [
   "xelatex",
   "bibtex",
   "xelatex",
   "xelatex"
          ]
      },
      {
   "name": "pdflatex->bibtex->pdflatex->pdflatex",
   "tools": [
   "pdflatex",
   "bibtex",
   "pdflatex",
   "pdflatex"
          ]
      }
  ],
  "latex-workshop.view.pdf.viewer": "tab",
"editor.inlineSuggest.enabled": true,
"latex-workshop.latex.autoClean.run": "onBuilt",
"latex-workshop.latex.autoBuild.run": "never",
```

- Ctrl+Alt+B 是编译

- Ctrl+Alt+V是编译+预览pdf


我最开始写这些其实是想要把中大的foxitpdf设置成默认的pdf预览软件，不过最终效果并不好，所以作罢。


（咨询了foxit的技术客服，他们说目前是实现不了的）


上面这些设置主要是因为默认的编译工具是 latexmk，由于不需要用到
latexmk，因此把其修改为中文环境常用的 xelatex；将 tools 中的
%DOC%替换成%DOCFILE%就可以支持编译中文路径下的文件了。


还可以研究的设置有很多，什么正向搜索反向搜索之类的，有兴趣的朋友可以自行了解。


[公式指导手册](https://ericp.cn/cmd)


如果中文无法显示就加上这一句：


```css
\usepackage[UTF8]{ctex}
```


Latex的相关公式及使用就不再赘述了。


由于vscode不一定能成功实现4次编译，故我编写了以下bat文件，可以一次性生成pdf并清除所有过程文件：


```shell
::======================================
:: 四次编译：xe-bib-xe-xe
::======================================
xelatex report
bibtex report
xelatex report
xelatex report
::======================================
:: 清除文件以及清除更多文件
::======================================
:clean
echo 删除编译临时文件
del /f /q /s *.log *.glo *.ilg *.lof *.ind *.out *.thm *.toc *.lot *.loe *.out.bak *.blg *.synctex.gz *.aux *.bbl *.xdv
del /f /q *.idx
del /f /s *.dvi *.ps
goto end

::======================================
:: 结束符，无任何具体意义
::======================================
:end
```
