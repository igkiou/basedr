function [val deriv] = linear_regression_obj_grad_sample(w, m, var)

x = randn(size(w));
y = var * randn(1, 1) + m' * x;
% y = w' * x;
[val deriv] = linear_regression_obj_grad(w, x, y);
