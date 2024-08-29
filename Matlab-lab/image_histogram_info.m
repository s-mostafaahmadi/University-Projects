clc;
clear;
clear all;
close all;
p = imread("peppers.png", "png");
imhist(p);
c = whos("p");
p_info = imfinfo("peppers.png");
