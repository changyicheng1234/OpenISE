#import "../base/templates/report.typ": *
#import "@preview/codly:1.2.0": *
#import "@preview/codly-languages:0.1.1": *
#import "@preview/colorful-boxes:1.4.2": *
#import "@preview/numbly:0.1.0": *
#show: codly-init.with()
#codly(languages: codly-languages)
#show: report.with(
  title: "《人工智能编程语言》- 期末大作业",
  subtitle: "使用Python语言实现最短路算法",
  name: "胡嘉睿",
  stdid: "24311019",
  classid: "智慧交通班",
  major: "智慧交通",
  school: "智能工程学院",
  time: "2024~2025 学年第二学期",
  banner: "../images/sysu.png", // 请确保此路径正确
)
#set text(font: "Noto Serif CJK SC")
#show math.equation: set text(font: ("Libertinus Math", "Noto Serif CJK SC"))
#set page(numbering: numbly("{1}", "第{1}页/共{2}页"))
#set page(
  header: context {
    // Title top left
    set text(10pt)
    let _sym = ""
    place(
      top + left,
      {
        if query(heading.where(level: 1)).find(h => h.location().page() == here().page()) == none {
          // Filter headers that come after the current page
          let smh = query(heading.where(level: 1)).filter(h => h.location().page() <= here().page())
          _sym + smh.last().body // last element in array is newest level 1 headline
        } else {
          let onPageHeading = query(heading.where(level: 1)).filter(h => h.location().page() == here().page())
          _sym + onPageHeading.first().body
        }
      },
      dy: 1cm,
      dx: 0cm,
    )
    place(
      top + right,
      {
        if query(heading.where(level: 2)).find(h => h.location().page() == here().page()) == none {
          let sub = query(heading.where(level: 2)).filter(h => h.location().page() <= here().page())
          sub.last().body
        } else {
          let onPageSub = query(heading.where(level: 2)).filter(h => h.location().page() == here().page())
          onPageSub.first().body
        }
      },
      dy: 1cm,
      dx: 0cm,
    )
    //Numbering top right
    //place(top + right, "Revue CTF☆素晴らしい Flag", dy: 1cm)  // horizontal line underneath
    place(
      top + left,
      line(length: 100%, stroke: 0.4pt),
      dy: 1.4cm, // 根据文字行高调整线的位置
    )
  },
)
#set par(first-line-indent: 0pt)
= 算法理解与数据预处理

== 在计算机中储存图

#outline-colorbox(
  title: "图在数据结构中的表示",
  color: "green",
  width: auto,
  radius: 2pt,
  centering: true,
)[
  图在数据结构中可以使用*邻接矩阵*和*邻接表*两种方式来表示。

  邻接矩阵是一个 $n times n$ 的矩阵，其中 $n$ 是图中顶点的数量，矩阵中的每个元素表示顶点之间的边的权重。邻接表则是一个数组，每个元素是一个线性表，表中存储与该顶点相连的所有顶点及其对应的边的权重。
]

#figure(
  image("raw.png"), // 请确保 raw.png 图片存在于正确路径
  caption: "原始数据",
)<pic1>

设 @pic1 中有顶点 $V_0, V_1, V_2, V_3, V_4, V_5$ 。构建邻接矩阵 $A$ ，其中 $A_(i j)$ 表示从顶点 $i$ 到顶点 $j$ 的边的权重，若不存在边则权重为 $0$ 。

\

#slanted-colorbox(title: "统计边数")[#grid(
    columns: (2fr, 2fr),
    rows: (auto, 0pt),
    gutter: 20pt,
    [
      V₀ 的出边:
      - V₀→V₂: 权重为 10
      - V₀→V₄: 权重为 30
      - V₀→V₅: 权重为 100

      V₁ 的出边:
      - V₁→V₂: 权重为 5

      V₂ 的出边:
      - V₂→V₃: 权重为 50
      - V₂→V₄: 权重为 10
    ],
    [
      V₃ 的出边:
      - V₃→V₄: 权重为 20
      - V₃→V₅: 权重为 60

      V₄ 的出边:
      - 无出边

      V₅ 的出边:
      - 无出边
    ],
  )]

#pagebreak()

对应的邻接矩阵如下：

$
  A = mat(
    0, 0, 10, 0, 30, 100;
    0, 0, 5, 0, 0, 0;
    0, 0, 0, 50, 10, 0;
    0, 0, 0, 0, 20, 60;
    0, 0, 0, 0, 0, 0;
    0, 0, 0, 0, 0, 0
  )
$

对应的邻接表如下：
$
  "AdjList": \
  &V₀: (V₂, 10), (V₄, 30), (V₅, 100);\
  &V₁: (V₂, 5);\
  &V₂: (V₃, 50), (V₄, 10);\
  &V₃: (V₄, 20), (V₅, 60);\
  &V₄: ();\
  &V₅: ();
$

== 模拟 Dijkstra 算法

#set par(first-line-indent: 2em)

在 Dijkstra 算法中，我们将邻接矩阵中的权重视为边的长度。对于无直接相连的顶点，权重为 $$infinity$$ (无穷大)。算法维护一个从源点到各顶点的当前最短距离估计数组 `dist`，以及一个已找到最短路径的顶点集合 `S`。

\
#figure(
  table(
    columns: 7,
    align: center + horizon,
    [], [$V_0$], [$V_1$], [$V_2$], [$V_3$], [$V_4$], [$V_5$],
    [$V_0$], [0], [0], [10], [0], [30], [100],
    [$V_1$], [0], [0], [5], [0], [0], [0],
    [$V_2$], [0], [0], [0], [50], [10], [0],
    [$V_3$], [0], [0], [0], [0], [20], [60],
    [$V_4$], [0], [0], [0], [0], [0], [0],
    [$V_5$], [0], [0], [0], [0], [0], [0],
  ),
  caption: "原始邻接矩阵",
)

#pagebreak()

#set par(first-line-indent: 0em)

将邻接矩阵转化为Dijkstra算法的输入图（0权重边转化为 $infinity$，对角线为0）：

#figure(
  table(
    columns: 7,
    align: center + horizon,
    [], [$V_0$], [$V_1$], [$V_2$], [$V_3$], [$V_4$], [$V_5$],
    [$V_0$], [0], [$infinity$], [10], [$infinity$], [30], [100],
    [$V_1$], [$infinity$], [0], [5], [$infinity$], [$infinity$], [$infinity$],
    [$V_2$], [$infinity$], [$infinity$], [0], [50], [10], [$infinity$],
    [$V_3$], [$infinity$], [$infinity$], [$infinity$], [0], [20], [60],
    [$V_4$], [$infinity$], [$infinity$], [$infinity$], [$infinity$], [0], [0],
    [$V_5$], [$infinity$], [$infinity$], [$infinity$], [$infinity$], [0], [0],
  ),
  caption: "Dijkstra算法输入图",
)

=== 以 $V_0$ 为源点

首先我们指定源点 $V_0$ ，并初始化 `dist` 数组为 [$infinity$], [10], [$infinity$], [30], [100]，`S` 集合为空。

#slanted-colorbox(title: "第一次迭代")[
  1. 从 `dist` 数组中找到最小值 $10$ ，对应的顶点为 $V_2$ ，将其加入集合 `S` 。

  $
    S = {V_2}
  $
]
#slanted-colorbox(title: "第二次迭代")[
  1. 从 `dist` 数组中找到最小值 $20$ ，对应的顶点为 $V_4$ ，将其加入集合 `S` 。

  $
    S = {V_2, V_4}
  $

  2. 检查 $V_4$ 的出边（无出边），无需更新 `dist` 数组。
]

#slanted-colorbox(title: "第三次迭代")[
  1. 从 `dist` 数组中找到最小值 $60$ ，对应的顶点为 $V_3$ ，将其加入集合 `S` 。

  $
    S = {V_2, V_4, V_3}
  $

  2. 检查 $V_3$ 的出边：
    - $V_3→V_4$: 60 + 20 = 80 ≥ 20（不更新）
    - $V_3→V_5$: 60 + 60 = 120 ≥ 100（不更新）
]

#slanted-colorbox(title: "第四次迭代")[
  1. 从 `dist` 数组中找到最小值 $100$ ，对应的顶点为 $V_5$ ，将其加入集合 `S` 。

  $
    S = {V_2, V_4, V_3, V_5}
  $

  2. 检查 $V_5$ 的出边（无出边），无需更新 `dist` 数组。
]

#pagebreak()

*最终结果*

此时未处理顶点仅剩 $V_1$，其距离值为 $∞$，说明从 $V_0$ 无法到达 $V_1$。最终结果：

#figure(
  table(
    columns: 8,
    align: center + horizon,
    [], [顶点], [$V_0$], [$V_1$], [$V_2$], [$V_3$], [$V_4$], [$V_5$],
    [最短距离], [0], [$∞$], [10], [60], [20], [100],
  ),
  caption: "最终结果",
)

#pagebreak()

=== 以 $V_1$ 为源点

我们以 $V_1$ 为源点，初始化 `dist` 数组为 [$infinity$], [0], [$infinity$], [$infinity$], [$infinity$]，`S` 集合为空。

#slanted-colorbox(title: "第一次迭代")[
  1. 从 `dist` 数组中找到最小值 $0$ ，对应的顶点为 $V_1$ ，将其加入集合 `S` 。

  $
    S = {V_1}
  $
]

#slanted-colorbox(title: "第二次迭代")[
  1. 从 `dist` 数组中找到最小值 $5$ ，对应的顶点为 $V_2$ ，将其加入集合 `S` 。

  $
    S = {V_1, V_2}
  $

  2. 检查 $V_2$ 的出边：
    - $V_2→V_3$: 5 + 50 = 55 ≥ $∞$（更新）
    - $V_2→V_4$: 5 + 10 = 15 ≥ $∞$（更新）
]

#slanted-colorbox(title: "第三次迭代")[
  1. 从 `dist` 数组中找到最小值 $15$ ，对应的顶点为 $V_4$ ，将其加入集合 `S` 。

  $
    S = {V_1, V_2, V_4}
  $

  2. 检查 $V_4$ 的出边（无出边），无需更新 `dist` 数组。
]

#slanted-colorbox(title: "第四次迭代")[
  1. 从 `dist` 数组中找到最小值 $55$ ，对应的顶点为 $V_3$ ，将其加入集合 `S` 。

  $
    S = {V_1, V_2, V_4, V_3}
  $

  2. 检查 $V_3$ 的出边：
    - $V_3→V_4$: 55 + 20 = 75 ≥ 15（不更新）
    - $V_3→V_5$: 55 + 60 = 115 ≥ $∞$（更新）
]

#slanted-colorbox(title: "第五次迭代")[
  1. 从 `dist` 数组中找到最小值 $100$ ，对应的顶点为 $V_5$ ，将其加入集合 `S` 。

  $
    S = {V_1, V_2, V_4, V_3, V_5}
  $

  2. 检查 $V_5$ 的出边（无出边），无需更新 `dist` 数组。
]

#pagebreak()

*最终结果*

此时未处理顶点仅剩 $V_0$，其距离值为 $∞$，说明从 $V_1$ 无法到达 $V_0$。最终结果：

#figure(
  table(
    columns: 8,
    align: center + horizon,
    [], [顶点], [$V_0$], [$V_1$], [$V_2$], [$V_3$], [$V_4$], [$V_5$],
    [最短距离], [$∞$], [0], [5], [55], [15], [100], [115],
  ),
  caption: "最终结果",
)

#pagebreak()

= Python 编程

== 代码实现

将原始矩阵保存在 `data.txt` 文件中，读取数据并构建邻接矩阵。然后实现 Dijkstra 算法，计算从源点到各顶点的最短路径。

具体代码以及注释见附件的 `work.py` 文件。

== 运行结果

#figure(
  image("image.png"), // 请确保 result.png 图片存在于正确路径
  caption: "运行结果",
)


\

#figure(
  image("Figure_1.png"), // 请确保 result.png 图片存在于正确路径
  caption: [$v_1, v_2$ 到各节点的最短路径占总的比],
)

#set image(width: 95%, height: auto)

#figure(
  image("Figure_2.png"), // 请确保 result.png 图片存在于正确路径
  caption: [$v_0$到各节点的最短路径动态图（完整动态演示请运行程序查看）],
)

#figure(
  image("Figure_3.png"), // 请确保 result.png 图片存在于正确路径
  caption: [$v_1$到各节点的最短路径动态图（完整动态演示请运行程序查看）],
)

= 附加挑战：寻找一种新的最短路算法

#set par(first-line-indent: 2em)

Dijkstra 算法已经被证明为*普遍最优* @haeupler2025universaloptimalitydijkstrabeyondworstcase，然而在某些情况下，Dijkstra 算法的性能可能不如其他算法。

比如在稀疏图中，Dijkstra 算法的时间复杂度为 $O(E + V log V)$，而 Bellman-Ford 算法的时间复杂度为 $O(V E)$。

对于交通领域而言，Dijkstra 算法的性能可能会受到交通流量、路况等因素的影响。比如在高峰期，某些路段的通行能力可能会降低，从而导致 Dijkstra 算法计算出的最短路径不再是最优路径。

同时，交通流量是快速变化的，Dijkstra 算法无法实时更新最短路径。因此，在交通领域，Dijkstra 算法可能不是最优选择。

所以，我尝试使用A\*算法来寻找最短路径。A\*算法是一种启发式搜索算法，它结合了 Dijkstra 算法和贪心算法的优点。A\* 算法使用启发式函数来估计从当前节点到目标节点的距离，从而更快地找到最短路径。

A\*算法的时间复杂度为 $O(E + V log V)$，在稀疏图中性能优于 Dijkstra 算法。

A\*算法的实现与 Dijkstra 算法类似，只需在 Dijkstra 算法的基础上添加*启发式函数*即可。具体实现见附件的 `A_star.py` 和 `work_modified.py` 文件。


#outline-colorbox(
  title: "启发式函数",
  color: "green",
  width: auto,
  radius: 2pt,
  centering: true,
)[
  #set par(first-line-indent: 0em)
  启发式函数是一个估计当前节点到目标节点距离的函数，用于指导 A\* 算法的搜索方向，从而更快找到最短路径。其选择对算法性能影响显著，常用的启发式函数包括曼哈顿距离、欧几里得距离和切比雪夫距离等。在交通领域，启发式函数可根据交通流量和路况动态调整，以提升算法效率。
]

#set par(first-line-indent: 0em)
在 `A_star.py` 中，我们实现了一个 简单的 A\* 算法。可以通过修改启发式函数来适应不同的场景。默认提供了曼哈顿距离和无（退化为 Dijkstra 算法）两种启发式函数。我们在 `work_modified.py` 中调用它。

*运行结果如下：*

#figure(
  image("a_star.png"), // 请确保 result.png 图片存在于正确路径
  caption: "A*算法运行结果，使用曼哈顿距离",
)

#bibliography("works.bib")
