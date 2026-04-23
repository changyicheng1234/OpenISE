import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib import rcParams

# 设置中文环境
rcParams['font.family'] = 'SimHei'

# 修改数据预处理部分
# 数据加载与预处理
df = pd.read_csv('ICData.csv',
                names=['交易类型', '交易时间', '交易卡号', '刷卡类型', 
                       '线路号', '车辆编号', '上车站点', '下车站点', 
                       '驾驶员编号', '运营公司编号'],
                parse_dates=['交易时间'],
                low_memory=False)

# 修复1：明确指定时间格式并添加小时列
df['交易时间'] = pd.to_datetime(df['交易时间'], format='%Y/%m/%d %H:%M:%S', errors='coerce')
df['小时'] = df['交易时间'].dt.hour  # 新增小时列

# 修复2：调整运营公司分析的分组逻辑
# 1. 运营公司对比分析（分组条形图）
plt.figure(figsize=(18, 12))

# 计算运营公司指标
company_stats = df.groupby('运营公司编号').agg(
    车辆运行频率=('车辆编号', 'nunique'),  # 改为统计不同车辆数
    工作时长=('小时', lambda x: x.max() - x.min() if not x.empty else 0)
).reset_index()

# 数据重塑为长格式
company_melt = company_stats.melt(id_vars='运营公司编号', 
                                value_vars=['车辆运行频率', '工作时长'],
                                var_name='指标')

plt.subplot(2,2,1)
sns.barplot(x='运营公司编号', y='value', hue='指标', data=company_melt)
plt.title('运营公司运行指标对比')
plt.xlabel('运营公司')
plt.ylabel('数值')
plt.legend(title='指标类型')

# 2. 站点人流量分析（条形图）
plt.subplot(2,2,2)
station_flow = df.melt(value_vars=['上车站点', '下车站点']) \
                .groupby('value').size() \
                .nlargest(10)
sns.barplot(x=station_flow.values, y=station_flow.index.astype(str))
plt.title('TOP10人流量站点')
plt.xlabel('人流量')

# 3. 交易类型关联分析（堆叠柱状图）
plt.subplot(2,1,2)

# 时间关联分析
hourly_txn = df.groupby(['小时', '交易类型']).size().unstack()
hourly_txn.plot(kind='bar', stacked=True, ax=plt.gca())
plt.title('交易类型-时间分布')
plt.xlabel('小时')
plt.ylabel('交易次数')

plt.tight_layout()
plt.savefig('visualization2_results.png', bbox_inches='tight')
plt.show()