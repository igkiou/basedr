%%
% dim = 10;
% var = 1;
% numIters = 100;
% m = abs(randn(dim, 1));
% m = m / sum(m);
% params = optimparams('numIters', numIters);
% func = @(w)linear_regression_obj_grad_sample(w, m, var);
% [wavg w] = stochastic_gradient(ones(dim, 1) / dim, params, func); 
% [norm(wavg - m)/norm(m), norm(w - m)/norm(m), maxv(abs(wavg - m)./abs(m)), maxv(abs(w - m)./abs(m))]
% 
% %%
% dim = 10;
% var = 1;
% numIters = 100;
% m = abs(randn(dim, 1));
% m = m / sum(m);
% params = optimparams('numIters', numIters);
% X = randn(dim, dim * 500);
% func = @(w)linear_regression_obj_grad_sample2(w, m, X, var);
% [wavg w] = stochastic_gradient(ones(dim, 1) / dim, params, func); 
% [norm(wavg - m)/norm(m), norm(w - m)/norm(m), maxv(abs(wavg - m)./abs(m)), maxv(abs(w - m)./abs(m))]


%%
dim = 100;
var = 0.1;
numIters = 1000;
sampleMultiplier = 500;
numProblems = 10;

for iterProblem = 1:numProblems,
	fprintf('iter %d\n', iterProblem);
	m = abs(randn(dim, 1));
	m = m / sum(m);
	params = optimparams('numIters', numIters);
	X = randn(dim, dim * sampleMultiplier);
	func = @(w)linear_regression_obj_grad_sample2(w, m, X, var);

	[wavg w] = projected_stochastic_gradient(ones(dim, 1) / dim, params, func, @(x)simplex_projection_vector(x, 1)); 
	fprintf('SGD   %g %g %g %g\n', norm(wavg - m)/norm(m), norm(w - m)/norm(m), maxv(abs(wavg - m)./abs(m)), maxv(abs(w - m)./abs(m)));

	[wavg w] = projected_dual_averaging(ones(dim, 1) / dim, params, func, @(x)simplex_projection_vector(x, 1)); 
	fprintf('PDA   %g %g %g %g\n', norm(wavg - m)/norm(m), norm(w - m)/norm(m), maxv(abs(wavg - m)./abs(m)), maxv(abs(w - m)./abs(m)));

	[wavg w] = exponentiated_gradient(ones(dim, 1) / dim, params, func, 1); 
	fprintf('EG    %g %g %g %g\n', norm(wavg - m)/norm(m), norm(w - m)/norm(m), maxv(abs(wavg - m)./abs(m)), maxv(abs(w - m)./abs(m)));

	[wavg w] = exponentiated_dual_averaging(ones(dim, 1) / dim, params, func, 1); 
	fprintf('EDA   %g %g %g %g\n', norm(wavg - m)/norm(m), norm(w - m)/norm(m), maxv(abs(wavg - m)./abs(m)), maxv(abs(w - m)./abs(m)));

	[wavg w] = projected_accelerated_dual_averaging(ones(dim, 1) / dim, params, func, @(x)simplex_projection_vector(x, 1)); 
	fprintf('PADA  %g %g %g %g\n', norm(wavg - m)/norm(m), norm(w - m)/norm(m), maxv(abs(wavg - m)./abs(m)), maxv(abs(w - m)./abs(m)));

	[wavg w] = projected_stochastic_accelerated_gradient(ones(dim, 1) / dim, params, func, @(x)simplex_projection_vector(x, 1)); 
	fprintf('PSAGE %g %g %g %g\n', norm(wavg - m)/norm(m), norm(w - m)/norm(m), maxv(abs(wavg - m)./abs(m)), maxv(abs(w - m)./abs(m)));
end;
