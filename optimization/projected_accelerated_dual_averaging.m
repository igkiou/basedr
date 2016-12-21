function [wavg, w] = projected_accelerated_dual_averaging(w0, params, obj_grad_func, proj_func)

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
	error('When used with projected_stochastic_gradient, PARAMS must have a NUMITERS field. Use optimparams to set automatically.');
else
	numIters = params.numIters;
end;

if (~isfield(params, 'eta0')),
	error('When used with projected_stochastic_gradient, PARAMS must have an ETA0 field. Use optimparams to set automatically.');
else
	eta0 = params.eta0;
end;

if (~isfield(params, 'L0')),
	error('When used with projected_accelerated_dual_averaging, PARAMS must have a L0 field. Use optimparams to set automatically.');
else
	L0 = params.L0;
end;

if (~isfield(params, 'store')),
	error('When used with projected_accelerated_dual_averaging, PARAMS must have a STORE field. Use optimparams to set automatically.');
else
	store = params.store;
end;

% if (~isfield(params, 'termCondition')),
% 	error('When used with projected_stochastic_gradient, PARAMS must have a TERMCONDITION field. Use optimparams to set automatically.');
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
	error('When used with projected_stochastic_gradient, PARAMS must have a VERBOSE field. Use optimparams to set automatically.');
else
	verbose = params.verbose;
end;

w = w0;
v = w;
wavg = zeros(size(w));
g = zeros(size(w));
gbar = zeros(size(w));
At = 0;

for iter = 1:numIters,
	if (verbose > 0),
		fprintf('Now running iter: %d\n.', iter);
	end;
	
	% Here, I use the "optimal convergence" parameters from Xiao, JMLR
	% 2010a, page 32, eqs. (37)-(38).
	a = iter / 2;
	beta = (iter + 1) ^ (3 / 2) / 2 / eta0;
% 	At = At + at; thetat = at / At;
	At = iter * (iter + 1) / 4; thetat = 2 / (iter + 1);
	u = (1 - thetat) * w + thetat * v;
	[~, g] = obj_grad_func(u);
	gbar = (1 - thetat) * gbar + thetat * g;
	v = proj_func(- At / (L0 + beta) * gbar);
	w = (1 - thetat) * w + thetat * v;
	wavg = ((iter - 1) * wavg + w) / iter;
	if (~isempty(store)),
		save(sprintf('%s_%d.mat', store, iter), 'w', 'wavg', 'g', 'eta', 'iter');
	end;
end;
