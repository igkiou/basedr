function params = sceneparams(varargin)
%% 
% All units are in mm.

params.iorMedium = 1.3;
params.mediumDimensions = [2.5; 34; 22];
% params.mediumDimensions = [2.5; 100; 100];

params.rayOrigin = [-params.mediumDimensions(1) / 2; 0.0; 0.0];
params.rayDir = normvec([1; -1; 0]);
params.rayRadius = 0.5;
params.Li = 75000.0;

params.viewOrigin = [0.0; 0.0];
params.viewDir = [-1.0; 0.0; 0.0];
params.viewY = [0.0; 0.0; -1.0];
params.viewPlane = [5; 5];
params.viewReso = [128; 128];
								
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

