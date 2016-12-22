%%
sigmaT = 10;
albedo = 0.99;
gVal = 0.5;

iorMedium = 1;
% mediumDimensions = [2 * 1.25, 2 * 27.5, 2 * 27.5];
mediumDimensions = [10, 10, 10];

frontLighting = 0;
rayAngle = 0;
rayOriginOrig = [0.0; 0.0];
if (frontLighting == 0) ,
	rayOrigin = [-mediumDimensions(1) / 2; rayOriginOrig];
else
	rayOrigin = [mediumDimensions(1) / 2; rayOriginOrig];
	rayAngle = rayAngle + 180;
end;
% rayAngle = -0.3927;
% rayAngle = -0.7854;
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

numPhotons = 500000;
useDirect = 0;

%%
im = renderImage_mex(sigmaT, albedo, gVal, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						numPhotons, useDirect)';
