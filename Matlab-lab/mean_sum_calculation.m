clc;
clear;

% adder takes two numbers and returns their sum (exercise 1).
c = adder(2, 3);
fprintf('adder(2, 3) = %g\n', c);

% average of three numbers (exercise 1, second part).
c = mean_of_three(1, 2, 3);
fprintf('mean_of_three(1, 2, 3) = %g\n', c);

function c = adder(a, b)
    c = a + b;
end

function c = mean_of_three(a, b, d)
    c = (a + b + d) / 3;
end
