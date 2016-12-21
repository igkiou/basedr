function [wavg, w] = projected_dual_averaging(w0, params, obj_grad_func, proj_func)

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
	error('When used with projected_dual_averaging, PARAMS must have a NUMITERS field. Use optimparams to set automatically.');
else
	numIters = params.numIters;
end;

if (~isfield(params, 'eta0')),
	error('When used with projected_dual_averaging, PARAMS must have an ETA0 field. Use optimparams to set automatically.');
else
	eta0 = params.eta0;
end;

if (~isfield(params, 'stepUpdate')),
	error('When used with projected_dual_averaging, PARAMS must have a STEPUPDATE field. Use optimparams to set automatically.');
else
	stepUpdate = params.stepUpdate;
end;

if (~isfield(params, 'store')),
	error('When used with projected_dual_averaging, PARAMS must have a STORE field. Use optimparams to set automatically.');
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

eta = eta0;
w = w0;
wavg = zeros(size(w));
g = zeros(size(w));
gbar = zeros(size(w));

for iter = 1:numIters,
	if (verbose > 0),
		fprintf('Now running iter: %d\n.', iter);
	end;
	
	[~, g] = obj_grad_func(w);
	gbar = ((iter - 1) * gbar + g) / iter;
	
	% Using the notation from Lin Xiao's JMLR10 paper, we have
	% eta = 1 / beta;
	% beta = gamma * sqrt(iter) => eta = eta0 / sqrt(iter), eta0 = 1 / gamma;
	% w = P(- t / beta * G) = P(- t * eta * G) = P(- sqrt(t) * eta0 * G);
	eta = update_eta(iter, eta0, stepUpdate);
	w = proj_func(- iter * eta * gbar);
	wavg = ((iter - 1) * wavg + w) / iter;
	if (~isempty(store)),
		save(sprintf('%s_%d.mat', store, iter), 'w', 'wavg', 'g', 'eta', 'iter');
	end;
end;
