import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
plt.rcParams['font.family'] = 'SimHei'  # 替换为你选择的字体
# 1. 数据加载与预处理（修正参数名和日期解析）
df = pd.read_csv('ICData.csv',
                names=['交易类型', '交易时间', '交易卡号', '刷卡类型', 
                       '线路号', '车辆编号', '上车站点', '下车站点', 
                       '驾驶员编号', '运营公司编号'],
                parse_dates=['交易时间'],
                low_memory=False)

# 转换日期格式（添加错误处理）
df['交易时间'] = pd.to_datetime(df['交易时间'], errors='coerce')
df['小时'] = df['交易时间'].dt.hour

# 2. 创建画布（修正函数名）
fig = plt.figure(figsize=(20, 16))
fig.suptitle('公交IC卡数据分析可视化', fontsize=18, y=1.02)

# 3. 时间序列分析（修正绘图逻辑）
ax1 = plt.subplot(2,2,1)
hourly_counts = df.groupby('小时').size()
sns.lineplot(x=hourly_counts.index, y=hourly_counts.values, marker='o', ax=ax1)
plt.xticks(range(0,24))
plt.title('每小时刷卡频率趋势')
plt.xlabel('小时')
plt.ylabel('交易次数')

# 4. 线路分析（修正分组方法）
ax2 = plt.subplot(2,2,2)
top_routes = df['线路号'].value_counts().nlargest(10)
sns.barplot(x=top_routes.values, y=top_routes.index.astype(str), ax=ax2)
plt.title('TOP10高频线路')
plt.xlabel('交易次数')

# 5. 驾驶员工作时间分析（修正箱线图参数）
ax3 = plt.subplot(2,2,3)
top_drivers = df['驾驶员编号'].value_counts().nlargest(5).index
driver_subset = df[df['驾驶员编号'].isin(top_drivers)]
sns.boxplot(x='驾驶员编号', y='小时', data=driver_subset, ax=ax3)
plt.title('驾驶员工作时间分布')

# 6. 交易类型分析（修正饼图参数）
ax4 = plt.subplot(2,2,4)
txn_counts = df['交易类型'].value_counts()
plt.pie(txn_counts, 
        labels=txn_counts.index, 
        autopct='%1.1f%%',
        startangle=90)
plt.title('交易类型分布')

# 7. 调整布局并保存（修正拼写错误）
plt.tight_layout()
plt.savefig('visualization_results.png', bbox_inches='tight')
plt.show()