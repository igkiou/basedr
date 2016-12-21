function [val deriv] = linear_regression_obj_grad(w, x, y)

val = sum((y - x' * w) .^ 2) / length(y);
deriv = 2 * x * (x' * w - y) / length(y);
