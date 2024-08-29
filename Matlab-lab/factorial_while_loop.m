clc;
clear all;
x = input("Enter your number :");
y = 1;
while x > 0
    y = y * x;
    x = x - 1;
end
disp(y);
