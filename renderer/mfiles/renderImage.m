function im = renderImage(sigmaT, sigmaS, func, theta, scene, renderer)
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
viewReso = scene.viewReso;

% renderer
hstep = renderer.hstep;
numPhotons = renderer.numPhotons;
useContinuous = renderer.useContinuous;
useLF = renderer.useLF;
useDirect = renderer.useDirect;
useHybrid = renderer.useHybrid;

%%
im = renderImage_mex(theta, func, sigmaT, sigmaS, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, useContinuous, useLF, useDirect, useHybrid)';
