#import "../base/templates/report.typ": *
#import "@preview/codly:1.2.0": *
#import "@preview/codly-languages:0.1.1": *
#show: codly-init.with()
#codly(languages: codly-languages)
#show: report.with(
  title: "《人工智能编程语言》第3次作业",
  subtitle: "公交IC卡刷卡数据分析",
  name: "胡嘉睿",
  stdid: "24311019",
  classid: "智慧交通班",
  major: "智慧交通",
  school: "智能工程学院",
  time: "2024~2025 学年第二学期",
  banner: "../images/sysu.png",
)

#set text(font: "Noto Serif CJK SC")
#show math.equation: set text(font: ("Libertinus Math", "Noto Serif CJK SC"))

#set par(first-line-indent: 0pt)

= 目标
熟练掌握Python中的`numpy`，`pandas`等第三方应用库的使用。

= 具体任务

#quote(block: true)[交通大数据是由交通运行管理直接产生的数据（包括各类道路交通、公共交通、对外交通的刷卡、线圈、卡口、GPS、视频、图片等数据）、交通相关行业和领域导入的数据（气象、环境、人口、规划、移动通信手机信令等数据），以及来自公众互动提供的交通状况数据（通过微博、微信、论坛、广播电台等提供的文字、图片、音视频等数据）构成的。]

#set par(first-line-indent: 2em)
--- 


本次作业给出了一个公交刷卡样例数据集`ICData.csv`，包含有交易类型、交易时间、交易卡号、刷卡类型、线路号、车辆编号、上车站点、下车站点、驾驶员编号、运营公司编号等。试导入该数据集并做分析。

+ 分别计算早上7点前和晚上10点之后的公共交通上车刷卡量；
+ 构造一个乘客搭乘站点数分析函数，计算不同线路乘客的平均公交搭乘站点数及其标准差；
+ 计算高峰小时刷卡量和公交刷卡量的高峰小时系数（*PHF5*和*PHF15*）
        $
        "PHF5"="高峰小时交通量"/"12高峰5分钟流量"\
        "PHF15"="高峰小时交通量"/"4高峰15分钟流量"
        $
+ 输出线路号1101-1120中各个车辆编号对应的的驾驶员编号，分别输出为20个txt文档，并命名为相应的线路号，并保存到一个文件夹中；
+ 分析搭载乘客情况，确定服务乘客人次最多的10个司机、10条线路、10个站点和10台车辆。

\
\

代码和生成的文件已经附于压缩包内。

#pagebreak()

= 运行截图

#set image(width: 70%)
#image("image-5.png")
#image("image-1.png")
#image("image-2.png")
#image("image-3.png")
#image("image-4.png")