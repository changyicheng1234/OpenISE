import gurobipy as gp
from gurobipy import GRB, quicksum
import numpy as np
import pandas as pd

# ====================
# 读取需求数据
# ====================
filePath = r"E:\\all_1.xlsx"
try:
    data = pd.read_excel(filePath)  # 读取文件
    demand = data.to_numpy()
    print("需求数据成功读取。")
except Exception as e:
    print(f"读取需求数据时出错: {e}")
    exit()

# ====================
# 基本参数设置
# ====================
zone = 16            # 总客户数量（每个中转站有8个客户）
timeInterval = 30    # 时间间隔数量

# ====================
# 成本和容量等参数
# ====================
RentCost = 0.1       # 中转站的单位容积租赁费用
RentSize = 800       # 中转站的租赁容积
HoardCost = 0.1      # 单位货物在中转站的囤积成本

TravelCost1 = np.array([1, 3])
# 工厂 -> 中转站 i 的单位货物运输成本

TravelCost2 = 2 * np.array([2, 2, 2, 3, 3, 2, 4, 4,
                            7, 3, 4, 2, 3, 4, 7, 5])
# 工厂 -> 客户 j 的单位货物运输成本（16个客户）

TravelCost3 = 4
# 中转站 i -> 客户 j 的统一运输成本

TimeoutCost = 1000
# 客户j超时订单成本

DispatchCostForOneVehicle = 20
# 调度一次车辆的固定成本

# 车辆最大容量
Q = 100

# 每个中转站可用车辆总数
K = np.array([6, 6])

# 其他参数
Max1 = RentSize
Max2 = 100
Max3 = Q
M = 100000

# ====================
# 定义集合
# ====================
S = [1, 2]          # 中转站节点编号为1和2
P1 = list(range(3, 11))   # 中转站1对应的客户编号3-10（8个客户）
P2 = list(range(11, 19))  # 中转站2对应的客户编号11-18（8个客户）
P = P1 + P2         # 所有客户
N = S + P           # 所有节点（不包括仓库）

# ====================
# DispatchCost 的处理
# ====================
DispatchCost_matrix = np.array([
    [0, 11, 15, 13, 47, 2, 1, 2, 10],
    [14, 0, 8, 15, 37, 3, 14, 12, 9],
    [17, 8, 0, 23, 36, 9, 18, 16, 16],
    [13, 15, 22, 0, 42, 16, 11, 12, 7],
    [47, 36, 37, 42, 0, 39, 47, 45, 38],
    [12, 2, 8, 16, 38, 0, 10, 8, 9],
    [2, 12, 16, 11, 47, 9, 0, 2, 9],
    [3, 9, 14, 12, 46, 7, 2, 0, 8],
    [13, 10, 17, 7, 38, 11, 12, 11, 0],
    [0, 26, 34, 30, 14, 8, 25, 25, 32],
    [26, 0, 21, 8, 40, 35, 5, 7, 30],
    [34, 20, 0, 18, 48, 42, 15, 14, 15],
    [30, 8, 18, 0, 44, 38, 5, 5, 29],
    [14, 41, 48, 44, 0, 13, 40, 40, 43],
    [8, 34, 42, 38, 13, 0, 34, 33, 40],
    [25, 5, 16, 5, 39, 33, 0, 1, 25],
    [25, 7, 15, 6, 39, 33, 2, 0, 24],
    [31, 30, 16, 29, 43, 40, 25, 24, 0]
])

# ====================
# 创建模型
# ====================
model = gp.Model("Transportation_Optimization")

# ====================
# 决策变量
# ====================
# x[i,j,t]: 是否有车辆从中转站i到客户j在时刻t运输
x = model.addVars(S, P, range(timeInterval), vtype=GRB.BINARY, name="x")

# y1[i,t]: 从工厂到中转站 i 的货物量
y1 = model.addVars(S, range(timeInterval), lb=0, ub=Max1, vtype=GRB.CONTINUOUS, name="y1")

# y2[j,t]: 从工厂到客户 j 的货物量
y2 = model.addVars(P, range(timeInterval), lb=0, ub=Max2, vtype=GRB.CONTINUOUS, name="y2")

# y3[i,j,t]: 中转站 i -> 客户 j 的货物量，仅定义有效的 (i,j,t)
y3 = model.addVars([(i, j, t) for i in S for j in (P1 if i ==1 else P2) for t in range(timeInterval)],
                  lb=0, ub=Max3, vtype=GRB.CONTINUOUS, name="y3")

# l[j,t]: 客户 j 在时刻 t 的未完成订单量
l = model.addVars(P, range(timeInterval), lb=0, vtype=GRB.CONTINUOUS, name="l")

# h[i,t]: 中转站 i 在时刻 t 的实时存货量
h = model.addVars(S, range(timeInterval), lb=0, vtype=GRB.CONTINUOUS, name="h")

# k[i,t]: 中转站 i 在时刻 t 的可用车辆数量
k_ub = {(i, t): K[i - 1] for i in S for t in range(timeInterval)}
k = model.addVars(S, range(timeInterval), lb=0, ub=k_ub, vtype=GRB.INTEGER, name="k")

# li[j,t]: 时刻 t 车辆到客户 j 时未卸货前的剩余负载
li = model.addVars(P, range(timeInterval), lb=0, ub=Q, vtype=GRB.CONTINUOUS, name="li")

# z[i,j,t]: 车辆是否从客户i直接运送到客户j（表示多个客户服务）
z = model.addVars(P, P, range(timeInterval), vtype=GRB.BINARY, name="z")


# 取 DispatchCost1 和 DispatchCost2 的第一列作为调度成本
DispatchCost1 = DispatchCost_matrix[0:8, 0]  # 中转站1对应P1的调度成本
DispatchCost2 = DispatchCost_matrix[8:16, 0] # 中转站2对应P2的调度成本

# # 目标函数
# total_cost = (
#     quicksum(HoardCost * h[i, t] for i in S for t in range(timeInterval)) +  # 存货成本
#     quicksum(TravelCost1[i - 1] * y1[i, t] for i in S for t in range(timeInterval)) +  # 工厂到中转站运输成本
#     quicksum(TravelCost2[j - 3] * y2[j, t] for j in P for t in range(timeInterval)) +  # 工厂到客户运输成本
#     quicksum(TravelCost3 * y3[i, j, t] for (i, j, t) in y3.keys()) +  # 中转站到客户运输成本
#     quicksum(DispatchCost1[j - 3] * x[1, j, t] for j in P1 for t in range(timeInterval)) +  # 中转站1到客户调度成本
#     quicksum(DispatchCost2[j - 11] * x[2, j, t] for j in P2 for t in range(timeInterval)) +  # 中转站2到客户调度成本
#     quicksum(TimeoutCost * l[j, t] for j in P for t in range(timeInterval))  # 客户需求超时成本
# )

# model.setObjective(total_cost, GRB.MINIMIZE)
# dispatch_cost = quicksum(
#     DispatchCost_matrix[i - 1, j - 3] * x[i, j, t]  # 从中转站到客户的调度成本
#     for i in S for j in P for t in range(timeInterval)
#     if 3 <= j <= 10  # 客户3到客户10，确保索引不超出
# ) + quicksum(
#     DispatchCost_matrix[i - 1, j - 11] * x[i, j, t]  # 中转站到客户的调度成本
#     for i in S for j in P for t in range(timeInterval)
#     if 11 <= j <= 18  # 客户11到客户18，确保索引不超出
# )

# # 将dispatch_cost添加到目标函数中
# model.setObjective(
#     -(RentCost + HoardCost + TravelCost1 + TravelCost2 + TimeoutCost + dispatch_cost),
#     GRB.MAXIMIZE
# )

# 目标函数
total_cost = (
    quicksum(RentCost * RentSize for i in S for t in range(timeInterval)) +  # 中转站租赁成本（标量乘以租赁容量）
    quicksum(HoardCost * k[i, t] for i in S for t in range(timeInterval)) +  # 中转站存货成本
    quicksum(TravelCost1[i - 1] * y1[i, t] for i in S for t in range(timeInterval)) +  # 工厂到中转站运输成本
    quicksum(TravelCost2[j - 3] * y2[j, t] for j in P for t in range(timeInterval)) +  # 工厂到客户运输成本
    quicksum(TravelCost3 * y3[i, j, t] for (i, j, t) in y3.keys()) +  # 中转站到客户运输成本
    quicksum(DispatchCost1[j - 3] * x[1, j, t] for j in P1 for t in range(timeInterval)) +  # 中转站1到客户调度成本
    quicksum(DispatchCost2[j - 11] * x[2, j, t] for j in P2 for t in range(timeInterval)) +  # 中转站2到客户调度成本
    quicksum(TimeoutCost * l[j, t] for j in P for t in range(timeInterval))  # 客户需求超时成本
)

# 设置目标函数为最小化总成本
model.setObjective(total_cost, GRB.MINIMIZE)

# 车辆调度成本
dispatch_cost = quicksum(
    DispatchCost_matrix[i - 1, j - 3] * x[i, j, t]  # 从中转站到客户的调度成本
    for i in S for j in P for t in range(timeInterval)
    if 3 <= j <= 10  # 客户3到客户10，确保索引不超出
) + quicksum(
    DispatchCost_matrix[i - 1, j - 11] * x[i, j, t]  # 中转站到客户的调度成本
    for i in S for j in P for t in range(timeInterval)
    if 11 <= j <= 18  # 客户11到客户18，确保索引不超出
)

# 将dispatch_cost添加到目标函数中
model.setObjective(
    -(total_cost + dispatch_cost),
    GRB.MAXIMIZE
)


# ====================
# 约束条件
# ====================

# 1. 每个客户在每个时刻的需求必须被满足（包括未完成订单）
for j in P:
    for t in range(timeInterval):
        if t == 0:
            model.addConstr(
                y2[j, t] + quicksum(y3[i, j, t] for i in S if j in (P1 if i ==1 else P2)) + l[j, t] == demand[t, j - 3],
                name=f"Demand_Fulfill_{j}_{t}"
            )
        else:
            model.addConstr(
                y2[j, t] + quicksum(y3[i, j, t] for i in S if j in (P1 if i ==1 else P2)) + l[j, t] == l[j, t - 1] + demand[t, j - 3],
                name=f"Load_Balance_{j}_{t}"
            )

# 2. 中转站库存容量限制
for i in S:
    for t in range(timeInterval):
        model.addConstr(
            h[i, t] <= RentSize,
            name=f"Storage_Limit_{i}_{t}"
        )

# 3. y3 与 x 的车辆容量挂钩
for (i, j, t) in y3.keys():
    model.addConstr(
        y3[i, j, t] <= Q * x[i, j, t],
        name=f"Vehicle_Capacity_{i}_{j}_{t}"
    )

# 4. 动态库存平衡
for i in S:
    for t in range(timeInterval):
        if t == 0:
            model.addConstr(
                h[i, t] == y1[i, t] - quicksum(y3[i, j, t] for j in (P1 if i == 1 else P2)),
                name=f"Initial_Inventory_{i}_{t}"
            )
        else:
            model.addConstr(
                h[i, t] == h[i, t - 1] + y1[i, t] - quicksum(y3[i, j, t] for j in (P1 if i == 1 else P2)),
                name=f"Inventory_Balance_{i}_{t}"
            )

# 5. 未完成订单 l[j,t] 动态更新
for j in P:
    for t in range(timeInterval):
        if t == 0:
            model.addConstr(
                l[j, t] == demand[t, j - 3] - y2[j, t] - quicksum(y3[i, j, t] for i in S if j in (P1 if i == 1 else P2)),
                name=f"Initial_Load_{j}_{t}"
            )
        else:
            model.addConstr(
                l[j, t] == l[j, t - 1] + demand[t, j - 3] - y2[j, t] - quicksum(y3[i, j, t] for i in S if j in (P1 if i == 1 else P2)),
                name=f"Load_Balance_{j}_{t}"
            )

# 6. 客户服务约束：每个客户在每个时刻只能由一辆车服务
for j in P:
    for t in range(timeInterval):
        model.addConstr(
            quicksum(x[i, j, t] for i in S if j in (P1 if i == 1 else P2)) <= 1,
            name=f"One_Vehicle_Per_Customer_{j}_{t}"
        )

# 7. 车辆数量限制：中转站 i 每个时刻调度车辆 <= K[i - 1]
for i in S:
    for t in range(timeInterval):
        model.addConstr(
            quicksum(x[i, j, t] for j in (P1 if i == 1 else P2)) <= K[i - 1],
            name=f"Vehicle_Limit_{i}_{t}"
        )

# 8. 车辆可用性约束：k[i,t] 动态追踪可用车辆
for i in S:
    for t in range(timeInterval):
        if t == 0:
            model.addConstr(
                k[i, t] == K[i - 1] - quicksum(x[i, j, t] for j in (P1 if i == 1 else P2)),
                name=f"Initial_Vehicle_Availability_{i}_{t}"
            )
        else:
            model.addConstr(
                k[i, t] == k[i, t - 1] + quicksum(x[i, j, t - 1] for j in (P1 if i == 1 else P2)) - \
                            quicksum(x[i, j, t] for j in (P1 if i == 1 else P2)),
                name=f"Vehicle_Availability_{i}_{t}"
            )

# 9. 剩余负载约束
for j in P:
    for t in range(timeInterval):
        model.addConstr(
            li[j, t] >= quicksum(y3[i, j, t] for i in S if j in (P1 if i == 1 else P2)) - Q,
            name=f"Remaining_Load_{j}_{t}"
        )
        model.addConstr(
            li[j, t] >= 0,
            name=f"Nonnegative_Remaining_Load_{j}_{t}"
        )

# 10. 客户间运输路径约束（从一个客户到另一个客户）
for i in P:
    for j in P:
        if i != j:  # 如果i与j不同，才能建立路径
            for t in range(timeInterval):
                model.addConstr(z[i, j, t] <= quicksum(x[i, j, t] for i in S), name=f"Path_Condition_{i}_{j}_{t}")

# ====================
# 求解模型
# ====================
model.setParam('OutputFlag', 1)  # 启用详细日志以便调试
model.optimize()

# ====================
# 输出结果
# ====================
if model.status == GRB.OPTIMAL:
    print(f"最优解找到。目标值（成本）: {model.objVal}")
    
    # 输出决策变量的值
    # y1: 从仓库到中转站的运输量
    print("\n从仓库到中转站的运输量 y1:")
    for i in S:
        for t in range(timeInterval):
            if y1[i, t].X > 0:
                print(f"中转站 {i}, 时间 {t}: {y1[i, t].X}")

    # y2: 从仓库到客户的运输量
    print("\n从仓库到客户的运输量 y2:")
    for i in P:
        for t in range(timeInterval):
            if y2[i, t].X > 0:
                print(f"客户 {i}, 时间 {t}: {y2[i, t].X}")

    # y3: 从中转站到客户的运输量
    print("\n从中转站到客户的运输量 y3:")
    for i in S:  # 对每个中转站
        for j in (P1 if i == 1 else P2):  # 对每个客户
            for t in range(timeInterval):  # 对每个时间段
                if (i, j, t) in y3 and y3[i, j, t].X > 0:  # 检查变量是否存在且大于0
                    print(f"中转站 {i} 到客户 {j}, 时间 {t}: {y3[i, j, t].X}")

    # 输出 x (车辆路径)
    print("\n车辆路径 x:")
    for i in S:  # 对每个中转站
        for j in P:  # 对每个客户
            for t in range(timeInterval):  # 对每个时间段
                if (i != j) and (i, j, t) in x and x[i, j, t].X > 0.5:  # 检查是否有车辆路径且不为自环
                    print(f"时间 {t}: 从中转站 {i} 到客户 {j} 有车辆")
else:
    print("未找到最优解。")

