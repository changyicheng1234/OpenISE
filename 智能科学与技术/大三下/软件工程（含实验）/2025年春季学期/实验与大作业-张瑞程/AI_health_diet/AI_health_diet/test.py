import unittest
from flask import session
from main import app  # 假设你的主程序文件名为app.py


class DietAppTestCase(unittest.TestCase):
    def setUp(self):
        """测试前准备：配置测试客户端并开启会话支持"""
        app.config['TESTING'] = True
        app.config['SECRET_KEY'] = 'test_secret_key'  # 匹配app中的secret_key
        self.client = app.test_client()
        self.app_context = app.app_context()
        self.app_context.push()

    def tearDown(self):
        """测试后清理"""
        self.app_context.pop()

    # ====================== 用户资料模块测试 ======================
    def test_save_profile_normal_case(self):
        """测试正常用户资料保存及健康数据计算"""
        data = {
            "gender": "male",
            "age": 30,
            "height": 175,
            "weight": 70,
            "activity": "moderate",
            "health_goal": "lose_weight"
        }
        response = self.client.post('/save_profile', json=data)
        result = response.get_json()

        # 验证基本数据
        self.assertEqual(response.status_code, 200)
        self.assertAlmostEqual(result['bmi'], 22.86, places=2)
        self.assertEqual(result['bmr'], 1696)
    def test_save_profile_normal_case_2(self):
        """测试正常用户资料保存及健康数据计算"""
        data = {
            "gender": "female",
            "age": 40,
            "height": 160,
            "weight": 55,
            "activity": "moderate",
            "health_goal": "lose_weight"
        }
        response = self.client.post('/save_profile', json=data)
        result = response.get_json()

        # 验证基本数据
        self.assertEqual(response.status_code, 200)
        self.assertAlmostEqual(result['bmi'], 21.48, places=2)
        self.assertEqual(result['bmr'], 1283)



    # ====================== 饮食推荐模块测试 ======================
    def test_recommend_with_profile(self):
        """测试已保存用户资料后的饮食推荐"""
        # 先保存用户资料
        self.client.post('/save_profile', json={
            "gender": "female",
            "age": 25,
            "height": 160,
            "weight": 55,
            "activity": "low",
            "health_goal": "maintain"
        })

        # 发起推荐请求
        response = self.client.post('/recommend', json={
            "health_goal": "maintain",
            "dietary_preferences": "low_fat"
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 200)
        self.assertIn('早餐推荐', result)
        self.assertIn('推荐食物', result['早餐推荐'])
        self.assertTrue(len(result['早餐推荐']['推荐食物']) > 0, "早餐推荐食物列表为空")

        self.assertIn('午餐推荐', result)
        self.assertIn('推荐食物', result['午餐推荐'])
        self.assertTrue(len(result['午餐推荐']['推荐食物']) > 0, "午餐推荐食物列表为空")

        self.assertIn('晚餐推荐', result)
        self.assertIn('推荐食物', result['晚餐推荐'])
        self.assertTrue(len(result['晚餐推荐']['推荐食物']) > 0, "晚餐推荐食物列表为空")



    # ====================== 餐食分析模块测试 ======================
    def test_analyze_valid_meals(self):
        """测试有效餐食分析"""
        response = self.client.post('/analyze', json={
            "meals": {
                "早餐": "鸡蛋+牛奶",
                "午餐": "鸡胸肉+西兰花+糙米饭",
                "晚餐": "鱼肉+菠菜"
            }
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 200)
        self.assertNotEqual(result['早餐分析'], '暂无分析数据')
        self.assertIn('营养建议', result)

    def test_analyze_empty_meals(self):
        """测试空餐食数据处理"""
        response = self.client.post('/analyze', json={"meals": {}})
        result = response.get_json()
        print("分析失败，请重试")
        self.assertEqual(response.status_code, 200)


    # ====================== 菜谱生成模块测试 ======================
    def test_generate_valid_recipe(self):
        """测试正常菜谱生成"""
        response = self.client.post('/recipe', json={
            "recipe_name": "番茄炒蛋",
            "location": "中国"
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 200)
        self.assertTrue(len(result['ingredients_list']) > 0)
        self.assertTrue(len(result['cooking_process']) > 0)

        # ====================== 外卖推荐模块测试 ======================

    def test_takeout_valid_request(self):
        """测试有效外卖推荐请求"""
        response = self.client.post('/takeout', json={
            "takeout_type": "中餐",
            "price": "20",
            "location": "北京市"
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 200)
        self.assertIsInstance(result['dishes_list'], list)
        self.assertIsInstance(result['nutritional_analysis'], dict)
        if result['dishes_list']:
            self.assertIn('name', result['dishes_list'][0])
            self.assertIn('price', result['dishes_list'][0])

    def test_takeout_missing_parameter(self):
        """测试缺少必要参数的外卖推荐请求"""
        response = self.client.post('/takeout', json={
            "takeout_type": "中餐",
            "location": "北京市"
            # 缺少price参数
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 500)  # 根据实际代码可能返回400或500
        self.assertIn('error', result)

        # ====================== 经济型饮食模块测试 ======================

    def test_economical_valid_request(self):
        """测试有效经济型饮食推荐请求"""
        response = self.client.post('/economical', json={
            "budget": 50,
            "location": "上海市",
            "health_goal": "lose_weight",
            "dietary_preferences": "vegetarian"
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 200)
        self.assertIsInstance(result['早餐推荐'], dict)
        self.assertIsInstance(result['午餐推荐'], dict)
        self.assertIsInstance(result['晚餐推荐'], dict)
        self.assertIsInstance(result['省钱建议'], list)
        self.assertIsInstance(result['总费用'], (int, float))
        self.assertGreaterEqual(result['总费用'], 0)

    def test_economical_zero_budget(self):
        """测试预算为0的经济型饮食推荐请求"""
        response = self.client.post('/economical', json={
            "budget": 0,
            "location": "广州市",
            "health_goal": "maintain",
            "dietary_preferences": ""
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 200)  # 根据业务逻辑可能返回400或500


    def test_economical_missing_budget(self):
        """测试缺少预算参数的经济型饮食推荐请求"""
        response = self.client.post('/economical', json={
            "location": "深圳市",
            "health_goal": "gain_muscle",
            "dietary_preferences": "low_carb"
            # 缺少budget参数
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 500)  # 根据实际代码可能返回400或500
        self.assertIn('error', result)
    # ====================== 减肥计划模块测试 ======================
    def test_weight_loss_valid_goal(self):
        """测试有效减肥目标"""
        response = self.client.post('/weight_loss', json={
            "current_weight": 80,
            "target_weight": 75,
            "days": 30,
            "gender": "female",
            "age": 35,
            "height": 165,
            "activity_level": "moderate"
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 200)
        self.assertIn('计划概览', result)
        self.assertTrue(len(result['每日食谱']['早餐']['菜单']) > 0)

    def test_weight_loss_invalid_goal(self):
        """测试目标体重不低于当前体重"""
        response = self.client.post('/weight_loss', json={
            "current_weight": 70,
            "target_weight": 75,
            "days": 30,
            "gender": "female",
            "age": 35,
            "height": 165,
            "activity_level": "moderate"
        })
        result = response.get_json()

        self.assertEqual(response.status_code, 200)



if __name__ == '__main__':
    unittest.main()