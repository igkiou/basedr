function [wavg, w] = pada(w0, params, obj_grad_func, proj_func)

[wavg, w] = projected_accelerated_dual_averaging(w0, params, obj_grad_func, proj_func);
