function params = sceneparams(varargin)
%% 
% All units are in mm.

%% cube medium
% index of refraction of the medium
params.iorMedium = 1.3;
% dimensions of the medium
params.mediumDimensions = [2.5; 34; 22];

%% pointer source
% position of center of source
params.rayOrigin = [-params.mediumDimensions(1) / 2; 0.0; 0.0];
% direction of source
params.rayDir = normvec([1; -1; 0]);
% radius of source
params.rayRadius = 0.5;
% "intensity" of source
params.Li = 75000.0;

%% orthographic camera
% center of camera plane
params.viewOrigin = [0.0; 0.0];
% direction of camera
params.viewDir = [-1.0; 0.0; 0.0];
% orientation of camera plane
params.viewY = [0.0; 0.0; -1.0];
% size of camera plane
params.viewPlane = [5; 5];
% pathlength range of camera plane (leave [-1 -1] to measure all depths)
params.pathlengthRange = [-1; -1];
% x-y-z resolution of camera (setting z > 1 generates a transient image)
params.viewReso = [128; 128; 1];
								
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

