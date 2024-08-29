clc;
clear;
clear all;
im1 = imread("cameraman.tif");
imresult = edge(im1);
imshow(imresult);
