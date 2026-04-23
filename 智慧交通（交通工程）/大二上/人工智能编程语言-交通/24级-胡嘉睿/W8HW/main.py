import pandas as pd
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
import numpy as np
from matplotlib.animation import FuncAnimation

# Load the data
data = pd.read_csv('ICData.csv', encoding='utf-8')

# Convert transaction time to datetime and extract the hour
data['交易时间'] = pd.to_datetime(data['交易时间'])
data['Hour'] = data['交易时间'].dt.hour

# Group by hour to calculate swipe frequency
hourly_swipes = data.groupby('Hour').size()

# Group by hour and route number to analyze route activity
route_activity = data.groupby(['Hour', '线路号']).size().unstack(fill_value=0)

# Calculate the busiest and least busy hours for each route
route_peak = route_activity.idxmax(axis=0)
route_idle = route_activity.idxmin(axis=0)

# Perform clustering to group similar routes
kmeans = KMeans(n_clusters=3, random_state=42)
clusters = kmeans.fit_predict(route_activity)
route_activity['Cluster'] = clusters

# Line plot: Hourly swipe frequency
plt.figure(figsize=(12, 6))
hourly_swipes.plot(kind='line', color='blue', marker='o')
plt.title('Hourly Swipe Frequency (Line Plot)', fontsize=16)
plt.xlabel('Hour of the Day', fontsize=14)
plt.ylabel('Number of Swipes', fontsize=14)
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.savefig('line_plot_hourly_swipes.png')
plt.show()

# Bar plot: Route activity by hour
plt.figure(figsize=(12, 6))
route_activity.drop(columns='Cluster').plot(kind='bar', stacked=True, colormap='tab20', figsize=(12, 6))
plt.title('Route Activity by Hour (Bar Plot)', fontsize=16)
plt.xlabel('Hour of the Day', fontsize=14)
plt.ylabel('Number of Swipes', fontsize=14)
plt.legend(title='Route Number', bbox_to_anchor=(1.05, 1), loc='upper left')
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.savefig('bar_plot_route_activity.png')
plt.show()

# Dynamic plot: Animated line chart for route activity
fig, ax = plt.subplots(figsize=(12, 6))
lines = []
for route in route_activity.drop(columns='Cluster').columns:
    line, = ax.plot([], [], label=f'Route {route}')
    lines.append(line)

def init():
    ax.set_xlim(0, 23)
    ax.set_ylim(0, route_activity.drop(columns='Cluster').values.max())
    ax.set_title('Route Activity Trends (Animated)', fontsize=16)
    ax.set_xlabel('Hour of the Day', fontsize=14)
    ax.set_ylabel('Number of Swipes', fontsize=14)
    ax.legend(title='Route Number', loc='upper left')
    return lines

def update(frame):
    for line, route in zip(lines, route_activity.drop(columns='Cluster').columns):
        line.set_data(route_activity.index[:frame], route_activity[route].values[:frame])
    return lines

# ani = FuncAnimation(fig, update, frames=len(route_activity), init_func=init, blit=True)
# ani.save('animated_route_activity.gif', writer='pillow')
# plt.show()

from sklearn.decomposition import PCA

# Perform PCA to reduce dimensions and retain main features
pca = PCA(n_components=3)  # Retain 3 main components
reduced_features = pca.fit_transform(route_activity.drop(columns='Cluster'))

# Create a DataFrame for the reduced features
reduced_df = pd.DataFrame(reduced_features, index=route_activity.index, columns=['Feature 1', 'Feature 2', 'Feature 3'])

# Line plot: Reduced features
plt.figure(figsize=(12, 6))
for column in reduced_df.columns:
    plt.plot(reduced_df.index, reduced_df[column], marker='o', label=column)

plt.title('Route Activity with Main Features (Line Plot)', fontsize=16)
plt.xlabel('Hour of the Day', fontsize=14)
plt.ylabel('Feature Value', fontsize=14)
plt.legend(title='Features', loc='upper left')
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.savefig('line_plot_main_features.png')
plt.show()
