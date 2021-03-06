function [wavg, w] = projected_stochastic_accelerated_gradient(w0, params, obj_grad_func, proj_func)

if ((~exist('w0', 'var')) || (isempty(w0))),
	error('Initialization W0 must be provided.');
end;

if ((~exist('params', 'var')) || (isempty(params))),
	params = optimparams;
end;

if (~isstruct(params)),
	error('PARAMS must be a struct.');
end;

if (~isfield(params, 'numIters')),
	error('When used with projected_stochastic_accelerated_gradient, PARAMS must have a NUMITERS field. Use optimparams to set automatically.');
else
	numIters = params.numIters;
end;

if (~isfield(params, 'L0')),
	error('When used with projected_stochastic_accelerated_gradient, PARAMS must have a L0 field. Use optimparams to set automatically.');
else
	L0 = params.L0;
end;

if (~isfield(params, 'b')),
	error('When used with projected_stochastic_accelerated_gradient, PARAMS must have a B field. Use optimparams to set automatically.');
else
	b = params.b;
end;

if (~isfield(params, 'store')),
	error('When used with projected_stochastic_accelerated_gradient, PARAMS must have a STORE field. Use optimparams to set automatically.');
else
	store = params.store;
end;

% if (~isfield(params, 'termCondition')),
% 	error('When used with projected_stochastic_accelerated_gradient, PARAMS must have a TERMCONDITION field. Use optimparams to set automatically.');
% else
% 	termCondition = params.termCondition;
% end;
% 
% if (termCondition > 0),
% 	if (~isfield(params, 'eps')),
% 		error('When TERMCONDITION > 0, PARAMS must have an EPS field. Use optimparams to set automatically.');
% 	else
% 		eps = params.eps;
% 	end;
% end;

if (~isfield(params, 'verbose')),
	error('When used with projected_stochastic_accelerated_gradient, PARAMS must have a VERBOSE field. Use optimparams to set automatically.');
else
	verbose = params.verbose;
end;

a = 0;
L = L0;
w = w0;
wavg = zeros(size(w));
z = zeros(size(w));
x = zeros(size(w));
g = zeros(size(w));

for iter = 1:numIters,
	if (verbose > 0),
		fprintf('Now running iter: %d\n.', iter);
	end;
	a = 2 / (iter + 2);
	L = b * (iter + 1) ^ (3 / 2) + L0;	% TODO: Maybe use linesearch to make
													% L > "Lipschitz"? Need to study 
													% proof to see how L > L0 is used.
	x = (1 - a) * w + a * z;
	[~, g] = obj_grad_func(x);
	w = proj_func(x - 1 / L * g);
	z = z - L / (L * a) * (x - w);
	wavg = ((iter - 1) * wavg + w) / iter;
	if (~isempty(store)),
		save(sprintf('%s_%d.mat', store, iter), 'w', 'wavg', 'g', 'eta', 'iter');
	end;
end;
