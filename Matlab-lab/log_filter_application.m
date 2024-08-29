clc;
clear;
clear all;
close all;
x = imread("peppers.png", "png");
z = fspecial("log", 80);
y = imfilter(x, z);
imshow(y);
