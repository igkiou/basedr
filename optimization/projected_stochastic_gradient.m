function [wavg, w] = projected_stochastic_gradient(w0, params, obj_grad_func, proj_func)

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

if (~isfield(params, 'stepUpdate')),
	error('When used with projected_stochastic_gradient, PARAMS must have a STEPUPDATE field. Use optimparams to set automatically.');
else
	stepUpdate = params.stepUpdate;
end;

if (~isfield(params, 'store')),
	error('When used with projected_stochastic_gradient, PARAMS must have a STORE field. Use optimparams to set automatically.');
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

for iter = 1:numIters,
	if (verbose > 0),
		fprintf('Now running iter: %d\n.', iter);
	end;
	
	[~, g] = obj_grad_func(w);
	eta = update_eta(iter, eta0, stepUpdate);
	w = w - eta * g;
	w = proj_func(w);
	wavg = ((iter - 1) * wavg + w) / iter;
	if (~isempty(store)),
		save(sprintf('%s_%d.mat', store, iter), 'w', 'wavg', 'g', 'eta', 'iter');
	end;
end;
