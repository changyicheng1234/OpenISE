import pandas as pd
import numpy as np
import os

# 导入交通数据
# 定义列的数据类型
data_types = {
    '交易类型': 'int64',
    '交易时间': 'str',
    '交易卡号': 'int64',
    '刷卡类型': 'int64',
    '线路号': 'int64',
    '车辆编号': 'int64',
    '上车站点': 'int64',
    '下车站点': 'int64',
    '驾驶员编号': 'int64',
    '运营公司编号': 'int64'
}

# 构造一个乘客搭乘站点数分析函数，计算不同线路乘客的平均公交搭乘站点数及其标准差；


def analyze_passenger_stops(data):
    # 计算每个乘客的搭乘站点数
    data['搭乘站点数'] = abs(data['下车站点'] - data['上车站点'])
    # 计算不同线路的平均搭乘站点数及其标准差
    result = data.groupby('线路号')['搭乘站点数'].agg(['mean', 'std']).reset_index()
    result.columns = ['线路号', '平均搭乘站点数', '标准差']
    return result

# 计算高峰小时刷卡量和公交刷卡量的高峰小时系数（*PHF5*和*PHF15*）


def calculate_peak_hour_factors(data):
    # 提取交易时间的小时和分钟
    data['小时'] = data['交易时间'].dt.hour
    data['分钟'] = data['交易时间'].dt.minute

    # 计算5分钟和15分钟时间段
    data['5分钟段'] = data['小时'] * 12 + data['分钟'] // 5
    data['15分钟段'] = data['小时'] * 4 + data['分钟'] // 15

    # 计算每个5分钟段和15分钟段的刷卡量
    transactions_5min = data.groupby('5分钟段').size()
    transactions_15min = data.groupby('15分钟段').size()

    # 找到峰值小时的刷卡量
    peak_hour_transactions = transactions_5min.rolling(12).sum().max()

    # 找到峰值5分钟和15分钟的刷卡量
    max_5min_transactions = transactions_5min.max()
    max_15min_transactions = transactions_15min.max()

    # 计算PHF5和PHF15
    phf5 = peak_hour_transactions / (4 * max_5min_transactions)
    phf15 = peak_hour_transactions / (12 * max_15min_transactions)

    return phf5, phf15

# 输出线路号1101-1120中各个车辆编号对应的的驾驶员编号，分别输出为20个txt文档，并命名为相应的线路号，并保存到一个文件夹中；


def save_driver_ids(data):
    # 创建文件夹
    folder_name = '线路驾驶员编号'
    if not os.path.exists(folder_name):
        os.makedirs(folder_name)

    # 筛选线路号在1101到1120之间的数据
    filtered_data = data[(data['线路号'] >= 1101) & (data['线路号'] <= 1120)]

    # 按照线路号分组并保存到文件
    for line_number, group in filtered_data.groupby('线路号'):
        driver_ids = group['驾驶员编号'].unique()
        file_path = os.path.join(folder_name, f"{line_number}.txt")
        with open(file_path, 'w') as f:
            for driver_id in driver_ids:
                f.write(f"{driver_id}\n")
        print(f"线路 {line_number} 的驾驶员编号已保存到 {file_path}")

# 确定服务乘客人次最多的10个司机、10条线路、10个站点和10台车辆。


def analyze_passenger_data(data):
    # 计算每个司机的乘客人次
    driver_passenger_count = data.groupby(
        '驾驶员编号').size().reset_index(name='乘客人次')
    top_10_drivers = driver_passenger_count.nlargest(10, '乘客人次')[
        ['驾驶员编号', '乘客人次']]

    # 计算每条线路的乘客人次
    line_passenger_count = data.groupby('线路号').size().reset_index(name='乘客人次')
    top_10_lines = line_passenger_count.nlargest(10, '乘客人次')[['线路号', '乘客人次']]

    # 计算每个站点的乘客人次
    station_passenger_count = data.groupby(
        '上车站点').size().reset_index(name='乘客人次')
    top_10_stations = station_passenger_count.nlargest(10, '乘客人次')[
        ['上车站点', '乘客人次']]

    # 计算每辆车的乘客人次
    vehicle_passenger_count = data.groupby(
        '车辆编号').size().reset_index(name='乘客人次')
    top_10_vehicles = vehicle_passenger_count.nlargest(10, '乘客人次')[
        ['车辆编号', '乘客人次']]

    return top_10_drivers, top_10_lines, top_10_stations, top_10_vehicles


def main():
    # 使用指定的数据类型读取数据
    data = pd.read_csv('ICData.csv', dtype=data_types)
    # 确保交易时间列是时间格式
    data['交易时间'] = pd.to_datetime(data['交易时间'], format='%Y/%m/%d %H:%M:%S')
    # 计算早上7点前的上车刷卡量
    morning_rides = data[data['交易时间'].dt.time < pd.to_datetime(
        '07:00:00', format='%H:%M:%S').time()].shape[0]
    # 计算晚上10点之后的上车刷卡量
    night_rides = data[data['交易时间'].dt.time > pd.to_datetime(
        '22:00:00', format='%H:%M:%S').time()].shape[0]
    print(f"早上7点前的上车刷卡量: {morning_rides}")
    print(f"晚上10点之后的上车刷卡量: {night_rides}")
    # 保存乘客搭乘站点数分析结果
    result = analyze_passenger_stops(data)
    print("不同线路乘客的平均公交搭乘站点数及其标准差（已保存）：\n", result)
    result.to_csv('乘客搭乘站点数分析结果.csv', index=False)
    # 计算高峰小时刷卡量和公交刷卡量的高峰小时系数
    phf5, phf15 = calculate_peak_hour_factors(data)
    print(f"PHF5: {phf5}")
    print(f"PHF15: {phf15}")
    # 保存司机编号
    save_driver_ids(data)
    print("司机编号已保存到文件夹[路线驾驶员编号]中。")
    # 调用函数并打印结果
    top_10_drivers, top_10_lines, top_10_stations, top_10_vehicles = analyze_passenger_data(
        data)
    print("服务乘客人次最多的10个司机：\n", top_10_drivers)
    print("服务乘客人次最多的10条线路：\n", top_10_lines)
    print("服务乘客人次最多的10个站点：\n", top_10_stations)
    print("服务乘客人次最多的10台车辆：\n", top_10_vehicles)


if __name__ == "__main__":
    main()
