function [wavg, w] = eda(w0, params, obj_grad_func, D)

[wavg, w] = exponentiated_dual_averaging(w0, params, obj_grad_func, D);
