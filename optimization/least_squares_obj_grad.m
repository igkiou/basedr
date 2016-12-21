function [val deriv] = least_squares_obj_grad(m, s)

val = sum((m - s) .^ 2);
deriv = 2 * (m - s);
