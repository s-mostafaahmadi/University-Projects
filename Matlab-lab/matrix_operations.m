clc;
clear all;
a = zeros(4, 2);
b = ones(3, 5);
mat = [1 2 3 4 5 6; 7 8 9 10 11 12; 13 14 15 16 17 18; 19 20 21 22 23 24];
z = zeros(4, 6);
z(3, 2:5) = mat(3, 2:5) * 6;
