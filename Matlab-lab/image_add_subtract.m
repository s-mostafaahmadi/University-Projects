clc;
clear;
clear all;
im1 = imread("Lenna.png");
im2 = imread("image2.png");
im3 = imadd(im1, im2);
im4 = imsubtract(im1, im2);
imshow(im3);
