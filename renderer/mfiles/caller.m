%%%%%%%%%%%%%%%%%%
%%%% setup problem

% all units are in mm
% see 'sceneparams' and 'rendererparams' for descriptions of the various
% parameters

%% scattering medium

% simulated medium
sigmaT = 1;
albedo = 0.9;
gVal = 0.8;

% sampling medium (used for path sampling)
samplingSigmaT = sigmaT;
samplingAlbedo = albedo;
samplingGVal = gVal;
% samplingSigmaT = 1;
% samplingAlbedo = 0.95;
% samplingGVal = 0.5;

%% basic scene info
iorMedium = 1;
mediumDimensions = [2.5; 100; 100];

%% lighting directions

% frontFlagSet = {[0; 0; 0; 0], [0; 0; 1; 1], [1; 1; 1; 1]};
% rayAnglesSet = {[-5; -11.25; -22.5; -45], [-5; -22.5; 185; 202.5], [185; 191.25; 202.5; 225]};

% frontFlag = 1 for frontlighting
% frontFlag = 0;
% rayAngle = deg2rad(-45);
frontFlag = 1;
rayAngle = deg2rad(225);
rayRadius = 0.1;
Li = 75000.0;

%% viewing directions
% viewAngles = deg2rad([0; -10; -20]);
viewAngle = deg2rad(0);
viewOrigin = [0.0; 0.0];

%% renderer options
numPhotons = 1000000000;
maxDepth = -1;
maxPathlength = -1;

%% image params

viewPlane = [50; 50];
pathlengthRange = [-1; -1];
viewReso = [128; 128; 1];
% pathlengthRange = [0; 100];
% viewReso = [128; 128; 128];

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

%% do rendering
% im and im_alt should be numerically identical;
% im and im_altw should be the same up to noise, and identical if the
%		simulated and sampling mediums are the same;
% dX and dXw should be the same up to noise, and identical if the
%		simulated and sampling mediums are the same;

% render an image by importance sampling the simulated medium
imss = renderImage(sigmaT, albedo, gVal, scene, renderer);

% render an image and derivatives by importance sampling the simulated medium
[im_altss, dSigmaTss, dAlbedoss, dGValss] = renderDerivImage(sigmaT, albedo, gVal, scene, renderer);

% render an image and derivatives by importance sampling the alternative sampling medium
[im_altwss, dSigmaTwss, dAlbedowss, dGValwss] = renderDerivImageWeight(sigmaT, albedo, gVal,...
										samplingSigmaT, samplingAlbedo, samplingGVal, scene, renderer);