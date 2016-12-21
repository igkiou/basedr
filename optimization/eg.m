function [wavg, w] = eg(w0, params, obj_grad_func, D)

[wavg, w] = exponentiated_gradient(w0, params, obj_grad_func, D);
