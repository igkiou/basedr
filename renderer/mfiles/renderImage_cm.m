function im = renderImage_cm(sigmaVal, numPhotons)
%% 
% All units are in cm.

theta = 0:0.001:pi;
func = evaluateHG(theta, 0.9);
% func = zeros(size(theta)); func(1) = 1;
% [theta func] = readDensity('../samples/hg0p8.txt');
sigmaT = sigmaVal;
sigmaS = sigmaVal;
% sigmaS = 0;
% sigmaT = 100;
% sigmaS = 100;

iorMedium = 1.0;
% mediumDimensions = [2 * 1.25; 2 * 27.5; 2 * 27.5];
mediumDimensions = [0.25; 2.2; 3.4];

rayOrigin = [0.0; 0.0];
% rayOrigin = [1.82331752365019; 0.0];
% rayAngle = -0.3927;
% rayAngle = -0.7854;
% rayAngle = 0;
% rayDir = [cos(rayAngle); sin(rayAngle); 0];
% rayDir = ones(3, 1); rayDir = normvec(rayDir);
% rayDir = [1; -1; 0]; rayDir = normvec(rayDir);
rayDir = [1; 0; 0]; rayDir = normvec(rayDir);
% rayDir = randn(3,1); rayDir = normvec(rayDir);
[c1 c2] = getCoordFromNormal(rayDir);
refDir = vectorSnell(1, iorMedium, [1;0;0], rayDir)';
rayRadius = 0.05;
Li = 75000.0;

viewOrigin = [0.0; 0.0];
% viewDir = [-1.0; 0.0; 0.0];
% viewDirTemp = -[1; 1; 0]; viewDirTemp = normvec(viewDirTemp);
% viewYTemp = [0.0; 0.0; -1.0];
% viewXTemp = cross(viewYTemp, viewDirTemp);
viewDir = - rayDir;
% viewY = cross(viewDir, viewXTemp);
viewY = [0.0; 0.0; -1.0];
% viewY = [0.0; 1.0; 0.0];
viewPlane = [5; 5];
viewReso = [128; 128];
% viewReso = [1024; 1024];

% numPhotons = 5000000;
useContinuous = 1;

%%
im = renderImage_mex(theta, func, sigmaT, sigmaS, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, useContinuous)';
