# File: work_modified.py
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from A_star import a_star, zero_heuristic

# 从 data.txt 文件读取邻接矩阵并转换
with open("data.txt", "r") as f:
    raw_data = f.readlines()

graph = []
for line in raw_data:
    row = [float('inf') if int(x) == 0 else int(x)
           for x in line.strip().split(",")]
    graph.append(row)
# 对角线元素设置为0
for i in range(len(graph)):
    graph[i][i] = 0

# 定义简单启发式函数（示例
def simple_heuristic(node, goal):
    return abs(node - goal)


# ① 从v0出发到各节点的最短路径（使用A*算法）
dist_v0 = []
for j in range(6):
    dist, _ = a_star(graph, 0, j, simple_heuristic)
    dist_v0.append(dist[j])
print("从v0出发的最短路径:", dist_v0)

# ② 从v1出发到各节点的最短路径
dist_v1 = []
for j in range(6):
    dist, _ = a_star(graph, 1, j, simple_heuristic)
    dist_v1.append(dist[j])
print("从v1出发的最短路径:", dist_v1)

# 统计分析
all_dists = []
for i in range(6):
    row = []
    for j in range(6):
        dist, _ = a_star(graph, i, j, zero_heuristic)
        row.append(dist[j])
    all_dists.append(row)
all_dists = np.array(all_dists)
valid_dists = all_dists[all_dists != -1]

print("\n统计分析:")
print(f"平均最短距离: {np.mean(valid_dists):.2f}")
print(f"最大最短距离: {np.max(valid_dists)}")
print(f"最小最短距离: {np.min(valid_dists)}")
print(f"非连通节点比例: {np.sum(all_dists == -1)/all_dists.size:.2%}")
