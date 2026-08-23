clear;
clc;

% PID 参数
kp = 1.5; ki = 0.8; kd = 0;
% 最大时间
Time = 100;
% 时间步长
tk = 1;
% 起始值和期望值
init = 0;
target = 96;
% 定义传递函数
obj_sys = tf(1, [1 4 4 1]);
% 定义扰动
diff_sys = tf(50, [1 4 4 1]);
diff_sys = c2d(diff_sys, tk, 'tustin');
diff_opt = step(diff_sys, Time);
obj_sys = feedback(obj_sys, 1);

% 绘制结果
figure('Position', [100 100 1500 500]);
t = 1:Time;
[sys_opt1, pid_opt1, err_opt1] = PID_controller(kp, ki, kd, obj_sys, diff_opt, init, target, Time, tk);
diff_opt = zeros(1, Time);
[sys_opt2, pid_opt2, err_opt2] = PID_controller(kp, ki, kd, obj_sys, diff_opt, init, target, Time, tk);
subplot(1, 2, 1);
plot(t, sys_opt1, 'LineWidth', 1.2);
hold on;
title('PID 仿真结果展示（有扰动）');
xlabel('t/min'); ylabel('y(t)/mmHg');
grid on;
subplot(1, 2, 2);
plot(t, sys_opt2, 'LineWidth', 1.2);
title('PID 仿真结果展示（无扰动）');
xlabel('t/min'); ylabel('y(t)/mmHg');
grid on;

function [sys_opt, pid_opt, err_opt] = PID_controller(kp, ki, kd, obj_sys, diff_opt, init, target, max_time, tk)
    % 计算PID控制下的系统输出
    
    % param: kp, ki, kd - PID参数
    %        obj_sys - 被控系统
    %        diff_opt - 扰动输出
    %        init - 系统输出初始值
    %        target - 系统期望输出
    %        max_time - 最大模拟时间
    %        tk - 时间步长
    % return: sys_opt - 系统输出
    %         pid_opt - pid控制器输出
    %         err_opt - 误差输出

    % 参数验证
    if (kp <= 0 || ki < 0 || kd < 0 || max_time <= 0 || tk <= 0)
        error('参数错误');
    end
    % 累计误差
    err_sum = 0;
    % 将传递函数转换成离散形式
    obj_sys = c2d(obj_sys, tk, 'tustin');
    % 获取离散系统模型的分子分母矩阵
    [num, den] = tfdata(obj_sys, 'v');
    % 系统输出
    sys_opt = zeros(1, max_time);
    sys_opt(1) = init;
    % pid 控制器输出
    pid_opt = zeros(1, max_time);
    % 误差输出
    err_opt = zeros(1, max_time);
    err_opt(1) = target - init; err_sum = err_sum + err_opt(1);
    % pid 模拟
    for k=2:max_time
        sys_opt(k) = -den(2) * sys_opt(k-1) + num(1) * pid_opt(k) + num(2) * pid_opt(k-1) - diff_opt(k);
        err_opt(k) = target - sys_opt(k);
        pid_opt(k) = kp * err_opt(k) + ki * err_sum + kd * (err_opt(k) - err_opt(k-1));
        err_sum = err_sum + err_opt(k);
    end
end