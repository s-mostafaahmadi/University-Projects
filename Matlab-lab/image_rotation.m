clc;
clear;
clear all;
close all;
x = imread("cameraman.tif");
o = imrotate(x, 60);
imshow(o);
