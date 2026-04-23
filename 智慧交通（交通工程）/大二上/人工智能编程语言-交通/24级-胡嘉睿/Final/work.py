import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.patches import Patch
plt.rcParams['font.family'] = 'Noto Sans CJK JP'


def dijkstra(graph, start):
    n = len(graph)
    dist = [float('inf')] * n
    dist[start] = 0
    visited = [False] * n
    steps = [dist.copy()]  # 记录初始化状态

    for _ in range(n):
        # 寻找未访问的最小距离节点
        min_dist = float('inf')
        min_idx = -1
        for i in range(n):
            if not visited[i] and dist[i] < min_dist:
                min_dist = dist[i]
                min_idx = i

        if min_idx == -1:
            break

        visited[min_idx] = True

        # 更新邻居距离
        for j in range(n):
            if graph[min_idx][j] != float('inf') and not visited[j]:
                if dist[j] > dist[min_idx] + graph[min_idx][j]:
                    dist[j] = dist[min_idx] + graph[min_idx][j]

        steps.append(dist.copy())

    # 转换不可达节点为-1
    final_dist = [d if d != float('inf') else -1 for d in dist]
    steps.append(final_dist)
    return final_dist, steps


# 从 data.txt 文件读取邻接矩阵并转换
file_path = "./data.txt"

# 读取文件内容并转换为邻接矩阵
with open(file_path, "r") as f:
    raw_data = f.readlines()

graph = []
for line in raw_data:
    row = [float('inf') if int(x) == 0 else int(x)
           for x in line.strip().split(",")]
    graph.append(row)

# 对角线元素设置为0
for i in range(len(graph)):
    graph[i][i] = 0

# 从v0出发的最短路径
dist_v0 = dijkstra(graph, 0)[0]
print("从v0出发的最短路径:", dist_v0)

# 从v1出发的最短路径
dist_v1 = dijkstra(graph, 1)[0]
print("从v1出发的最短路径:", dist_v1)

# 统计分析
all_dists = np.array([dijkstra(graph, i)[0] for i in range(6)])
valid_dists = all_dists[all_dists != -1]

print("\n统计分析:")
print(f"平均最短距离: {np.mean(valid_dists):.2f}")
print(f"最大最短距离: {np.max(valid_dists)}")
print(f"最小最短距离: {np.min(valid_dists)}")
print(f"非连通节点比例: {np.sum(all_dists == -1)/all_dists.size:.2%}")

# 可视化分析
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

# 饼图1：v0 到各节点的距离占比 (不包含自身)
v0_distances = dijkstra(graph, 0)[0]
v0_labels = [f'v{i}' for i, d in enumerate(v0_distances) if i != 0 and d != -1]
v0_sizes = [d for i, d in enumerate(v0_distances) if i != 0 and d != -1]
v0_colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd']
ax1.pie(v0_sizes, labels=v0_labels, autopct='%1.1f%%',
        colors=v0_colors, startangle=90)
ax1.set_title("v0 到各节点的距离占比 (不包含自身)")

# 饼图2：v1 到各节点的距离占比 (不包含自身)
v1_distances = dijkstra(graph, 1)[0]
v1_labels = [f'v{i}' for i, d in enumerate(v1_distances) if i != 1 and d != -1]
v1_sizes = [d for i, d in enumerate(v1_distances) if i != 1 and d != -1]
v1_colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd']
ax2.pie(v1_sizes, labels=v1_labels, autopct='%1.1f%%',
        colors=v1_colors, startangle=90)
ax2.set_title("v1 到各节点的距离占比 (不包含自身)")

plt.tight_layout()

# 动态算法过程可视化 (分别统计 v0 和 v1)
fig_v0, ax_v0 = plt.subplots(figsize=(8, 6))
fig_v1, ax_v1 = plt.subplots(figsize=(8, 6))

all_steps = [dijkstra(graph, i)[1] for i in range(2)]  # 仅计算 v0 和 v1
max_steps = max(len(steps) for steps in all_steps)

# 可视化 v0 的过程
table_data_v0 = [[f"v{j}" for j in range(6)]]
table_data_v0 += [["" for _ in range(6)] for _ in range(max_steps)]

ax_v0.axis('off')
table_v0 = ax_v0.table(
    cellText=table_data_v0,
    colLabels=[f"v{i}" for i in range(6)],
    rowLabels=["步骤"] + [f"步骤{i}" for i in range(max_steps)],
    loc="center",
    cellLoc="center"
)
table_v0.auto_set_font_size(False)
table_v0.set_fontsize(10)
table_v0.scale(1.2, 1.2)


def update_v0(frame):
    step = frame % max_steps
    if step >= len(all_steps[0]):
        return table_v0,
    current = all_steps[0][step]
    for j in range(6):
        value = current[j]
        display = str(value) if value != float('inf') and value != - \
            1 else "∞" if value == float('inf') else "-1"
        color = 'lightgreen' if j == 0 else 'white'
        if step < len(all_steps[0]) - 1 and value != float('inf') and value != 0:
            color = 'lightyellow'
        elif step == len(all_steps[0]) - 1 and value != -1:
            color = 'lightblue'
        table_v0.get_celld()[(step + 1, j)].get_text().set_text(display)
        table_v0.get_celld()[(step + 1, j)].set_facecolor(color)
    plt.suptitle(f"Dijkstra算法过程可视化 (起点: v0, 当前步骤: {step})")
    return table_v0,


ani_v0 = animation.FuncAnimation(
    fig_v0, update_v0,
    frames=max_steps,
    interval=800,
    blit=False,
    repeat=False
)

# 可视化 v1 的过程
table_data_v1 = [[f"v{j}" for j in range(6)]]
table_data_v1 += [["" for _ in range(6)] for _ in range(max_steps)]

ax_v1.axis('off')
table_v1 = ax_v1.table(
    cellText=table_data_v1,
    colLabels=[f"v{i}" for i in range(6)],
    rowLabels=["初始化"] + [f"步骤{i}" for i in range(max_steps)],
    loc="center",
    cellLoc="center"
)
table_v1.auto_set_font_size(False)
table_v1.set_fontsize(10)
table_v1.scale(1.2, 1.2)


def update_v1(frame):
    step = frame % max_steps
    if step >= len(all_steps[1]):
        return table_v1,
    current = all_steps[1][step]
    for j in range(6):
        value = current[j]
        display = str(value) if value != float('inf') and value != - \
            1 else "∞" if value == float('inf') else "-1"
        color = 'lightgreen' if j == 1 else 'white'
        if step < len(all_steps[1]) - 1 and value != float('inf') and value != 0:
            color = 'lightyellow'
        elif step == len(all_steps[1]) - 1 and value != -1:
            color = 'lightblue'
        table_v1.get_celld()[(step + 1, j)].get_text().set_text(display)
        table_v1.get_celld()[(step + 1, j)].set_facecolor(color)
    plt.suptitle(f"Dijkstra算法过程可视化 (起点: v1, 当前步骤: {step})")
    return table_v1,


ani_v1 = animation.FuncAnimation(
    fig_v1, update_v1,
    frames=max_steps,
    interval=800,
    blit=False,
    repeat=False
)

plt.show()
