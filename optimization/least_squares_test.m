dim = 100;
var = 100;
mag = 100;
numIters = 100;
params = optimparams('numIters', numIters);
m = mag * randn(dim, 1);
func = @(x)least_squares_obj_grad(x, m + var * randn(dim, 1));
[wavg w] = stochastic_gradient(zeros(dim, 1), params, func); 
[norm(wavg - m) / norm(m), norm(w - m) / norm(m), maxv(abs(wavg - m) ./ abs(m)), maxv(abs(w - m) ./ abs(m))]
