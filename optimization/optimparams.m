function params = optimparams(varargin)

params.numIters = 100;	% can be Inf, in which case must have termCondition ~= 0.
params.eta0 = 1;
params.stepUpdate = 1;	% options: 
								% 0 = constant;
								% 1 = inverse sqrt of iter;
								% 2 = inverse of iter.
								% 3 = line search.
params.termCondition = 0;	% options: 
									% 0 = none (use num of iters);
									% 1 = change in gradient;
									% 2 = relative change in gradient;
									% 3 = change in objective function value.
params.eps = 10 ^ - 6;	% to be used with option above.


params.verbose = 0;	% verbosity setting:
							% 0 = print nothing;
							% 1 = print num of iters;
							% 2 = print num of iters, decrease in obj. value, and
							% change in gradient.
params.store = [];	% storage setting:
						% [] = save nothing;
						% name = save intermediate weights in format name_iter;
params.L0 = 100;	% parameter used for SAGE. Should be set large enough.
						% TODO: Use number of pixels and max intensity to set
						% heuristically.
params.b = 1;		% parameter used for SAGE. Should be robust.
								
% check for incorrect inputs
if (mod(length(varargin),2) ~= 0)
	error('Invalid input arguments.');
end;

fieldsList = fieldnames(params);
fieldsSet = zeros(length(fieldsList));

% check for invalid inputs
for argCount = 1:2:length(varargin)
	if(~isfield(params, varargin{argCount}))
		error('Invalid field "%s".', varargin{argCount});
	else
		fieldIndex = find(strcmp(fieldsList, varargin{argCount}));
		if (fieldsSet(fieldIndex) == 1),
			warning('Field "%s" already set by previous argument.', fieldsList{fieldIndex});
		else
			fieldsSet(fieldIndex) = 1;
		end;
		eval(sprintf('params.%s = varargin{argCount + 1};',varargin{argCount}));
	end;
end;
