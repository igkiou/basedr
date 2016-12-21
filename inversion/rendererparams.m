function params = rendererparams(varargin)
%% 

params.useContinuous = 0;	% always keep 0, except for debugging continuous deriv.
params.useHybrid = 1;			% always keep 1, except for simulating reference using f-diff approximation.
params.useLF = 0;				% always keep 0, deriv ignores it and uses it anyway.
params.useDirect = 0;			% always keep 0, except for geometric.
params.numPhotons = 500000;
params.numPhotonsLF = 500000;
params.hstep = 0.001;
	
% check for incorrect inputs
if (mod(length(varargin), 2) ~= 0)
	error('Invalid input arguments, type help sparseClassificationParams.');
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

