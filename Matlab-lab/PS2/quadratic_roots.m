function [x1, x2] = get_x(a, b, delta)
    x1 = (-b + sqrt(delta)) / (2 * a);
    x2 = (-b - sqrt(delta)) / (2 * a);
end
