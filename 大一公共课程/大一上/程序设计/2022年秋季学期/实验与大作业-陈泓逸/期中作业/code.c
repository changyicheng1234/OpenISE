#include<stdio.h>
int main()
{
	//定义变量 
	double cloth_S,cloth_P,cloth_C;
	double foodWine,foodBeer,foodSmoke,foodMutton,foodBeef,foodPork,foodKitchen,foodEgg;
	double foodFish,foodDuck,foodPeanut,foodYoghurt,foodBroccoli,foodBeanCurd,foodMilk;
	double foodTomato,foodBean,foodCorn,foodPotato,foodTurnip,foodRice;
	double houseWash,houseAirConditioner,houseLight,houseFan,houseLapTop,houseWater;
	double travelCar,travelTrain,travelBus,travelSubway,travelBike,travelElevator;
	double useChopsticks,usePlastic,useShampoo;
	double _cloth=0,_food=0,_house=0,_travel=0,_use=0,_all=0; 
	
	//提示输入变量的值并读入 
	printf("衣：\n您使用的洗衣液（瓶）："); scanf("%lf",&cloth_S);
	printf("您购买的涤纶织物（件）："); scanf("%lf",&cloth_P);
	printf("您购买的棉制衣物（件）："); scanf("%lf",&cloth_C);
	printf("\n食：\n您饮用的白酒（瓶）："); scanf("%lf",&foodWine);
	printf("您饮用的啤酒（瓶）："); scanf("%lf",&foodBeer);
	printf("您吸的烟（包）："); scanf("%lf",&foodSmoke);
	printf("您食用的羊肉（千克）："); scanf("%lf",&foodMutton);
	printf("您食用的牛肉（千克）："); scanf("%lf",&foodBeef);
	printf("您食用的猪肉（千克）："); scanf("%lf",&foodPork);
	printf("您食用的鸡肉（千克）："); scanf("%lf",&foodKitchen);
	printf("您食用的鸡蛋（千克）："); scanf("%lf",&foodEgg);
	printf("您食用的鱼肉（千克）："); scanf("%lf",&foodFish);
	printf("您食用的鸭肉（千克）："); scanf("%lf",&foodDuck);
	printf("您食用的花生（千克）："); scanf("%lf",&foodPeanut);
	printf("您食用的酸奶（盒）："); scanf("%lf",&foodYoghurt);
	printf("您食用的西兰花（千克）："); scanf("%lf",&foodBroccoli);
	printf("您食用的豆腐（千克）："); scanf("%lf",&foodBeanCurd);
	printf("您食用的牛奶（盒）："); scanf("%lf",&foodMilk);
	printf("您食用的西红柿（千克）："); scanf("%lf",&foodTomato);
	printf("您食用的扁豆（千克）："); scanf("%lf",&foodBean);
	printf("您食用的玉米（个）："); scanf("%lf",&foodCorn);
	printf("您食用的土豆（千克）："); scanf("%lf",&foodPotato);
	printf("您食用的萝卜（千克）："); scanf("%lf",&foodTurnip);
	printf("您食用的米饭（千克）："); scanf("%lf",&foodRice);
	printf("\n住：\n您洗的热水澡（次）："); scanf("%lf",&houseWash);
	printf("您使用的空调（小时）："); scanf("%lf",&houseAirConditioner);
	printf("您使用的节能灯（小时）："); scanf("%lf",&houseLight);
	printf("您使用的风扇（小时）："); scanf("%lf",&houseFan);
	printf("您使用的笔记本电脑（小时）："); scanf("%lf",&houseLapTop);
	printf("您使用的水（吨）："); scanf("%lf",&houseWater);
	printf("\n行：\n您乘坐/驾驶的小汽车（千米）："); scanf("%lf",&travelCar);
	printf("您乘坐/驾驶的火车（千米）："); scanf("%lf",&travelTrain);
	printf("您乘坐/驾驶的公共汽车（站）："); scanf("%lf",&travelBus);
	printf("您乘坐/驾驶的地铁（站）："); scanf("%lf",&travelSubway);
	printf("您乘坐/驾驶的小电车（千米）："); scanf("%lf",&travelBike);
	printf("您乘坐的电梯（层）："); scanf("%lf",&travelElevator);
	printf("\n用：\n您使用的一次性筷子（双）："); scanf("%lf",&useChopsticks);
	printf("您使用的塑料袋（个）："); scanf("%lf",&usePlastic);
	printf("您使用的洗发水（瓶）："); scanf("%lf",&useShampoo);
	
	//分别计算碳排放量 
	cloth_S*=0.80; cloth_P*=25.70; cloth_C*=7.00;
	foodWine*=1.76; foodBeer*=0.22; foodSmoke*=0.02; foodMutton*=39.20;
	foodBeef*=27.00; foodPork*=12.10; foodKitchen*=1.80; foodEgg*=4.80;
	foodFish*=4.40; foodDuck*=3.10; foodPeanut*=2.50; foodYoghurt*=1.10;
	foodBroccoli*=2.00; foodBeanCurd*=2.00; foodMilk*=0.90; foodTomato*=1.10;
	foodBean*=0.90; foodCorn*=0.35; foodPotato*=2.90; foodTurnip*=0.014;
	foodRice*=2.70; houseWash*=0.42; houseAirConditioner*=0.621;
	houseLight*=0.011; houseFan*=0.045; houseLapTop*=0.013; houseWater*=0.194;
	travelCar*=0.30; travelTrain*=0.01; travelBus*=0.01; travelSubway*=0.10;
	travelBike*=0.055; travelElevator*=0.218; useChopsticks*=0.005;
	usePlastic*=0.10; useShampoo*=0.02;
	_cloth=cloth_S+cloth_P+cloth_C;
	_food=foodWine+foodBeer+foodSmoke+foodMutton+foodBeef+foodPork+foodKitchen+foodEgg
	      +foodFish+foodDuck+foodPeanut+foodYoghurt+foodBroccoli+foodBeanCurd+foodMilk
		  +foodTomato+foodBean+foodCorn+foodPotato+foodTurnip+foodRice;
	_house=houseWash+houseAirConditioner+houseLight+houseFan+houseLapTop+houseWater;
	_travel=travelCar+travelTrain+travelBus+travelSubway+travelBike+travelElevator;
	_use=useChopsticks+usePlastic+useShampoo;
	_all=_cloth+_food+_house+_travel+_use;
	
	//格式化（子单元缩进）输出碳排放量 
	printf("\n\n\n您在生活中产生的碳排放总量为：%.3lfkg\n",_all);
	printf("\t您在“衣”方面产生的碳排放量为：%.3lfkg\n",_cloth);
	printf("\t\t您使用洗衣液产生的碳排放量为：%.3lfkg\n",cloth_S);
	printf("\t\t您购买涤纶织物产生的碳排放量为：%.3lfkg\n",cloth_P);
	printf("\t\t您购买棉制衣物产生的碳排放量为：%.3lfkg\n",cloth_C);
	printf("\t您在“食”方面产生的碳排放量为：%.3lfkg\n",_food);
	printf("\t\t您饮用白酒产生的碳排放量为：%.3lfkg\n",foodWine);
	printf("\t\t您饮用啤酒产生的碳排放量为：%.3lfkg\n",foodBeer);
	printf("\t\t您吸烟产生的碳排放量为：%.3lfkg\n",foodSmoke);
	printf("\t\t您食用羊肉产生的碳排放量为：%.3lfkg\n",foodMutton);
	printf("\t\t您食用牛肉产生的碳排放量为：%.3lfkg\n",foodBeef);
	printf("\t\t您食用猪肉产生的碳排放量为：%.3lfkg\n",foodPork);
	printf("\t\t您食用鸡肉产生的碳排放量为：%.3lfkg\n",foodKitchen);
	printf("\t\t您食用鸡蛋产生的碳排放量为：%.3lfkg\n",foodEgg);
	printf("\t\t您食用鱼肉产生的碳排放量为：%.3lfkg\n",foodFish);
	printf("\t\t您食用鸭肉产生的碳排放量为：%.3lfkg\n",foodDuck);
	printf("\t\t您食用花生产生的碳排放量为：%.3lfkg\n",foodPeanut);
	printf("\t\t您食用酸奶产生的碳排放量为：%.3lfkg\n",foodYoghurt);
	printf("\t\t您食用西兰花产生的碳排放量为：%.3lfkg\n",foodBroccoli);
	printf("\t\t您食用豆腐产生的碳排放量为：%.3lfkg\n",foodBeanCurd);
	printf("\t\t您食用牛奶产生的碳排放量为：%.3lfkg\n",foodMilk);
	printf("\t\t您食用西红柿产生的碳排放量为：%.3lfkg\n",foodTomato);
	printf("\t\t您食用扁豆产生的碳排放量为：%.3lfkg\n",foodBean);
	printf("\t\t您食用玉米产生的碳排放量为：%.3lfkg\n",foodCorn);
	printf("\t\t您食用土豆产生的碳排放量为：%.3lfkg\n",foodPotato);
	printf("\t\t您食用萝卜产生的碳排放量为：%.3lfkg\n",foodTurnip);
	printf("\t\t您食用米饭产生的碳排放量为：%.3lfkg\n",foodRice);
	printf("\t您在“住”方面产生的碳排放量为：%.3lfkg\n",_house);
	printf("\t\t您洗热水澡产生的碳排放量为：%.3lfkg\n",houseWash);
	printf("\t\t您使用空调产生的碳排放量为：%.3lfkg\n",houseAirConditioner);
	printf("\t\t您使用节能灯产生的碳排放量为：%.3lfkg\n",houseLight);
	printf("\t\t您使用风扇产生的碳排放量为：%.3lfkg\n",houseFan);
	printf("\t\t您使用笔记本电脑产生的碳排放量为：%.3lfkg\n",houseLapTop);
	printf("\t\t您使用水产生的碳排放量为：%.3lfkg\n",houseWater);
	printf("\t您在“行”方面产生的碳排放量为：%.3lfkg\n",_travel);
	printf("\t\t您乘坐/驾驶小汽车产生的碳排放量为：%.3lfkg\n",travelCar);
	printf("\t\t您乘坐/驾驶火车产生的碳排放量为：%.3lfkg\n",travelTrain);
	printf("\t\t您乘坐/驾驶公共汽车产生的碳排放量为：%.3lfkg\n",travelBus);
	printf("\t\t您乘坐/驾驶地铁产生的碳排放量为：%.3lfkg\n",travelSubway);
	printf("\t\t您乘坐/驾驶小电车产生的碳排放量为：%.3lfkg\n",travelBike);
	printf("\t\t您乘坐电梯产生的碳排放量为：%.3lfkg\n",travelElevator);
	printf("\t您在“用”方面产生的碳排放量为：%.3lfkg\n",_use);
	printf("\t\t您使用一次性筷子产生的碳排放量为：%.3lfkg\n",useChopsticks);
	printf("\t\t您使用塑料袋产生的碳排放量为：%.3lfkg\n",usePlastic);
	printf("\t\t您使用洗发水产生的碳排放量为：%.3lfkg\n",useShampoo);
	return 0;
}
