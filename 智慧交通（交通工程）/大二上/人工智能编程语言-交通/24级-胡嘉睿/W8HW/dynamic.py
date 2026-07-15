import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import rcParams

# 设置中文字体
rcParams['font.family'] = 'SimHei'

# 数据加载与预处理
df = pd.read_csv('ICData.csv')
df['交易时间'] = pd.to_datetime(df['交易时间'], errors='coerce')
df['小时'] = df['交易时间'].dt.hour

# 创建按小时和线路分组的统计数据
hourly_route = df.groupby(['小时', '线路号']).size().unstack().fillna(0)

# 动态图初始化
fig, ax = plt.subplots(figsize=(12, 8))

def animate(hour):
    ax.clear()
    current_data = hourly_route.loc[hour].nlargest(10)
    
    bars = ax.barh(current_data.index.astype(str), 
                 current_data.values,
                 color=plt.cm.tab20c(range(10)))
    
    ax.set_title(f'每小时线路刷卡量排名（{hour:02d}:00）')
    ax.set_xlabel('交易次数')
    ax.set_xlim(0, current_data.max()*1.1)
    ax.bar_label(bars, fmt='%.0f', padding=3)
    
    # 返回所有需要更新的图形元素列表
    return bars, ax.title, ax.xaxis.label, ax.yaxis.label

# 修改blit参数为False
ani = animation.FuncAnimation(fig=fig,
                             func=animate,
                             frames=range(0,24),
                             interval=500,
                             blit=False)  # 关闭blit加速

# 保存动画
ani.save('dynamic_route_ranking.gif', 
        writer='pillow', 
        fps=2,  # 2帧/秒
        dpi=100)

plt.close()