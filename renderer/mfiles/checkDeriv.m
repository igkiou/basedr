%%%%%%%%%%%%%%%%%%
%%%% setup problem

%% scattering medium

% simulated medium
% sigmaT = 10;
% albedo = 0.9;
% gVal = 0.3;
sigmaT = 1;
albedo = 0.9;
gVal = 0.8;

% sampling medium (used for path sampling)
samplingSigmaT = sigmaT;
samplingAlbedo = albedo;
samplingGVal = gVal;
% samplingSigmaT = 10;
% samplingAlbedo = 0.95;
% samplingGVal = 0.5;

%% basic scene info
iorMedium = 1;
mediumDimensions = [2.5; 100; 100];

%% lighting directions

% frontFlag = 1 for frontlighting
frontFlag = 0;
rayAngle = deg2rad(-45);
% frontFlag = 1;
% rayAngle = deg2rad(225);
rayRadius = 0.1;
Li = 75000.0;

%% viewing directions
% viewAngles = deg2rad([0; -10; -20]);
viewAngle = deg2rad(0);
viewOrigin = [0.0; 0.0];

%% renderer options
% numPhotons = 100000000;
numPhotons = 100000000;
maxDepth = -1;
maxPathlength = -1;

%% image params

viewPlane = [50; 50];
pathlengthRange = [0; 20];
viewReso = [128; 128; 128];

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% do not edit below here
%% create scene params
viewDir = -[cos(viewAngle); sin(viewAngle); 0];
viewY = [0.0; 0.0; -1.0];
		
rayDir = [cos(rayAngle); sin(rayAngle); 0];
if (frontFlag == 0),
	rayOrigin = [-mediumDimensions(1) / 2; 0.0; 0.0];
else
	rayOrigin = [mediumDimensions(1) / 2; 0.0; 0.0];
end;
scene = sceneparams('iorMedium', iorMedium, 'mediumDimensions', mediumDimensions,...
								'rayOrigin', rayOrigin, 'rayDir', rayDir, 'rayRadius', rayRadius, 'Li', Li,...
								'viewOrigin', viewOrigin, 'viewDir', viewDir, 'viewY', viewY,...
								'viewPlane', viewPlane, 'pathlengthRange', pathlengthRange, 'viewReso', viewReso);

%% create renderer params
useDirect = 0;			% always keep 0, except for geometric.
renderer = rendererparams('useDirect', useDirect', 'numPhotons', numPhotons,...
						'maxDepth', maxDepth, 'maxPathlength', maxPathlength);

%% check derivatives

materialParams = [sigmaT; albedo; gVal];
samplingMaterialParams = [samplingSigmaT; samplingAlbedo; samplingGVal];
e = 10^(-6);

[d dy dh] = checkgrad('obj_grad', materialParams, e, samplingMaterialParams,...
														scene, renderer)