function [im imd] = renderImage_geometric(sigmaVal, numPhotons)
%% 
% All units are in mm.

theta = 0:0.001:pi;
func = evaluateHG(theta, 0);
% func = zeros(size(theta)); func(1) = 1;
% [theta func] = readDensity('../samples/hg0p8.txt');
sigmaT = sigmaVal;
sigmaS = sigmaVal;
% sigmaS = 0;
% sigmaT = 10; These are 100 in cm version.
% sigmaS = 10; 

iorMedium = 1.3;
mediumDimensions = [2.5; 34; 22];
% mediumDimensions = [1; 10; 42.5];

rayOrigin = [-mediumDimensions(1) / 2; 0.0; 0.0];
% rayOrigin = [1.82331752365019; 0.0];
% rayAngle = -0.3927;
% rayAngle = -0.7854;
% rayAngle = -deg2rad(30);
% rayAngle = 0;
% rayDir = [cos(rayAngle); sin(rayAngle); 0];
% rayDir = ones(3, 1); rayDir = normvec(rayDir);
rayDir = [1; -1; 0]; rayDir = normvec(rayDir);
% rayDir = [1; 0; 0]; rayDir = normvec(rayDir);
% rayDir = randn(3,1); rayDir = normvec(rayDir);
[c1 c2] = getCoordFromNormal(rayDir);
[refDir fresnelTransSource] = vectorSnell(1, iorMedium, [1; 0; 0], rayDir);
[foo fresnelTrans2] = vectorSnell(iorMedium, 1, [1; 0; 0], refDir); fresnelTrans = fresnelTransSource * fresnelTrans2;
[pos dirMed medDist] = refrShift(1, iorMedium, mediumDimensions(1), rayDir, rayOrigin);
rayRadius = 0.01;
Li = 75000.0;
% [refDir', fresnelTrans, fresnelTrans, fresnelTransFull]

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
useLF = 0;
useDirect = 0;
useHybrid = 1;

%%
im = renderImage_mex(theta, func, sigmaT, sigmaS, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, useContinuous, useLF, useDirect, useHybrid)';

if (nargout > 1),
	imb = renderImage_mex(theta, func, 0, 0, ...
							iorMedium, mediumDimensions, ...
							rayOrigin, rayDir, rayRadius, Li, ...
							viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
							hstep, numPhotons, useContinuous, useLF, 1, useHybrid)';

	imd = (imb > 0) * Li * fresnelTrans * exp(- medDist * sigmaVal);
end;
