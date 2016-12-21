function [wavg, w] = psgd(w0, params, obj_grad_func, proj_func)

[wavg, w] = projected_stochastic_gradient(w0, params, obj_grad_func, proj_func);
