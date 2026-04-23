import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# 确保 images 目录存在
if not os.path.exists('./images'):
    os.makedirs('./images')

# 设置中文字体
# plt.rcParams['font.family'] = 'SimHei' #windows
plt.rcParams['font.family'] = 'Noto Sans CJK JP' #linux

# 1. 数据加载与预处理
df = pd.read_csv('ICData.csv',
                names=['交易类型', '交易时间', '交易卡号', '刷卡类型', 
                       '线路号', '车辆编号', '上车站点', '下车站点', 
                       '驾驶员编号', '运营公司编号'],
                parse_dates=['交易时间'],
                date_format='%Y-%m-%d %H:%M:%S',  # 使用date_format代替date_parser
                low_memory=False)

# 转换日期格式（添加错误处理）
df['交易时间'] = pd.to_datetime(df['交易时间'], errors='coerce')
df['小时'] = df['交易时间'].dt.hour

# 图1: 时间序列分析
plt.figure(figsize=(10, 8))
hourly_counts = df.groupby('小时').size()
sns.lineplot(x=hourly_counts.index, y=hourly_counts.values, marker='o')
plt.xticks(range(0,24))
plt.title('每小时刷卡频率趋势')
plt.xlabel('小时')
plt.ylabel('交易次数')
plt.tight_layout()
plt.savefig('./images/hourly_trend.png')
plt.close()

# 图1.1: 交易时间饼图（包含高峰时段统计）
plt.figure(figsize=(10, 8))
time_bins = pd.cut(df['小时'], 
                   bins=[0, 6, 9, 12, 16, 19, 23, 24], 
                   labels=['凌晨', '早高峰', '上午', '下午', '晚高峰', '晚上', '深夜'], 
                   right=False)
time_distribution = time_bins.value_counts().sort_index()
plt.pie(time_distribution, 
        labels=time_distribution.index, 
        autopct='%1.1f%%', 
        startangle=90)
plt.title('交易时间分布（包含高峰时段）')
plt.tight_layout()
plt.savefig('./images/transaction_time_pie_peak.png')
plt.close()

# 图2: 线路分析
plt.figure(figsize=(10, 8))
top_routes = df['线路号'].value_counts().nlargest(10)
sns.barplot(x=top_routes.values, y=top_routes.index.astype(str))
plt.title('TOP10高频线路')
plt.xlabel('交易次数')
plt.tight_layout()
plt.savefig('./images/top_routes.png')
plt.close()

# 为每个高频线路绘制时间分析图
top_routes_list = top_routes.index.tolist()
for route in top_routes_list:
    plt.figure(figsize=(10, 8))
    route_data = df[df['线路号'] == route]
    hourly_counts = route_data.groupby('小时').size()
    sns.lineplot(x=hourly_counts.index, y=hourly_counts.values, marker='o')
    plt.xticks(range(0, 24))
    plt.title(f'线路 {route} 每小时刷卡频率趋势')
    plt.xlabel('小时')
    plt.ylabel('交易次数')
    plt.tight_layout()
    plt.savefig(f'./images/route_{route}_hourly_trend.png')
    plt.close()

# 图3: 驾驶员工作时间分析
plt.figure(figsize=(10, 8))

# 创建驾驶员子集数据 - 选择前10名驾驶员
driver_subset = df[['驾驶员编号', '小时']].copy()
top_drivers = driver_subset['驾驶员编号'].value_counts().nlargest(10).index
driver_subset = driver_subset[driver_subset['驾驶员编号'].isin(top_drivers)]

# 过滤掉缺失值
driver_subset = driver_subset.dropna(subset=['驾驶员编号', '小时'])

# 确保'驾驶员编号'被视为分类变量
driver_subset['驾驶员编号'] = driver_subset['驾驶员编号'].astype(str)

sns.boxplot(x='驾驶员编号', y='小时', data=driver_subset)
plt.title('驾驶员工作时间分布')
plt.xlabel('驾驶员编号')
plt.ylabel('小时')
plt.tight_layout()
plt.savefig('./images/driver_hours.png')
plt.close()

# 图4: 交易类型分析
plt.figure(figsize=(10, 8))
txn_counts = df['交易类型'].value_counts()
plt.pie(txn_counts, 
        labels=txn_counts.index, 
        autopct='%1.1f%%',
        startangle=90)
plt.title('交易类型分布')
plt.tight_layout()
plt.savefig('./images/transaction_types.png')
plt.close()

# 图5: 运营公司运行指标对比
# Example definition of company_melt (replace with actual data preparation logic)
company_metrics = {
    '运营公司编号': ['A', 'B', 'C', 'D'],
    '指标': ['指标1', '指标2', '指标1', '指标2'],
    'value': [100, 200, 150, 250]
}
company_melt = pd.DataFrame(company_metrics)

# 删除重复的barplot调用
sns.barplot(x='运营公司编号', y='value', hue='指标', data=company_melt)
plt.title('运营公司运行指标对比')
plt.xlabel('运营公司')
plt.ylabel('数值')
plt.legend(title='指标类型')
plt.tight_layout()
plt.savefig('./images/company_comparison.png')
plt.close()

# 图6: TOP10人流量站点
plt.figure(figsize=(10, 8))
# 计算站点流量（上车站点计数）
station_flow = df['上车站点'].value_counts().nlargest(10)
sns.barplot(x=station_flow.values, y=station_flow.index.astype(str))
plt.title('TOP10人流量站点')
plt.xlabel('人流量')
plt.tight_layout()
plt.savefig('./images/top_stations.png')
plt.close()

# 图7: 交易类型-时间分布
plt.figure(figsize=(10, 8))
# 创建交易类型与小时的交叉表
hourly_txn = pd.crosstab(df['小时'], df['交易类型'])
hourly_txn.plot(kind='bar', stacked=True, ax=plt.gca())
plt.title('交易类型-时间分布')
plt.xlabel('小时')
plt.ylabel('交易次数')
plt.tight_layout()
plt.savefig('./images/transaction_time_distribution.png')
plt.close()

print("所有图表已成功保存到 ./images 目录")