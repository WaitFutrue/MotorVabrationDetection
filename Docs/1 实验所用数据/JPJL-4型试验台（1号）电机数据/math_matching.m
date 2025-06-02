% 示例数据（替换为实际数据）
rpm = [750,900,1050, 1200, 1350, 1500];
frequency = [119.613, 143.52,167.846,191.0707,215.4033,239.4943];

% 有截距的线性拟合
p = polyfit(rpm, frequency, 1);
a = p(1);
b = p(2);

% 无截距的拟合
A = rpm';
a_no_intercept = A \ frequency';

% 理论斜率
k_theory = 1/60;

% 输出结果
fprintf('有截距拟合: 频率 = %.6f * RPM + %.2f\n', a, b);
fprintf('无截距拟合: 频率 = %.6f * RPM\n', a_no_intercept);
fprintf('理论斜率: %.6f\n', k_theory);

% 绘图
scatter(rpm, frequency, 'b', 'DisplayName', '实际数据');
hold on;
plot(rpm, a*rpm + b, 'r', 'DisplayName', sprintf('有截距拟合: y=%.4fx+%.2f', a, b));
plot(rpm, a_no_intercept*rpm, 'g--', 'DisplayName', sprintf('无截距拟合: y=%.4fx', a_no_intercept));
xlabel('转速 (RPM)');
ylabel('频率 (Hz)');
legend;
hold off;