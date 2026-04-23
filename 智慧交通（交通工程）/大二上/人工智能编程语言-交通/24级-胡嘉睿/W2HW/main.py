print("要求1：")
# 获取输入的学生成绩字符串

rule = "输入字符串以提示信息开始，提示用户输入班级内的同学及其成绩分数。\n每个学生的信息包括学生姓名和三门课程的成绩，分别是高数成绩、英语成绩和大物成绩。\n学生信息之间用分号 ; 分隔。\n每个学生的信息格式为：学生姓名,高数成绩,英语成绩,大物成绩。\n学生成绩之间用逗号 , 分隔。"

raw_str = input(f"请输入班级内的同学及其成绩分数，格式要求：\n{rule}\n---\n样例如下： \n学生姓名,高数成绩,英语成绩,大物成绩;SanZhang,70,80,61;SiLi,86,77,81;WuWang,88,90, 77;MingLi,60,77,81;MiWang,71,70,60;HaiLi,88,78,89;HeWang,70,90,80;LiWang,67,71,70\n")

print("要求2：")
# 按分号分割字符串，得到每个学生的成绩信息
list_str = raw_str.split(";")
print(list_str)

print("要求3：")
# 将每个学生的成绩信息按逗号分割，形成嵌套列表
list_in_list = [i.split(",") for i in list_str]
print(list_in_list)

print("要求4：")
# 将嵌套列表转换为字典列表
head = list_in_list[0]
list_of_dict = [ {head[n] : i[n] for n in range(4)}  for i in list_in_list[1:]]
print(list_of_dict)

print("要求5：")
# 按总分从高到低排序并输出学生姓名
list_of_dict.sort(key=lambda x: x["高数成绩"] + x["大物成绩"] + x["英语成绩"], reverse=True)
print("总分从高到低：")
for i in list_of_dict:
    print(i["学生姓名"], end=' ')
print()

# 按总分从低到高排序并输出学生姓名
list_of_dict.sort(key=lambda x: x["高数成绩"] + x["大物成绩"] + x["英语成绩"])
print("总分从低到高：")
for i in list_of_dict:
    print(i["学生姓名"], end=' ')
print()

# 按各科成绩从高到低排序并输出学生姓名
for subject in ["高数成绩", "英语成绩", "大物成绩"]:
    list_of_dict.sort(key=lambda x: x[subject], reverse=True)
    print(f"{subject}从高到低：")
    for i in list_of_dict:
        print(i["学生姓名"], end=' ')
    print()
