clc;
clear;
clear all;

[c] = adder(2, 3);
disp(c);

[c] = mean(1 ,2, 3);
disp(c);


function [c] = adder(a, b, d)
    c = (a + b + d) / 3;
end

function [c] = mean(a, b, d)
    c = (a + b + d) / 3;
end
