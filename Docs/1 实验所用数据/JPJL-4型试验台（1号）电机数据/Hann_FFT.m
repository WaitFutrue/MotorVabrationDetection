% 1. 读取数据并验证
% try
%     data = readmatrix('0_unpowered_1.csv'); % 尝试读取纯数值数据
% catch
%     data_table = readtable('0_unpowered_1.csv'); % 如果有表头，使用readtable
%     data = table2array(data_table); % 转换为数值矩阵
% end
data = readmatrix('9_1500RPM_3.csv'); % 尝试读取纯数值数据

% 提取三轴数据
x = data(:,1);
y = data(:,2);
z = data(:,3);

% 验证数据
disp('数据维度：');
disp(size(data));
disp('前5行数据：');
disp(data(1:5, :));

% 检查数据长度
disp(['X轴数据长度: ', num2str(length(x))]);
disp(['Y轴数据长度: ', num2str(length(y))]);
disp(['Z轴数据长度: ', num2str(length(z))]);

% 2. 绘制原始信号
CSV_LENGTH=6400;
figure;
subplot(3,1,1);
plot(x(1:min(CSV_LENGTH, length(x)))); % 确保索引不超过数据长度
title('原始X轴振动信号');
xlabel('采样点'); ylabel('加速度');

subplot(3,1,2);
plot(y(1:min(CSV_LENGTH, length(y))));
title('原始Y轴振动信号');
xlabel('采样点'); ylabel('加速度');

subplot(3,1,3);
plot(z(1:min(CSV_LENGTH, length(z))));
title('原始Z轴振动信号');
xlabel('采样点'); ylabel('加速度');

shg % 强制显示图形窗口

%在进行FFT之前，先加入一个汉宁窗
% 1. 汉宁窗
window = hanning(length(x)); % 创建汉宁窗
x = x .* window; % 对X轴数据加窗
y = y .* window; % 对Y轴数据加窗
z = z .* window; % 对Z轴数据加窗

% 2. 绘制加窗后的信号
figure;
subplot(3,1,1);
plot(x(1:min(CSV_LENGTH, length(x)))); % 确保索引不超过数据长度
title('加窗后的X轴振动信号');
xlabel('采样点'); ylabel('加速度');
subplot(3,1,2);
plot(y(1:min(CSV_LENGTH, length(y))));
title('加窗后的Y轴振动信号');
xlabel('采样点'); ylabel('加速度');
subplot(3,1,3);
plot(z(1:min(CSV_LENGTH, length(z))));
title('加窗后的Z轴振动信号');
xlabel('采样点'); ylabel('加速度');
shg % 强制显示图形窗口

%对三个通道分别频谱分析，然后再绘制
% 3. 频谱分析
Fs = 3200; % 采样频率
N = length(x); % 数据长度
f = (0:N-1)*(Fs/N); % 频率范围
Y_x = fft(x); % X轴FFT
Y_y = fft(y); % Y轴FFT
Y_z = fft(z); % Z轴FFT
P2_x = abs(Y_x/N); % 双边谱
P1_x = P2_x(1:N/2+1); % 单边谱
P1_x(2:end-1) = 2*P1_x(2:end-1); % 单边谱修正
P2_y = abs(Y_y/N); % 双边谱
P1_y = P2_y(1:N/2+1); % 单边谱
P1_y(2:end-1) = 2*P1_y(2:end-1); % 单边谱修正
P2_z = abs(Y_z/N); % 双边谱
P1_z = P2_z(1:N/2+1); % 单边谱
P1_z(2:end-1) = 2*P1_z(2:end-1); % 单边谱修正
f = f(1:N/2+1); % 频率范围修正
% 绘制频谱
figure;
subplot(3,1,1);
plot(f, P1_x);
title('X轴频谱');
xlabel('频率 (Hz)'); ylabel('幅值');
subplot(3,1,2);
plot(f, P1_y);
title('Y轴频谱');
xlabel('频率 (Hz)'); ylabel('幅值');
subplot(3,1,3);
plot(f, P1_z);
title('Z轴频谱');
xlabel('频率 (Hz)'); ylabel('幅值');
shg % 强制显示图形窗口
