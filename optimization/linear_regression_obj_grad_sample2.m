function [val deriv] = linear_regression_obj_grad_sample2(w, m, X, var)

y = X' * m;
[val deriv] = linear_regression_obj_grad(w, X, y);
deriv = deriv + var * randn(size(m));
