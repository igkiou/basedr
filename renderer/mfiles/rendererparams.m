function params = rendererparams(varargin)
%% 

%% photon termination criteria
% max number of bounces (-1 is equivalent to infinite number of bounces)
params.maxDepth = -1;
% max pathlength (-1 is equivalent to infinite number of bounces)
params.maxPathlength = -1;

%% estimate direct component (currently disabled)
params.useDirect = 0;			% always keep 0, except for geometric.

%% number of photons to use
params.numPhotons = 500000;
	
% check for incorrect inputs
if (mod(length(varargin), 2) ~= 0)
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

