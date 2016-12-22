%%%% setup problem

%% setups
%
mediumWidthSet = {10};

%
frontFlagSet = {[0; 0; 0; 0], [0; 0; 1; 1], [1; 1; 1; 1]};
rayAnglesSet = {[-5; -11.25; -22.5; -45], [-5; -22.5; 185; 202.5], [185; 191.25; 202.5; 225]};
rayRadiusSet = {0.5};
viewPlaneSet = {[5; 5]};
% frontFlagSet = {0};
% rayAnglesSet = {-22.5};
% rayRadiusSet = {0.5};
% viewPlaneSet = {[5; 5]};

%
gValSet = {0 0.3 0.5};
densitySet = {1 5 10};
albedoSet = {0.9 0.95 0.99};

%% %% make all
for iterMediumWidth = 1:length(mediumWidthSet),
	for iterFrontFlag = 1:length(frontFlagSet),
		for iterRayRadius = 1:length(rayRadiusSet),
			for iterGVal = 1:length(gValSet),
				for iterAlbedo = 1:length(albedoSet),
					for iterDensity = 1:length(densitySet),

%% basic scene info
iorMedium = 1;
mediumDimensions = [mediumWidthSet{iterMediumWidth}; 100; 100];

%% lighting directions
% frontFlag = 1 for frontlighting
frontFlag = frontFlagSet{iterFrontFlag};
rayAngles = deg2rad(rayAnglesSet{iterFrontFlag});
numRayAngles = length(rayAngles);
assert(length(frontFlag) == numRayAngles);
rayRadius = rayRadiusSet{iterRayRadius};
Li = 75000.0;

%% viewing directions
% viewAngles = deg2rad([0; -10; -20]);
viewAngles = deg2rad(0);
numViewAngles = length(viewAngles);
viewOrigin = [0.0; 0.0];

%% image params

viewPlane = viewPlaneSet{iterRayRadius};
viewReso = [64; 64];

%% create scenes
numScenes = numViewAngles * numRayAngles;
sceneSet = repmat(sceneparams, [numScenes, 1]);
iterScene = 0;
fprintf('Started creating scenes.\n');
for iterViewAngle = 1:numViewAngles,
	viewDir = -[cos(viewAngles(iterViewAngle)); sin(viewAngles(iterViewAngle)); 0];
	viewY = [0.0; 0.0; -1.0];
	for iterRayAngle = 1:numRayAngles,
		rayDir = [cos(rayAngles(iterRayAngle)); sin(rayAngles(iterRayAngle)); 0];
		if (frontFlag(iterRayAngle) == 0),
			rayOrigin = [-mediumDimensions(1) / 2; 0.0; 0.0];
		else
			rayOrigin = [mediumDimensions(1) / 2; 0.0; 0.0];
		end;
		iterScene = iterScene + 1;
		sceneSet(iterScene) = sceneparams('iorMedium', iorMedium, 'mediumDimensions', mediumDimensions,...
										'rayOrigin', rayOrigin, 'rayDir', rayDir, 'rayRadius', rayRadius, 'Li', Li,...
										'viewOrigin', viewOrigin, 'viewDir', viewDir, 'viewY', viewY,...
										'viewPlane', viewPlane, 'viewReso', viewReso);
	end;
end;
fprintf('Finished creating %d scenes.\n', numScenes);

%% real material
density = densitySet{iterDensity};
albedo = albedoSet{iterAlbedo};
gVal = gValSet{iterGVal};

%% render reference images
useDirect = 0;			% always keep 0, except for geometric.
% numPhotons = 100000000;
numPhotons = 10000000;
renderer_ref = rendererparams('useDirect', useDirect', 'numPhotons', numPhotons);
rendererSet_ref = repmat(renderer_ref, [numScenes, 1]);

fprintf('iter mediumWidth%d_frontFlag%d_rayRadius%d_density%d_albedo%d_g%d',...
iterMediumWidth, iterFrontFlag, iterRayRadius, iterDensity, iterAlbedo, iterGVal);
name = sprintf('mediumWidth%d_frontFlag%d_rayRadius%d_density%d_albedo%d_g%d',...
	iterMediumWidth, iterFrontFlag, iterRayRadius, iterDensity, iterAlbedo, iterGVal);
largeRenderImageScenes(density, albedo, gVal, sceneSet, rendererSet_ref, name);
fprintf('Finished rendering reference images.\n');

%% %%
					end;
				end;
			end;
		end;
	end;
end;