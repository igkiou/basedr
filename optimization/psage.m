function [wavg, w] = psage(w0, params, obj_grad_func, proj_func)

[wavg, w] = projected_stochastic_accelerated_gradient(w0, params, obj_grad_func, proj_func);
