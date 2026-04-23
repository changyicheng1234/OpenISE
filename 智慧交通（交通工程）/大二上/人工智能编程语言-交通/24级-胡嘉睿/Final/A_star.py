# File: A_star.py
import heapq

def a_star(graph, start, goal, heuristic):
    """
    A*算法实现，返回最短路径距离和步骤记录
    :param graph: 邻接矩阵表示的图
    :param start: 起始节点
    :param goal: 目标节点
    :param heuristic: 启发式函数，接受当前节点和目标节点作为参数
    :return: (最短路径距离列表, 算法步骤记录)
    """
    n = len(graph)
    open_heap = []
    heapq.heappush(open_heap, (0 + heuristic(start, goal), start))
    
    # 初始化距离数组和步骤记录
    g_scores = [float('inf')] * n
    g_scores[start] = 0
    steps = [[-1 if x == float('inf') else x for x in g_scores.copy()]]
    
    visited = [False] * n
    
    while open_heap:
        current_f, current = heapq.heappop(open_heap)
        
        if visited[current]:
            continue
        visited[current] = True
        
        # 记录当前步骤
        current_step = [-1 if x == float('inf') else x for x in g_scores.copy()]
        steps.append(current_step)
        
        # 提前终止条件
        if current == goal:
            break
        
        # 遍历所有邻接节点
        for neighbor in range(n):
            if graph[current][neighbor] == float('inf'):
                continue
                
            tentative_g = g_scores[current] + graph[current][neighbor]
            
            if tentative_g < g_scores[neighbor]:
                g_scores[neighbor] = tentative_g
                f_score = tentative_g + heuristic(neighbor, goal)
                heapq.heappush(open_heap, (f_score, neighbor))
        
        # 记录更新后的步骤
        updated_step = [-1 if x == float('inf') else x for x in g_scores.copy()]
        steps.append(updated_step)
    
    # 最终处理不可达节点
    final_dist = [-1 if x == float('inf') else x for x in g_scores]
    steps.append(final_dist)
    return final_dist, steps

def manhattan_distance(node, goal):
    """曼哈顿距离启发式函数示例"""
    return abs(node - goal)

def zero_heuristic(node, goal):
    """退化为Dijkstra算法的启发式函数"""
    return 0
