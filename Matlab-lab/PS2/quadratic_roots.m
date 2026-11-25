function [x1, x2] = Quadratic_roots(a, b, delta)
% Quadratic_roots  Roots of a*x^2 + b*x + c = 0 given its delta.
% The file name must match the function name for MATLAB to find it.
    if a == 0
        error('Quadratic_roots: a must be non-zero.');
    end

    x1 = (-b + sqrt(delta)) / (2 * a);
    x2 = (-b - sqrt(delta)) / (2 * a);
end
