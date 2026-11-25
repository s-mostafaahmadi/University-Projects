clc;
clear;
close all;

% Quadratic equation: a*x^2 + b*x + c = 0
a = input('Enter a: ');
b = input('Enter b: ');
c = input('Enter c: ');

if a == 0
    error('a must be non-zero for a quadratic equation.');
end

d = Delta_function(a, b, c);
[x1, x2] = Quadratic_roots(a, b, d);

fprintf('delta = %g\n', d);
fprintf('x1 = %s\n', num2str(x1));
fprintf('x2 = %s\n', num2str(x2));
