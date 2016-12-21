function im = renderImage_mm(sigmaVal, numPhotons)

theta = 0:0.001:pi;
func = evaluateHG(theta, 0.8);
% func = zeros(size(theta)); func(1) = 1;
% [theta func] = readDensity('../samples/hg0p8.txt');
sigmaT = sigmaVal;
sigmaS = sigmaVal;
% sigmaS = 0;
% sigmaT = 10; These are 100 in cm version.
% sigmaS = 10; 

iorMedium = 1.2;
% mediumDimensions = [2 * 1.25, 2 * 27.5, 2 * 27.5];
mediumDimensions = [2.5, 22, 34];

rayOrigin = [0.0 0.0];
% rayAngle = -0.3927;
% rayAngle = -0.7854;
rayAngle = 0;
rayDir = [cos(rayAngle); sin(rayAngle); 0];
% rayDir = ones(3, 1); rayDir = normvec(rayDir);
% rayDir = randn(3,1); rayDir = normvec(rayDir);
[c1 c2] = getCoordFromNormal(rayDir)
refDir = vectorSnell(1, iorMedium, [1;0;0], rayDir)'
rayRadius = 0.5;
Li = 75000.0;

viewOrigin = [0.0, 0.0];

% viewDir = [1.0, 1.0, -0.5];
% viewY = [0.5, 0.5, 2.0];
viewDir = [-1.0, 0.0, 0.0];
viewY = [0.0, 0.0, -1.0];
% viewY = [0.0, 1.0, 0.0];
viewPlane = [50, 50];
viewReso = [128, 128];

% numPhotons = 5000000;
useContinuous = 1;

%%
im = renderImage_mex(theta, func, sigmaT, sigmaS, ...
						iorMedium, mediumDimensions,
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, useContinuous)';
