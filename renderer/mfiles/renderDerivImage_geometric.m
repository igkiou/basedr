function im = renderDerivImage_geometric(sigmaVal, sigmaValSingle, numPhotons)
%% 
% All units are in mm.

theta = 0:0.001:pi;

func = evaluateHG(theta, 0.3);
% func = zeros(size(theta)); func(1) = 1;
% [theta func] = readDensity('../samples/hg0p8.txt');
sigmaT = sigmaVal;
sigmaS = sigmaVal;

funcSingle = evaluateHG(theta, 0);
% func = zeros(size(theta)); func(1) = 1;
% [theta func] = readDensity('../samples/hg0p8.txt');
sigmaTSingle = sigmaValSingle;
sigmaSSingle = sigmaValSingle;
% sigmaS = 0;
% sigmaT = 10; These are 100 in cm version.
% sigmaS = 10; 

iorMedium = 1.3;
mediumDimensions = [2.5; 34; 22];
% mediumDimensions = [2.5; 100; 100];

rayOrigin = [-mediumDimensions(1) / 2; 0.0; 0.0];
% rayOrigin = [1.82331752365019; 0.0];
rayAngle = -0.3927;
% rayAngle = -0.7854;
% rayAngle = 0;
rayDir = [cos(rayAngle); sin(rayAngle); 0];
% rayDir = ones(3, 1); rayDir = normvec(rayDir);
% rayDir = [1; -1; 0]; rayDir = normvec(rayDir);
% rayDir = [1; 0; 0]; rayDir = normvec(rayDir);
% rayDir = randn(3,1); rayDir = normvec(rayDir);
rayRadius = 0.5;
Li = 75000.0;

viewOrigin = [0.0; 0.0];
viewDir = [-1.0; 0.0; 0.0];
% viewDirTemp = -[1; 1; 0]; viewDirTemp = normvec(viewDirTemp);
% viewYTemp = [0.0; 0.0; -1.0];
% viewXTemp = cross(viewYTemp, viewDirTemp);
% viewDir = - rayDir;
% viewY = cross(viewDir, viewXTemp);
viewY = [0.0; 0.0; -1.0];
% viewY = [0.0; 1.0; 0.0];
viewPlane = [5; 5];
viewReso = [128; 128];
% viewReso = [1024; 1024];

% numPhotons = 5000000;
numPhotonsLF = numPhotons;
useContinuous = 1;
useHybrid = 1;

%%
im = renderDerivImage_mex(theta, func, sigmaT, sigmaS, theta, funcSingle, sigmaTSingle, sigmaSSingle, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, numPhotonsLF, useContinuous, useHybrid)';
