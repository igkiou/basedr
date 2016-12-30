function [im, dSigmaT, dAlbedo, dGVal] = renderDerivImage(sigmaT, albedo, gVal, scene, renderer)
%% 
% All units are in mm.

% sample
iorMedium = scene.iorMedium;
mediumDimensions = scene.mediumDimensions;

% light source
rayOrigin = scene.rayOrigin;
rayDir = scene.rayDir;
rayRadius = scene.rayRadius;
Li = scene.Li;

% camera and image
viewOrigin = scene.viewOrigin;
viewDir = scene.viewDir;
viewY = scene.viewY;
viewPlane = scene.viewPlane;
pathlengthRange = scene.pathlengthRange;
viewReso = scene.viewReso;

% renderer
numPhotons = renderer.numPhotons;
maxDepth = renderer.maxDepth;
maxPathlength = renderer.maxPathlength;
useDirect = renderer.useDirect;

%%
[im, dSigmaT, dAlbedo, dGVal] = renderDerivImage_mex(sigmaT, albedo, gVal, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, pathlengthRange, viewReso, ...
						numPhotons, maxDepth, maxPathlength, useDirect);
im = permute(im, [2 1 3]);
dSigmaT = permute(dSigmaT, [2 1 3]);
dAlbedo = permute(dAlbedo, [2 1 3]);
dGVal = permute(dGVal, [2 1 3]);
