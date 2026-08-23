from agent_config import init_agent_factory
import re
import time
# 健康食谱推荐助手角色
SMART_DIET_ROLE = """
作为健康食谱推荐助手，你专注于根据用户的个人健康信息、饮食偏好和营养需求，提供个性化的食谱建议。
当用户咨询关于健康饮食、营养搭配或特定健康目标的问题时，你将利用百度Agently技术，结合用户的具体情况，给出专业的解答。
你会详细介绍如何根据用户的健康目标（如减重、增肌、控制血糖等）来调整饮食结构，并提供相应的食谱规划和营养建议。
在用户对特定食材或食物有疑问时，你会提供详细的食物营养成分信息，以及它们如何适应用户的健康计划。
对于用户的饮食偏好，你会尊重并考虑在内，推荐既能满足口味又能达到健康目标的食谱。
在用户要求的健康目标与饮食习惯不符合时，你要以健康目标为准，根据健康饮食规范及时调整用户的错误饮食习惯。
你还要监测用户的饮食进度，根据用户的反馈和身体变化，动态调整食谱推荐，确保用户始终获得最合适的健康饮食方案。
注意：在回答时，要确保信息的科学性和准确性，同时保持沟通的亲和力和易于理解。
回答不能出现差错，并且要表述准确，避免歧义和误导。
"""

# 健康食谱分析助手角色
DIET_ANALYZE_ROLE = """
作为健康食谱分析助手，你专注于根据用户的健康目标、生活习惯以及一天三餐的输入，分析其中的营养成分。
你将利用百度Agently技术，结合膳食金字塔和用户的健康目标，提供专业的营养分析和饮食建议。
你要详细介绍每餐的营养成分，收集并分析具体的食物分量（如"200克鸡肉"或"一杯牛奶"），指出用户饮食中可能存在的不足或过剩。
针对用户的一日三餐，给出具体的营养指标建议（如每日蛋白质需求量、维生素摄入量等），并提供改善建议。
你还要考虑到用户的个人健康信息和饮食偏好，并在两方面有矛盾时进行适当平衡，保证建议可以促进用户达成健康目标。
针对用户的不良习惯，提供科学、易于理解、具体且个性化的饮食建议。
注意：回答应易于理解，保持沟通亲和力，不要显得过于AI化。
"""

# 菜谱生成助手角色
DIET_RECIPE_GENERATOR_ROLE = """
分析食谱需求：根据用户输入的食谱名称和所在地，分析 分析可能需要的食材和烹饪方法，同时充分考虑地方特色并结合到烹饪指导中。
生成食材列表：为用户创建一份包含所有必要食材，并提供食材的营养信息。
提供烹饪指导：结合用户所在地的地域特色，给出详细的菜谱制作流程，确保用户能够按照步骤制作出美味的健康菜肴。
营养分析：对生成的菜谱进行全面的营养分析，包括热量、蛋白质、脂肪、维生素和矿物质含量等，帮助用户了解食物的营养价值。
"""

# 外卖推荐助手角色
TAKEOUT_RECOMMENDATION_ROLE = """
作为外卖推荐助手，你专注于根据用户输入的外卖类型、口味偏好、价格预算和所在地区，推荐健康的本地外卖菜品与本地外卖平台上的外卖商家。
你将利用百度Agently技术，结合用户的需求和所在地区的餐饮特色，提供个性化的外卖建议。
推荐的菜品应包括菜品名称、预计价格，并确保符合用户的口味偏好和健康饮食原则。
为每份推荐菜品提供详细的营养分析，包括热量、蛋白质、脂肪和碳水化合物含量。
如果用户的价格预算较低，优先推荐性价比高且营养均衡的菜品。
注意：推荐的菜品和商家应尽量贴合当地外卖平台的实际供应情况，保持建议的实用性和可操作性。
回答必须使用中文，信息科学准确，表述亲和易懂，避免歧义和误导。
"""

# 经济饮食助手角色
ECONOMICAL_ROLE = """
作为经济饮食助手，你专注于在保证饮食相对健康的前提下，为用户提供最经济实惠的饮食建议。
你将利用百度Agently技术，结合用户的预算、饮食偏好、所在地区和健康目标，推荐低成本但营养均衡的饮食方案。
根据用户的预算限制，优先选择价格低廉、易获取的食材，并确保推荐的饮食方案符合健康饮食原则。
你会提供详细的食材选择建议，包括具体份量（如"200克土豆"或"100克鸡胸肉"）和采购建议（如选择当季蔬菜或批发市场购买）。
针对用户的健康目标（如减重、增肌或维持健康），提供性价比高的食谱规划和营养搭配建议，确保营养均衡。
当用户的饮食偏好与预算或健康目标冲突时，以健康目标和预算优先，调整推荐方案并解释原因。
你还会提供省钱小贴士，如如何利用剩余食材、批量购买或自制替代高价加工食品。
注意：推荐的食材和菜谱应贴合用户所在地的市场价格和供应情况，确保实用性和可操作性。
回答必须使用中文，信息科学准确，表述亲和易懂，避免歧义和误导。
"""
WEIGHT_DIET_ROLE = """
作为智能减脂营养师，你专注于通过科学数据分析和个性化方案设计，帮助用户达成健康减重目标。
你将利用百度Agently技术，结合用户当前体重、目标体重、身体指标(体脂率/BMI)、运动习惯和饮食偏好，制定精准的饮食计划。
根据用户的身体数据，通过Harris-Benedict公式计算基础代谢率(BMR)，并考虑活动系数(PAL)得出每日总消耗量(TDEE)。
按照每周减重0.5-1kg的健康标准，设计每日300-500大卡的热量缺口，并确保三大营养素配比合理(蛋白质25-35%/脂肪20-30%/碳水40-50%)。
需提供详细的餐单规划，包含：
1. 精确到克数的食材份量(如"150克鸡胸肉")
2. 推荐烹饪方式(少油煎/蒸煮等)
3. 每餐营养数据(热量/蛋白质/脂肪/碳水)
4. 加餐建议(低GI水果/坚果等)
根据用户运动习惯提供饮食调整方案：
- 力量训练日增加碳水比例
- 有氧日补充电解质
- 休息日控制脂肪摄入
当检测到用户数据异常时(如BMI<18.5仍要求减重)，应主动警示健康风险并提供专业建议。
需定期提供进度跟踪方案，包括：
1. 建议称重频率(如每周一次晨起空腹)
2. 围度测量指导(腰/臀/腿)
3. 根据实际减重速度动态调整饮食计划
回答必须使用中文，推荐依据需标注参考文献(如《中国居民膳食指南》)，禁用模糊表述，统一数值类型（所有营养数据改为数字），规范字符串引号（使用英文双引号）。
"""


def create_diet_agent(role):
    """创建指定角色的代理"""
    agent_factory = init_agent_factory()
    return agent_factory.create_agent().set_role(role)

def query_diet_definition(agent):
    """查询健康饮食定义"""
    result = (
        agent
        .general("输出规定", "必须使用中文进行输出")
        .role({"姓名": "Agently健康饮食小助手", "任务": "使用自己的知识为用户解答常见问题"})
        .user_info("和你对话的用户是一个希望改善自己饮食的人")
        .input({
            "question": "请问健康饮食的定义是什么？",
            "reply_style_expect": "请用对健康饮食一点都不了解的人能理解的方式进行回复"
        })
        .instruct(["请使用{reply_style_expect}的回复风格，回复{question}提出的问题"])
        .output({
            "reply": ("str", "对{question}的直接回复"),
            "next_questions": ([("str", "根据{reply}内容，结合{user_info}提供的用户信息，给用户推荐的可以进一步提问的问题")], "不少于3个")
        })
        .start()
    )
    return result

def generate_diet_recommendation(agent, health_goal=None, dietary_preferences=None, 
                               lifestyle=None, daily_calories=2000, daily_nutrition=None,
                               bmi=None, gender=None, age=None):
    """
    生成个性化的饮食推荐
    """
    daily_nutrition = daily_nutrition or {}
    total_calories = daily_calories or 2000
    
    # 计算每餐分配的热量
    breakfast_calories = total_calories * 0.3  # 早餐30%
    lunch_calories = total_calories * 0.4      # 午餐40%
    dinner_calories = total_calories * 0.3     # 晚餐30%
    
    try:
        result = (
            agent
            .input({
                "user_info": {
                    "gender": gender or "未指定",
                    "age": age or "未指定",
                    "bmi": bmi or "未指定",
                    "health_goal": health_goal or "未指定",
                    "dietary_preferences": dietary_preferences or "未指定",
                    "lifestyle": lifestyle or "未指定"
                },
                "calories": {
                    "total": total_calories,
                    "breakfast": breakfast_calories,
                    "lunch": lunch_calories,
                    "dinner": dinner_calories
                }
            })
            .instruct(
                "请根据用户信息和卡路里需求生成一日三餐推荐。"
                "每餐必须包含：具体推荐的食物及份量清单、食物的详细营养价值分析、具体的食用建议。"
                "确保推荐的食物符合用户的健康目标和饮食偏好，满足每餐的热量需求，"
                "营养均衡且含有适量的蛋白质、碳水化合物和脂肪，并提供具体的食物份量。"
            )
            .output({
                "breakfast": {
                    "foods": ("list", "早餐推荐的具体食物及份量清单"),
                    "nutrition": ("str", "早餐食物的营养价值分析"),
                    "advice": ("str", "早餐的具体食用建议")
                },
                "lunch": {
                    "foods": ("list", "午餐推荐的具体食物及份量清单"),
                    "nutrition": ("str", "午餐食物的营养价值分析"),
                    "advice": ("str", "午餐的具体食用建议")
                },
                "dinner": {
                    "foods": ("list", "晚餐推荐的具体食物及份量清单"),
                    "nutrition": ("str", "晚餐食物的营养价值分析"),
                    "advice": ("str", "晚餐的具体食用建议")
                },
                "nutrition_advice": ("str", "全天营养搭配建议")
            })
            .start()
        )
        
        # 格式化返回结果
        formatted_result = {
            '早餐推荐': {
                '推荐食物': result.get('breakfast', {}).get('foods', []),
                '营养价值': result.get('breakfast', {}).get('nutrition', ''),
                '食用建议': result.get('breakfast', {}).get('advice', '')
            },
            '午餐推荐': {
                '推荐食物': result.get('lunch', {}).get('foods', []),
                '营养价值': result.get('lunch', {}).get('nutrition', ''),
                '食用建议': result.get('lunch', {}).get('advice', '')
            },
            '晚餐推荐': {
                '推荐食物': result.get('dinner', {}).get('foods', []),
                '营养价值': result.get('dinner', {}).get('nutrition', ''),
                '食用建议': result.get('dinner', {}).get('advice', '')
            }
        }

        if 'nutrition_advice' in result:
            formatted_result['营养建议'] = result['nutrition_advice']
        
        return formatted_result
        
    except Exception as e:
        print(f"生成推荐时出错: {e}")
        return {
            '早餐推荐': {'推荐食物': ['生成失败'], '营养价值': '暂无数据', '食用建议': '请稍后重试'},
            '午餐推荐': {'推荐食物': ['生成失败'], '营养价值': '暂无数据', '食用建议': '请稍后重试'},
            '晚餐推荐': {'推荐食物': ['生成失败'], '营养价值': '暂无数据', '食用建议': '请稍后重试'}
        }

def analyze_meals(agent, meals):
    """分析一日三餐的营养情况"""
    try:
        print(f"开始分析meals: {meals}")
        
        # 构建详细的分析指令
        meal_descriptions = []
        if meals.get('breakfast'):
            meal_descriptions.append(f"早餐：{meals['breakfast']}")
        if meals.get('lunch'):
            meal_descriptions.append(f"午餐：{meals['lunch']}")
        if meals.get('dinner'):
            meal_descriptions.append(f"晚餐：{meals['dinner']}")
        
        meal_text = "\n".join(meal_descriptions)
        
        # 详细的营养分析指令
        detailed_instruction = f"""
作为专业的营养分析师，请仔细分析用户的一日三餐营养情况：

{meal_text}

请提供以下详细分析：

1. 分别分析每餐的营养构成、优点和不足
2. 基于中国居民膳食指南，计算具体的营养摄入量
3. 提供综合营养建议和改进方案
4. 给出1-10分的整体评分

营养计算要求：
- 总热量：参考食物热量表，计算总千卡数
- 蛋白质：计算总克数，成年人建议每日50-70克
- 脂肪：计算总克数，占总热量20-30%
- 碳水化合物：计算总克数，占总热量50-65%

请确保数据准确、分析深入、建议实用。
"""
        
        # 准备AI分析
        analysis_agent = (
            agent
            .input({"meals": meals, "instruction": detailed_instruction})
            .instruct(detailed_instruction)
            .output({
                "早餐分析": ("str", "对早餐营养的详细分析，包括热量、营养素和改进建议"),
                "午餐分析": ("str", "对午餐营养的详细分析，包括热量、营养素和改进建议"),
                "晚餐分析": ("str", "对晚餐营养的详细分析，包括热量、营养素和改进建议"),
                "营养建议": ("str", "基于整体分析的综合营养改进建议"),
                "总热量": ("str", "一日三餐总热量，必须包含具体数字，格式如：1850千卡"),
                "蛋白质": ("str", "总蛋白质摄入量，必须包含具体数字，格式如：65.5克"),
                "脂肪": ("str", "总脂肪摄入量，必须包含具体数字，格式如：58.2克"),
                "碳水化合物": ("str", "总碳水化合物摄入量，必须包含具体数字，格式如：220.8克"),
                "整体评分": ("float", "整体营养评分，1-10分的浮点数")
            })
        )
        
        print("正在调用AI模型进行分析...")
        result = analysis_agent.start()
        
        print(f"AI分析结果: {result}")
        
        if not result:
            # 如果AI分析失败，提供合理的模拟数据
            print("AI分析失败，使用备用分析逻辑")
            return generate_fallback_analysis(meals)
            
        # 验证并修复返回结果
        result = validate_and_fix_analysis_result(result, meals)
                
        return result
        
    except Exception as e:
        print(f"分析meals时出错: {str(e)}")
        import traceback
        traceback.print_exc()
        # 返回备用分析结果
        return generate_fallback_analysis(meals)

def generate_fallback_analysis(meals):
    """生成备用的营养分析结果"""
    print("使用备用分析逻辑...")
    
    # 基础营养估算
    total_calories = 0
    total_protein = 0
    total_fat = 0
    total_carbs = 0
    
    # 简单的营养估算逻辑
    meal_count = sum(1 for meal in meals.values() if meal and meal.strip())
    
    if meal_count == 0:
        return {
            "早餐分析": "未提供早餐信息",
            "午餐分析": "未提供午餐信息", 
            "晚餐分析": "未提供晚餐信息",
            "营养建议": "请提供完整的三餐信息以便进行营养分析",
            "总热量": "0千卡",
            "蛋白质": "0克",
            "脂肪": "0克",
            "碳水化合物": "0克",
            "整体评分": 0.0
        }
    
    # 基于餐食内容的估算
    for meal_name, meal_content in meals.items():
        if not meal_content or not meal_content.strip():
            continue
            
        # 简单的关键词营养估算
        content_lower = meal_content.lower()
        meal_calories = 0
        meal_protein = 0
        meal_fat = 0
        meal_carbs = 0
        
        # 主食类
        if any(word in content_lower for word in ['米饭', '面条', '面包', '馒头', '粥']):
            meal_calories += 200
            meal_carbs += 45
            meal_protein += 4
        
        # 蛋白质类
        if any(word in content_lower for word in ['鸡蛋', '肉', '鱼', '虾', '豆腐']):
            meal_calories += 150
            meal_protein += 15
            meal_fat += 8
        
        # 蔬菜类
        if any(word in content_lower for word in ['菜', '蔬菜', '西红柿', '黄瓜', '菠菜']):
            meal_calories += 30
            meal_carbs += 6
            meal_protein += 2
        
        # 奶制品
        if any(word in content_lower for word in ['牛奶', '酸奶', '奶']):
            meal_calories += 120
            meal_protein += 8
            meal_fat += 6
            meal_carbs += 12
        
        # 坚果类
        if any(word in content_lower for word in ['坚果', '核桃', '杏仁']):
            meal_calories += 100
            meal_fat += 9
            meal_protein += 3
        
        total_calories += meal_calories
        total_protein += meal_protein
        total_fat += meal_fat
        total_carbs += meal_carbs
    
    # 生成分析文本
    def generate_meal_analysis(meal_name, meal_content):
        if not meal_content or not meal_content.strip():
            return f"未提供{meal_name}信息，建议补充营养均衡的{meal_name}。"
        
        analysis = f"您的{meal_name}包含：{meal_content}。"
        
        # 根据内容给出简单评价
        content_lower = meal_content.lower()
        if any(word in content_lower for word in ['米饭', '面条', '粥']):
            analysis += "主食搭配较好，能提供基础能量。"
        if any(word in content_lower for word in ['鸡蛋', '肉', '鱼']):
            analysis += "蛋白质来源充足。"
        if any(word in content_lower for word in ['菜', '蔬菜']):
            analysis += "蔬菜摄入有助于维生素补充。"
        else:
            analysis += "建议增加蔬菜摄入。"
        
        return analysis
    
    # 计算评分
    score = 5.0  # 基础分
    if total_calories > 1200:
        score += 1
    if total_protein > 40:
        score += 1
    if meal_count >= 3:
        score += 1
    if any('蔬菜' in str(meal) or '菜' in str(meal) for meal in meals.values()):
        score += 1
    
    score = min(score, 10.0)
    
    return {
        "早餐分析": generate_meal_analysis("早餐", meals.get('breakfast', '')),
        "午餐分析": generate_meal_analysis("午餐", meals.get('lunch', '')),
        "晚餐分析": generate_meal_analysis("晚餐", meals.get('dinner', '')),
        "营养建议": f"根据您的饮食情况，建议：1. 保证三餐规律；2. 增加蔬菜水果摄入；3. 适量补充优质蛋白质；4. 控制油脂摄入。当前营养搭配{'较为均衡' if score >= 6 else '需要改善'}。",
        "总热量": f"{int(total_calories)}千卡",
        "蛋白质": f"{round(total_protein, 1)}克",
        "脂肪": f"{round(total_fat, 1)}克", 
        "碳水化合物": f"{round(total_carbs, 1)}克",
        "整体评分": round(score, 1)
    }

def validate_and_fix_analysis_result(result, meals):
    """验证并修复AI分析结果"""
    if not result:
        return generate_fallback_analysis(meals)
    
    # 确保所有必需字段都存在
    required_fields = ["早餐分析", "午餐分析", "晚餐分析", "营养建议", "总热量", "蛋白质", "脂肪", "碳水化合物", "整体评分"]
    
    for field in required_fields:
        if field not in result or not result[field]:
            print(f"修复缺失字段: {field}")
            if field in ["早餐分析", "午餐分析", "晚餐分析"]:
                result[field] = f"暂无{field}数据，建议合理搭配营养。"
            elif field == "营养建议":
                result[field] = "建议保持饮食均衡，适量运动，充足睡眠。"
            elif field in ["总热量", "蛋白质", "脂肪", "碳水化合物"]:
                # 如果营养数据缺失，使用备用估算
                fallback = generate_fallback_analysis(meals)
                result[field] = fallback[field]
            elif field == "整体评分":
                result[field] = 5.0
    
    # 验证营养数据格式
    nutrition_fields = ["总热量", "蛋白质", "脂肪", "碳水化合物"]
    for field in nutrition_fields:
        value = str(result[field])
        if not any(char.isdigit() for char in value):
            print(f"修复营养数据格式: {field}")
            fallback = generate_fallback_analysis(meals)
            result[field] = fallback[field]
    
    # 验证评分范围
    try:
        score = float(result["整体评分"])
        if score < 0 or score > 10:
            result["整体评分"] = 5.0
    except (ValueError, TypeError):
        result["整体评分"] = 5.0
    
    return result

def provide_diet_advice(agent, nutritional_analysis, health_goal, dietary_preferences):
    """提供饮食建议"""
    result = (
        agent
        .input({
            "nutritional_analysis": nutritional_analysis,
            "health_goal": health_goal,
            "dietary_preferences": dietary_preferences
        })
        .instruct(
            "基于用户的营养分析和健康目标，提供改进建议。"
            "建议应包括直观可量化的营养指标（如每日需摄入多少量蛋白质），以及具体的食物推荐（如'每天一杯酸奶'或'增加一小碗约200克蔬菜'）。"
            "保持建议内容自然、亲和力强，不显得过于AI化。"
        )
        .output({"advice": ("str",)})
        .start()
    )
    return result

def generate_recipe(agent, recipe_name, location):
    """生成菜谱"""
    try:
        result = (
            agent
            .input({
                "recipe_name": recipe_name,
                "location": location
            })
            .instruct(
                f"请为用户生成{recipe_name}的详细菜谱，考虑{location}地区的特色和食材availability。"
                "请提供详细的食材清单、制作步骤和营养分析。"
            )
            .output({
                "食材清单": ("str", "详细的食材清单和用量"),
                "制作步骤": ("str", "详细的制作步骤"),
                "营养分析": ("str", "菜谱的营养成分分析"),
                "制作时间": ("str", "预计制作时间"),
                "难度等级": ("str", "制作难度评级")
            })
            .start()
        )
        return result
    except Exception as e:
        print(f"生成菜谱时出错: {str(e)}")
        return {
            "食材清单": f"生成{recipe_name}菜谱失败，请重试",
            "制作步骤": "暂时无法提供制作步骤",
            "营养分析": "暂时无法提供营养分析",
            "制作时间": "未知",
            "难度等级": "未知"
        }

def generate_takeout_recommendation(agent, takeout_type, taste_preference, budget, location):
    """生成外卖推荐"""
    try:
        result = (
            agent
            .input({
                "takeout_type": takeout_type,
                "taste_preference": taste_preference,
                "budget": budget,
                "location": location
            })
            .instruct(
                f"基于用户在{location}的需求，推荐符合{taste_preference}口味、预算{budget}的{takeout_type}外卖。"
                "请提供具体的菜品推荐、价格估算和营养分析。"
            )
            .output({
                "推荐菜品": ("str", "推荐的外卖菜品"),
                "价格预估": ("str", "价格范围估算"),
                "营养分析": ("str", "推荐菜品的营养分析"),
                "商家建议": ("str", "推荐的外卖商家类型"),
                "健康建议": ("str", "外卖的健康搭配建议")
            })
            .start()
        )
        return result
    except Exception as e:
        print(f"生成外卖推荐时出错: {str(e)}")
        return {
            "推荐菜品": "暂时无法生成外卖推荐，请重试",
            "价格预估": "未知",
            "营养分析": "暂时无法提供营养分析",
            "商家建议": "请选择信誉良好的商家",
            "健康建议": "注意营养搭配，适量食用"
        }

def generate_economical_diet(agent, budget, health_goal, dietary_preferences, location):
    """生成经济实惠的饮食方案"""
    try:
        result = (
            agent
            .input({
                "budget": budget,
                "health_goal": health_goal,
                "dietary_preferences": dietary_preferences,
                "location": location
            })
            .instruct(
                f"为用户在{location}地区设计预算为{budget}的经济健康饮食方案。"
                "考虑健康目标：{health_goal}，饮食偏好：{dietary_preferences}。"
                "请提供具体的食物搭配、购买建议和成本控制方案。"
            )
            .output({
                "饮食方案": ("str", "经济实惠的一日三餐方案"),
                "食材清单": ("str", "推荐的经济实惠食材"),
                "预算分析": ("str", "详细的成本分析"),
                "营养评估": ("str", "方案的营养价值评估"),
                "购买建议": ("str", "食材采购和保存建议")
            })
            .start()
        )
        return result
    except Exception as e:
        print(f"生成经济饮食方案时出错: {str(e)}")
        return {
            "饮食方案": "暂时无法生成经济饮食方案，请重试",
            "食材清单": "建议选择当季蔬菜、基础谷物和经济蛋白质来源",
            "预算分析": "请根据当地市场价格调整预算",
            "营养评估": "注意营养均衡，不要因节省而影响健康",
            "购买建议": "选择大包装、当季食材，合理存储"
        }

def generate_weight_loss_plan(agent, current_weight, target_weight, time_frame, dietary_preferences):
    """生成减重计划"""
    try:
        result = (
            agent
            .input({
                "current_weight": current_weight,
                "target_weight": target_weight,
                "time_frame": time_frame,
                "dietary_preferences": dietary_preferences
            })
            .instruct(
                f"为用户制定从{current_weight}kg减到{target_weight}kg的{time_frame}减重计划。"
                "考虑饮食偏好：{dietary_preferences}。"
                "请提供科学、健康、可执行的减重方案。"
            )
            .output({
                "减重目标": ("str", "详细的减重目标分析"),
                "饮食计划": ("str", "减重期间的饮食安排"),
                "运动建议": ("str", "配合的运动方案"),
                "时间安排": ("str", "分阶段的时间计划"),
                "注意事项": ("str", "减重过程中的注意事项")
            })
            .start()
        )
        return result
    except Exception as e:
        print(f"生成减重计划时出错: {str(e)}")
        return {
            "减重目标": "暂时无法生成减重计划，请重试",
            "饮食计划": "建议咨询专业营养师制定个性化方案",
            "运动建议": "适量有氧运动，循序渐进",
            "时间安排": "健康减重速度为每周0.5-1公斤",
            "注意事项": "减重过程中注意营养均衡，避免极端节食"
        }