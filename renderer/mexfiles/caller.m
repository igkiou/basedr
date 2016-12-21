%%
sigmaT = 10;
albedo = 0.99;
gVal = 0.5;

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

numPhotons = 50000000;
useDirect = 0;

%%
im = renderImage_mex(sigmaT, albedo, gVal, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						numPhotons, useDirect)';
