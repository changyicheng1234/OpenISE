# ⚠️注意
 我们计划将项目完整开源至Github，当前README为方便助教测试进行的示例内容
 
 实际测试部分在安装部署部分

# AI健康饮食助手 🥗🤖

一个基于Flask和Agently AI技术的智能健康饮食推荐系统，为用户提供个性化的营养建议、食谱分析和健康饮食计划。

## 📋 目录

- [功能特性](#功能特性)
- [技术栈](#技术栈)
- [安装部署](#安装部署)
- [使用指南](#使用指南)
- [项目结构](#项目结构)
- [API接口](#api接口)
- [配置说明](#配置说明)
- [开发指南](#开发指南)
- [问题反馈](#问题反馈)
- [许可证](#许可证)

## 🌟 功能特性

### 🎯 核心功能
- **智能饮食推荐** - 基于个人健康目标和生活方式的个性化饮食建议
- **营养成分分析** - 详细分析一日三餐的营养成分和健康指标
- **食谱生成** - AI驱动的健康食谱生成，支持各种饮食偏好
- **减重计划** - 专业的减重饮食计划和进度跟踪
- **外卖推荐** - 健康外卖选择建议和营养评估
- **经济饮食** - 预算友好的健康饮食方案

### 💡 智能特性
- **AI驱动** - 集成百度ERNIE模型，提供专业营养建议
- **个性化定制** - 根据用户年龄、性别、BMI等个人信息定制方案
- **实时分析** - 即时营养成分分析和健康评估
- **数据持久化** - 用户配置文件本地存储，提升用户体验

### 🎨 用户体验
- **响应式设计** - 完美适配桌面和移动设备
- **现代化UI** - 清洁、直观的用户界面设计
- **流畅动画** - 页面切换和交互动画效果
- **多页面架构** - 模块化功能页面，便于导航

## 🛠 技术栈

### 后端技术
- **Flask** - Python轻量级Web框架
- **Agently AI** - AI代理框架，集成百度ERNIE模型
- **Python 3.8+** - 主要开发语言

### 前端技术
- **HTML5/CSS3** - 现代化前端标准
- **JavaScript (ES6+)** - 交互逻辑和API调用
- **Bootstrap** - 响应式UI框架
- **自定义CSS模块** - 精细化样式控制

### AI技术
- **百度ERNIE模型** - 自然语言处理和智能推荐
- **Agently框架** - AI代理管理和配置

## 🚀 安装部署

### 环境要求
- Python 3.8 或更高版本
- pip 包管理器
- 百度AI Studio API密钥

### 1. 创建虚拟环境
```bash
conda create -n ai_health_diet python=3.8
conda activate ai_health_diet
```

### 2. 安装依赖
```bash
pip install flask
pip install agently
pip install nest-asyncio
```

### 3. 配置API密钥
编辑 `agent_config.py` 文件，替换为你的百度AI Studio API密钥：
```python
.set_settings("model.ERNIE.auth", {"aistudio": "your_api_key_here"})

⚠️注意：
为方便助教测试我们的效果，文件中已经给出具体的API密钥配置示例。（.set_settings("model.ERNIE.auth", {"aistudio": "b44dc285697fcdbee35dc37c1cee12fe06fb8d83"})）
```

### 4. 启动应用
```bash
python main.py
```

应用将在 `http://localhost:5000` 启动。

## 📖 使用指南

### 1. 首页导航
- 访问主页查看功能介绍和轮播展示
- 通过导航栏快速访问各个功能模块

### 2. 个人档案设置
1. 点击"个人档案"页面
2. 填写基本信息：姓名、年龄、性别、身高、体重
3. 设置健康目标：减重、增重、维持健康等
4. 保存配置以获得个性化推荐

### 3. 饮食推荐
1. 进入"饮食推荐"页面
2. 选择健康目标和饮食偏好
3. 描述生活方式和特殊需求
4. 获取AI生成的个性化饮食建议

### 4. 营养分析
1. 访问"营养分析"页面
2. 输入一日三餐的具体食物和分量
3. 获得详细的营养成分分析报告
4. 查看改善建议和健康指标

### 5. 食谱生成
1. 进入"食谱生成"页面
2. 选择菜品类型和烹饪偏好
3. 设置制作时间和难度要求
4. 获取详细的制作步骤和营养信息

### 6. 减重计划
1. 访问"减重计划"页面
2. 输入目标体重和时间期限
3. 描述当前饮食习惯和运动情况
4. 获得专业的减重饮食计划

## 📁 项目结构

```
AI_health_diet/
├── main.py                 # Flask应用主文件
├── diet_agent.py          # AI代理和角色定义
├── agent_config.py        # Agently配置文件
├── workflow.py            # 工作流程文件
├── README.md              # 项目文档
├── requirements.txt       # 依赖包列表
├── .Agently/             # Agently配置目录
├── static/               # 静态资源
│   ├── css/             # 样式文件
│   │   ├── style.css    # 主样式文件
│   │   ├── variables.css # CSS变量定义
│   │   ├── button-fix.css # 按钮样式修复
│   │   ├── footer-fix.css # 页脚样式修复
│   │   └── ...          # 其他样式模块
│   ├── js/              # JavaScript文件
│   └── images/          # 图片资源
└── templates/           # HTML模板
    ├── base.html        # 基础模板
    ├── index.html       # 首页
    ├── recommend.html   # 饮食推荐页面
    ├── analyze.html     # 营养分析页面
    ├── recipe.html      # 食谱生成页面
    ├── weight_loss.html # 减重计划页面
    ├── profile.html     # 个人档案页面
    ├── economical.html  # 经济饮食页面
    └── takeout.html     # 外卖推荐页面
```

## 🔌 API接口

### 饮食推荐 API
```
POST /recommend
Content-Type: application/json

{
  "health_goal": "减重",
  "dietary_preferences": "素食主义",
  "lifestyle": "久坐办公"
}
```

### 营养分析 API
```
POST /analyze
Content-Type: application/json

{
  "breakfast": "燕麦粥一碗，苹果一个",
  "lunch": "鸡胸肉150g，蔬菜沙拉",
  "dinner": "蒸蛋羹，青菜汤"
}
```

### 食谱生成 API
```
POST /recipe
Content-Type: application/json

{
  "dish_type": "主菜",
  "cooking_method": "蒸煮",
  "prep_time": "30分钟内",
  "dietary_restrictions": "低盐"
}
```

### 减重计划 API
```
POST /weight_loss
Content-Type: application/json

{
  "current_weight": 70,
  "target_weight": 60,
  "time_frame": "3个月",
  "activity_level": "中等"
}
```

## ⚙️ 配置说明

### AI模型配置
在 `agent_config.py` 中配置AI模型参数：
- `current_model`: 使用的AI模型（默认：ERNIE）
- `model.ERNIE.auth`: 百度AI Studio认证信息
- `model.ERNIE.options.model`: 具体模型版本（默认：ernie-speed）

### Flask应用配置
在 `main.py` 中可以配置：
- `app.secret_key`: 会话密钥（生产环境请使用安全密钥）
- 调试模式和端口设置

### 样式定制
CSS模块化设计，可在 `static/css/` 目录下自定义：
- `variables.css`: 全局CSS变量（颜色、字体等）
- `style.css`: 主要样式定义
- 各个功能模块的独立样式文件

## 👨‍💻 开发指南

### 代码结构说明

#### AI代理系统
`diet_agent.py` 包含多个专业AI角色：
- `SMART_DIET_ROLE`: 智能饮食推荐助手
- `DIET_ANALYZE_ROLE`: 营养分析助手
- `DIET_RECIPE_GENERATOR_ROLE`: 食谱生成助手
- `WEIGHT_DIET_ROLE`: 减重计划助手
- `TAKEOUT_RECOMMENDATION_ROLE`: 外卖推荐助手
- `ECONOMICAL_ROLE`: 经济饮食助手

#### 前端架构
- **模块化CSS**: 每个功能页面有独立的样式文件
- **响应式设计**: 支持桌面和移动设备
- **组件化结构**: 可重用的UI组件和布局

### 添加新功能
1. 在 `diet_agent.py` 中定义新的AI角色
2. 在 `main.py` 中添加对应的路由处理
3. 创建新的HTML模板
4. 添加相应的CSS样式
5. 更新导航菜单

### 调试技巧
- 启用Flask调试模式查看详细错误信息
- 检查浏览器控制台的JavaScript错误
- 使用网络面板监控API请求和响应

## 🎨 UI/UX 特性

### 视觉设计
- **配色方案**: 以健康绿色（#4CAF50）为主色调
- **字体系统**: 现代化中文字体适配
- **图标系统**: 一致的功能图标设计

### 交互体验
- **页面转换**: 流畅的页面切换动画
- **表单反馈**: 实时输入验证和状态提示
- **加载状态**: 优雅的加载动画和提示
- **响应式布局**: 自适应不同屏幕尺寸

### 可访问性
- **键盘导航**: 支持Tab键导航
- **语义化HTML**: 正确的标签语义
- **对比度**: 符合WCAG标准的颜色对比度

## 🚧 已知问题和限制

### 当前限制
- API密钥需要手动配置
- 数据存储仅限于浏览器localStorage
- 暂不支持用户账户系统

### 计划改进
- 数据库集成（用户数据持久化）
- 用户认证和授权系统
- 移动应用版本
- 多语言支持

## 🤝 贡献指南

1. Fork 项目仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 提交Pull Request

## 📝 更新日志

### v1.0.0 (当前版本)
- ✅ 基础AI饮食推荐功能
- ✅ 营养成分分析系统
- ✅ 食谱生成功能
- ✅ 减重计划制定
- ✅ 响应式UI设计
- ✅ 模块化CSS架构

### 计划功能
- 🔄 用户数据库集成
- 🔄 移动端优化
- 🔄 多语言支持
- 🔄 社交分享功能

## 📞 问题反馈
⚠️注意：在此我们仅作为示例完成这部分的内容。
如果你在使用过程中遇到问题或有改进建议，请通过以下方式联系：

- 创建 GitHub Issue
- 发送邮件至: [your-email@example.com]
- 微信群: [群号或二维码]

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

---

## 🙏 致谢

感谢以下技术和服务的支持：
- [百度AI Studio](https://aistudio.baidu.com/) - 提供ERNIE模型支持
- [Agently Framework](https://github.com/Maplemx/Agently) - AI代理开发框架
- [Flask](https://flask.palletsprojects.com/) - Web应用框架
- [Bootstrap](https://getbootstrap.com/) - UI组件库

---

<div align="center">
  <p>Made with ❤️ for healthy living</p>
  <p>© 2025 AI健康饮食助手. All rights reserved.</p>
</div>
