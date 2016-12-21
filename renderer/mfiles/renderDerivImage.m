function im = renderDerivImage(sigmaT, sigmaS, func,...
										sigmaTSingle, sigmaSSingle, funcSingle, theta, ...
										scene, renderer)
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
numPhotonsLF = renderer.numPhotonsLF;
useContinuous = renderer.useContinuous;
useHybrid = renderer.useHybrid;

%%
im = renderDerivImage_mex(theta, func, sigmaT, sigmaS, theta, funcSingle, sigmaTSingle, sigmaSSingle, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, numPhotonsLF,	useContinuous, useHybrid)';
