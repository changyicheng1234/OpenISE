import Agently
from diet_agent import (
    create_diet_agent,
    generate_diet_recommendation,
    analyze_meals,
    generate_recipe,
    generate_takeout_recommendation,
    generate_economical_diet,  
    SMART_DIET_ROLE,
    DIET_ANALYZE_ROLE,
    DIET_RECIPE_GENERATOR_ROLE,
    TAKEOUT_RECOMMENDATION_ROLE,
    ECONOMICAL_ROLE ,
    generate_weight_loss_plan,
    WEIGHT_DIET_ROLE
)
def setup_diet_recommendation_workflow():
    """设置健康食谱推荐工作流"""
    workflow = Agently.Workflow()
    agent = create_diet_agent(SMART_DIET_ROLE)

    @workflow.chunk()
    def input_user_info(inputs, storage):
        health_goal = input(
            "请输入您的健康目标（如减重、增肌、控制血糖等，输入'n'终止流程）："
        ).strip().lower()
        if health_goal == "n":
            print("流程已终止。")
            return "end"
        if not health_goal:
            print("健康目标不能为空，请重新输入。")
            return "input_user_info"

        dietary_preferences = input(
            "请输入您的饮食偏好（如低脂、高蛋白、素食等，输入'n'终止流程）："
        ).strip().lower()
        if dietary_preferences == "n":
            print("流程已终止。")
            return "end"
        if not dietary_preferences:
            print("饮食偏好不能为空，请重新输入。")
            return "input_user_info"

        lifestyle = input(
            "请输入您的生活习惯（包括不良习惯如久坐、熬夜、重口味；或健康问题如高血压、高血脂等，输入'n'终止流程）："
        ).strip().lower()
        if lifestyle == "n":
            print("流程已终止。")
            return "end"
        if not lifestyle:
            print("生活习惯不能为空，请重新输入。")
            return "input_user_info"

        storage.set("health_goal", health_goal)
        storage.set("dietary_preferences", dietary_preferences)
        storage.set("lifestyle", lifestyle)
        return None

    @workflow.chunk()
    def generate_recommendations(inputs, storage):
        health_goal = storage.get("health_goal")
        dietary_preferences = storage.get("dietary_preferences")
        lifestyle = storage.get("lifestyle")
        recommendation = generate_diet_recommendation(
            agent, health_goal, dietary_preferences, lifestyle
        )
        storage.set("diet_recommendation", recommendation)
        return recommendation

    @workflow.chunk()
    def print_recommendations(inputs, storage):
        recommendation = storage.get("diet_recommendation", {})
        print(f"推荐的健康食谱：{recommendation.get('推荐食谱', '暂无推荐')}")
        recommendation_reason = recommendation.get("推荐理由", "")
        if recommendation_reason:
            print(f"推荐理由：{recommendation_reason}")
        return None

    @workflow.chunk()
    def ask_for_more(inputs, storage):
        confirm = input(
            "您还有其他健康饮食问题或需要新的食谱推荐吗？(y/n): "
        ).strip().lower()
        if confirm == "n":
            print("感谢您的咨询！")
            return "end"
        elif confirm != "y":
            print("输入无效，请输入'y'继续或'n'终止流程。")
            return "ask_for_more"
        return confirm == "y"

    (
        workflow
        .connect_to("input_user_info")
        .connect_to("generate_recommendations")
        .connect_to("print_recommendations")
        .connect_to("ask_for_more")
        .if_condition(lambda confirmed, _: confirmed)
        .connect_to("input_user_info")
    )
    return workflow

def setup_diet_analyze_workflow():
    """设置健康食谱分析工作流"""
    workflow = Agently.Workflow()
    agent = create_diet_agent(DIET_ANALYZE_ROLE)

    @workflow.chunk()
    def input_user_health_goal_and_preferences(inputs, storage):
        health_goal = input(
            "请输入您的健康目标（如减重、增肌、控制血糖等，输入'n'终止流程）："
        ).strip().lower()
        if health_goal == "n":
            print("流程已终止。")
            return "end"
        if not health_goal:
            print("健康目标不能为空，请重新输入。")
            return "input_user_health_goal_and_preferences"

        dietary_preferences = input(
            "请输入您的生活习惯（如饮食方面：低脂、高蛋白、素食；作息方面：乏力、失眠、久坐等，输入'n'终止流程）："
        ).strip().lower()
        if dietary_preferences == "n":
            print("流程已终止。")
            return "end"
        if not dietary_preferences:
            print("生活习惯不能为空，请重新输入。")
            return "input_user_health_goal_and_preferences"

        storage.set("health_goal", health_goal)
        storage.set("dietary_preferences", dietary_preferences)
        return "input_user_meals"

    @workflow.chunk()
    def input_user_meals(inputs, storage):
        breakfast = input(
            "请输入您具体的早餐内容（请尽量量化，如‘200克燕麦’）："
        ).strip()
        lunch = input(
            "请输入您具体的午餐内容（请尽量量化，如‘150克鸡胸肉’）："
        ).strip()
        dinner = input(
            "请输入您具体的晚餐内容（请尽量量化，如‘一碗米饭’）："
        ).strip()
        meals = {"breakfast": breakfast, "lunch": lunch, "dinner": dinner}
        storage.set("meals", meals)
        return "analyze_meals"

    @workflow.chunk()
    def analyze_meals_chunk(inputs, storage):
        meals = storage.get("meals")
        analysis_result = analyze_meals(agent, meals)
        storage.set("nutritional_analysis", analysis_result["nutritional_analysis"])
        return "provide_diet_advice"

    @workflow.chunk()
    def provide_diet_advice_chunk(inputs, storage):
        nutritional_analysis = storage.get("nutritional_analysis")
        health_goal = storage.get("health_goal")
        dietary_preferences = storage.get("dietary_preferences")
        advice = provide_diet_advice(agent, nutritional_analysis, health_goal, dietary_preferences)
        print(advice["advice"])
        return "ask_for_more_recommendations"

    @workflow.chunk()
    def ask_for_more_recommendations(inputs, storage):
        confirm = input("您是否需要更多饮食建议？(y/n): ").strip().lower()
        if confirm == "n":
            print("感谢您的咨询！")
            return "end"
        elif confirm != "y":
            print("输入无效，请输入'y'继续或'n'终止流程。")
            return "ask_for_more_recommendations"
        return confirm == "y"

    (
        workflow
        .connect_to("input_user_health_goal_and_preferences")
        .connect_to("input_user_meals")
        .connect_to("analyze_meals_chunk")
        .connect_to("provide_diet_advice_chunk")
        .connect_to("ask_for_more_recommendations")
        .if_condition(lambda confirmed, _: confirmed)
        .connect_to("input_user_health_goal_and_preferences")
    )
    return workflow

def setup_recipe_workflow():
    """设置菜谱生成工作流"""
    workflow = Agently.Workflow()
    agent = create_diet_agent(DIET_RECIPE_GENERATOR_ROLE)

    @workflow.chunk()
    def input_recipe_name(inputs, storage):
        recipe_name = input(
            "请输入您想要的食谱名称（如：赛螃蟹，输入'n'终止流程）："
        ).strip().lower()
        if recipe_name == "n":
            print("流程已终止。")
            return "end"
        if not recipe_name:
            print("食谱名称不能为空，请重新输入。")
            return "input_recipe_name"
        storage.set("recipe_name", recipe_name)
        return "input_user_location"

    @workflow.chunk()
    def input_user_location(inputs, storage):
        location = input(
            "请输入您的所在地（如：上海，输入'n'终止流程）："
        ).strip().lower()
        if location == "n":
            print("流程已终止。")
            return "end"
        if not location:
            print("所在地不能为空，请重新输入。")
            return "input_user_location"
        storage.set("location", location)
        return "generate_food_list"

    @workflow.chunk()
    def generate_food_list(inputs, storage):
        recipe_name = storage.get("recipe_name")
        location = storage.get("location")
        result = generate_recipe(agent, recipe_name, location)
        if result is None:
            print("抱歉，无法获取食谱信息，请稍后重试或更换其他食谱名称。")
            return "ask_for_more_recipes"

        try:
            ingredients_list = result.get("ingredients_list", [])
            cooking_process = result.get("cooking_process", "未提供制作流程")
            nutritional_analysis = result.get("nutritional_analysis", {})

            if ingredients_list:
                print("\n食材列表：")
                for item in ingredients_list:
                    if isinstance(item, dict):
                        name = item.get("name", "未知")
                        quantity = item.get("quantity", "未知")
                        print(f"- {name}（{quantity}）")
                    else:
                        print(f"- {item}")
            print("\n制作流程：")
            print(cooking_process)
            print("\n营养分析：")
            if isinstance(nutritional_analysis, dict):
                for nutrient, value in nutritional_analysis.items():
                    print(f"{nutrient}: {value}")
            else:
                print("营养分析信息不是字典类型，无法处理。")
        except KeyError as e:
            print(f"数据获取失败，缺少预期字段：{e}")
        return "ask_for_more_recipes"

    @workflow.chunk()
    def ask_for_more_recipes(inputs, storage):
        confirm = input("您是否需要更多食谱？(y/n): ").strip().lower()
        if confirm == "n":
            print("感谢您的使用！")
            return "end"
        elif confirm != "y":
            print("输入无效，请输入'y'继续或'n'终止流程。")
            return "ask_for_more_recipes"
        return confirm == "y"

    (
        workflow
        .connect_to("input_recipe_name")
        .connect_to("input_user_location")
        .connect_to("generate_food_list")
        .connect_to("ask_for_more_recipes")
        .if_condition(lambda confirmed, _: confirmed)
        .connect_to("input_recipe_name")
    )
    return workflow

def setup_takeout_recommendation_workflow():
    """设置外卖推荐工作流"""
    workflow = Agently.Workflow()
    agent = create_diet_agent(TAKEOUT_RECOMMENDATION_ROLE)

    @workflow.chunk()
    def input_takeout_type(inputs, storage):
        takeout_type = input(
            "请输入您想吃的外卖类型与口味（如：中式快餐，偏辣，输入'n'终止流程）："
        ).strip().lower()
        if takeout_type == "n":
            print("流程已终止。")
            return "end"
        if not takeout_type:
            print("外卖类型与口味不能为空，请重新输入。")
            return "input_takeout_type"
        storage.set("takeout_type", takeout_type)
        return "input_price"

    @workflow.chunk()
    def input_price(inputs, storage):
        price = input(
            "请输入您的外卖价格预算（如：30元，输入'n'终止流程）："
        ).strip().lower()
        if price == "n":
            print("流程已终止。")
            return "end"
        if not price:
            print("价格预算不能为空，请重新输入。")
            return "input_price"
        storage.set("price", price)
        return "input_user_location"

    @workflow.chunk()
    def input_user_location(inputs, storage):
        location = input(
            "请输入您的所在地（如：上海，输入'n'终止流程）："
        ).strip().lower()
        if location == "n":
            print("流程已终止。")
            return "end"
        if not location:
            print("所在地不能为空，请重新输入。")
            return "input_user_location"
        storage.set("location", location)
        return "generate_takeout_list"

    @workflow.chunk()
    def generate_takeout_list(inputs, storage):
        takeout_type = storage.get("takeout_type")
        price = storage.get("price")
        location = storage.get("location")
        result = generate_takeout_recommendation(agent, takeout_type, price, location)
        if result is None:
            print("抱歉，无法获取外卖推荐信息，请稍后重试或更换其他输入。")
            return "ask_for_more_takeouts"

        try:
            dishes_list = result.get("dishes_list", [])
            nutritional_analysis = result.get("nutritional_analysis", {})

            if dishes_list:
                print("\n推荐外卖菜品：")
                for item in dishes_list:
                    if isinstance(item, dict):
                        name = item.get("name", "未知")
                        price = item.get("price", "未知")
                        print(f"- {name}（{price}）")
                    else:
                        print(f"- {item}")
            print("\n营养分析：")
            if isinstance(nutritional_analysis, dict):
                for nutrient, value in nutritional_analysis.items():
                    print(f"{nutrient}: {value}")
            else:
                print("营养分析信息不是字典类型，无法处理。")
        except KeyError as e:
            print(f"数据获取失败，缺少预期字段：{e}")
        return "ask_for_more_takeouts"

    @workflow.chunk()
    def ask_for_more_takeouts(inputs, storage):
        confirm = input("您是否需要更多外卖推荐？(y/n): ").strip().lower()
        if confirm == "n":
            print("感谢您的使用！")
            return "end"
        elif confirm != "y":
            print("输入无效，请输入'y'继续或'n'终止流程。")
            return "ask_for_more_takeouts"
        return confirm == "y"

    (
        workflow
        .connect_to("input_takeout_type")
        .connect_to("input_price")
        .connect_to("input_user_location")
        .connect_to("generate_takeout_list")
        .connect_to("ask_for_more_takeouts")
        .if_condition(lambda confirmed, _: confirmed)
        .connect_to("input_takeout_type")
    )
    return workflow

def setup_economical_diet_workflow():
    """设置经济饮食推荐工作流"""
    workflow = Agently.Workflow()
    agent = create_diet_agent(ECONOMICAL_ROLE)

    @workflow.chunk()
    def input_budget_and_location(inputs, storage):
        budget = input(
            "请输入您的每日饮食预算（如：30元，输入'n'终止流程）："
        ).strip().lower()
        if budget == "n":
            print("流程已终止。")
            return "end"
        if not budget:
            print("预算不能为空，请重新输入。")
            return "input_budget_and_location"
        
        location = input(
            "请输入您的所在地（如：上海，输入'n'终止流程）："
        ).strip().lower()
        if location == "n":
            print("流程已终止。")
            return "end"
        if not location:
            print("所在地不能为空，请重新输入。")
            return "input_budget_and_location"

        storage.set("budget", budget)
        storage.set("location", location)
        return "input_user_info"

    @workflow.chunk()
    def input_user_info(inputs, storage):
        health_goal = input(
            "请输入您的健康目标（如减重、增肌、控制血糖等，输入'n'终止流程）："
        ).strip().lower()
        if health_goal == "n":
            print("流程已终止。")
            return "end"
        if not health_goal:
            print("健康目标不能为空，请重新输入。")
            return "input_user_info"

        dietary_preferences = input(
            "请输入您的饮食偏好（如低脂、高蛋白、素食等，输入'n'终止流程）："
        ).strip().lower()
        if dietary_preferences == "n":
            print("流程已终止。")
            return "end"
        if not dietary_preferences:
            print("饮食偏好不能为空，请重新输入。")
            return "input_user_info"

        storage.set("health_goal", health_goal)
        storage.set("dietary_preferences", dietary_preferences)
        return "generate_economical_diet"

    @workflow.chunk()
    def generate_economical_diet(inputs, storage):
        budget = storage.get("budget")
        location = storage.get("location")
        health_goal = storage.get("health_goal")
        dietary_preferences = storage.get("dietary_preferences")
        recommendation = generate_economical_diet(agent, budget, location, health_goal, dietary_preferences)
        storage.set("economical_diet_recommendation", recommendation)
        return recommendation

    @workflow.chunk()
    def print_economical_diet(inputs, storage):
        recommendation = storage.get("economical_diet_recommendation", {})
        print("\n经济型饮食推荐：")
        for meal in ["早餐推荐", "午餐推荐", "晚餐推荐"]:
            meal_data = recommendation.get(meal, {})
            print(f"\n{meal}：")
            print("推荐食材：")
            for item in meal_data.get("推荐食材", []):
                name = item.get("name", "未知")
                quantity = item.get("quantity", "未知")
                print(f"- {name}（{quantity}）")
            print(f"预计费用：{meal_data.get('预计费用', '未知')}元")
            print(f"营养价值：{meal_data.get('营养价值', '暂无数据')}")
        print("\n省钱建议：")
        for tip in recommendation.get("省钱建议", []):
            print(f"- {tip}")
        print(f"\n总费用：{recommendation.get('总费用', '未知')}元")
        return "ask_for_more_economical_diets"

    @workflow.chunk()
    def ask_for_more_economical_diets(inputs, storage):
        confirm = input("您是否需要更多经济型饮食推荐？(y/n): ").strip().lower()
        if confirm == "n":
            print("感谢您的使用！")
            return "end"
        elif confirm != "y":
            print("输入无效，请输入'y'继续或'n'终止流程。")
            return "ask_for_more_economical_diets"
        return confirm == "y"

    (
        workflow
        .connect_to("input_budget_and_location")
        .connect_to("input_user_info")
        .connect_to("generate_economical_diet")
        .connect_to("print_economical_diet")
        .connect_to("ask_for_more_economical_diets")
        .if_condition(lambda confirmed, _: confirmed)
        .connect_to("input_budget_and_location")
    )
    return workflow


# workflow.py
from Agently import Workflow
from diet_agent import (
    create_diet_agent,
    generate_weight_loss_plan,
    SMART_DIET_ROLE
)


def setup_weight_loss_workflow():
    """科学减重计划工作流"""
    workflow = Workflow()
    agent = create_diet_agent(SMART_DIET_ROLE)

    # ----------------- 核心流程定义 -----------------
    @workflow.chunk(name="input_basic_info")
    def input_basic_info(inputs, storage):
        """收集基础信息"""
        try:
            # 当前体重
            current_weight = float(input("\n请输入当前体重（kg）：").strip())
            if not (30 <= current_weight <= 300):
                raise ValueError("体重应在30-300kg之间")

            # 目标体重
            target_weight = float(input("请输入目标体重（kg）：").strip())
            if target_weight >= current_weight:
                raise ValueError("目标体重必须小于当前体重")

            # 计划天数
            days = int(input("请输入计划周期（7/14/30天）：").strip())
            if days not in [7, 14, 30]:
                raise ValueError("仅支持7/14/30天计划")

            storage.set("current_weight", current_weight)
            storage.set("target_weight", target_weight)
            storage.set("days", days)
            return "input_advanced_info"

        except ValueError as e:
            print(f"❗ 输入错误：{str(e)}")
            return "input_basic_info"
        except Exception as e:
            print(f"❗ 发生未知错误：{str(e)}")
            return "error_handling"

    @workflow.chunk(name="input_advanced_info")
    def input_advanced_info(inputs, storage):
        """收集高级信息"""
        try:
            # 身高（可选）
            height = input("请输入身高（cm，可选）：").strip()
            if height:
                height = float(height)
                if not (100 <= height <= 250):
                    raise ValueError("身高应在100-250cm之间")
                storage.set("height", height)

            # 活动强度
            activity_map = {
                "1": "low",
                "2": "moderate",
                "3": "active"
            }
            activity = input(
                "请选择活动强度：\n"
                "1. 久坐（办公室工作，很少运动）\n"
                "2. 中等（每周1-3次运动）\n"
                "3. 活跃（每周4次以上运动）\n"
                "请输入选项："
            ).strip()
            storage.set("activity_level", activity_map.get(activity, "moderate"))
            return "generate_plan"

        except ValueError as e:
            print(f"❗ 输入错误：{str(e)}")
            return "input_advanced_info"
        except Exception as e:
            print(f"❗ 发生未知错误：{str(e)}")
            return "error_handling"

    @workflow.chunk(name="generate_plan")
    def generate_plan(inputs, storage):
        """生成减重计划"""
        try:
            print("\n🔮 正在生成您的专属减重计划...")

            # 获取用户档案
            user_profile = storage.session.get('user_profile', {})

            # 调用核心逻辑
            plan = generate_weight_loss_plan(
                agent,
                current_weight=storage.get("current_weight"),
                target_weight=storage.get("target_weight"),
                days=storage.get("days"),
                height=storage.get("height", 170),
                activity_level=storage.get("activity_level"),
                age=user_profile.get("age", 30),
                gender=user_profile.get("gender", "unknown")
            )

            storage.set("plan", plan)
            return "show_plan"

        except Exception as e:
            print(f"❗ 生成计划失败：{str(e)}")
            storage.set("last_error", str(e))
            return "error_handling"

    @workflow.chunk(name="show_plan")
    def show_plan(inputs, storage):
        """展示计划详情"""
        plan = storage.get("plan")

        # 打印概览
        print("\n" + "=" * 40)
        print(f"🏆 您的 {plan['计划概览']['计划周期']} 减重计划")
        print(f"📍 当前体重: {plan['计划概览']['当前体重']}")
        print(f"🎯 目标体重: {plan['计划概览']['目标体重']}")
        print(f"🔥 每日建议热量: {plan['计划概览']['每日热量']} 大卡")

        print("\n📅 每日计划安排:")
        for day in range(plan['计划概览']['计划天数']):
            print(f"\n第{day + 1}天:")
            for meal_type in ["breakfast", "lunch", "dinner"]:
                meal = plan['每日食谱'][day][meal_type]
                print(f"  {meal_type.capitalize()}:")
                print(f"    - 菜单: {', '.join(meal['菜单'])}")
                print(f"    - 热量: {meal['营养分析']['calories']}kcal")

        # 打印运动建议
        print("\n🏃 运动建议:")
        for exercise in plan['运动建议']:
            print(f"  ✓ {exercise}")

        # 健康提示
        print("\n💡 健康提示:")
        for tip in plan['健康提示']:
            print(f"  ☆ {tip}")

        return "ask_follow_up"

    # ----------------- 后续操作 -----------------
    @workflow.chunk(name="ask_follow_up")
    def ask_follow_up(inputs, storage):
        """后续操作选择"""
        choice = input(
            "\n请选择后续操作：\n"
            "1. 重新生成计划\n"
            "2. 查看详细食材清单\n"
            "3. 导出为PDF\n"
            "4. 结束流程\n"
            "请输入选项："
        ).strip()

        if choice == "1":
            return "input_basic_info"
        elif choice == "2":
            return "show_shopping_list"
        elif choice == "3":
            return "export_pdf"
        elif choice == "4":
            print("\n🎉 祝您减重成功！")
            return "end"
        else:
            print("❗ 无效输入，请重新选择")
            return "ask_follow_up"

    @workflow.chunk(name="show_shopping_list")
    def show_shopping_list(inputs, storage):
        """生成购物清单"""
        print("\n🛒 本周食材清单：")
        # 这里可以添加具体的购物清单生成逻辑
        print("  - 鸡胸肉 1kg")
        print("  - 西兰花 500g")
        print("  - 糙米 2kg")
        return "ask_follow_up"

    # ----------------- 错误处理 -----------------
    @workflow.chunk(name="error_handling")
    def error_handling(inputs, storage):
        choice = input(
            "\n遇到问题，请选择：\n"
            "1. 重试\n"
            "2. 返回主菜单\n"
            "3. 退出\n"
            "请输入选项："
        ).strip()

        if choice == "1":
            return storage.last_step  # 返回上一步
        elif choice == "2":
            return "input_basic_info"
        elif choice == "3":
            return "end"
        else:
            print("❗ 无效输入，自动返回主菜单")
            return "input_basic_info"

    # ----------------- 流程连接 -----------------
    (
        workflow
        .set_start_point("input_basic_info")
        .connect("input_basic_info", {
            "input_advanced_info": "input_advanced_info",
            "error_handling": "error_handling"
        })
        .connect("input_advanced_info", "generate_plan")
        .connect("generate_plan", {
            "show_plan": "show_plan",
            "error_handling": "error_handling"
        })
        .connect("show_plan", "ask_follow_up")
        .connect("ask_follow_up", {
            "input_basic_info": "1",
            "show_shopping_list": "2",
            "export_pdf": "3",
            "end": "4"
        })
        .set_default_error_handler("error_handling")
    )

    return workflow