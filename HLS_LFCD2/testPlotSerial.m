% USING MATLAB 2020a
clc;
%clear all;
theta = linspace(0,359,360);
theta_radians = deg2rad(theta);

serialportlist("available")
arduino=serialport("COM8",230400);
arduino.Terminator%'/n':LF,'\r':CR
%configureTerminator(device,"CR")
arduino.Timeout = 20;
%configureTerminator(arduino,"CR/LF");
flush(arduino);


%clear arduino

write(arduino,"b","string");

pause(10);

write(arduino,"f","string");

countOut = 0;
while countOut < 10000
    data = readline(arduino);
    disp(data);
    if (contains(data,'start'))
        disp(data);
        break;
    end
    countOut = countOut+1;
end

disp("out loop");

data2 = linspace(0,359,360);
for c = 1:360
    data2(c) = readline(arduino);
end

write(arduino,"e","string");

disp(data2);
clear arduino


disp('making plot..');

polarplot(theta_radians,data2,'bo');
maxlimit = 1500;%3500
rlim([0 maxlimit]);%[0 3500]
hold on
polarplot(theta_radians,data2,'r--');
hold off
