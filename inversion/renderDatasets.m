%%%% setup problem
%% cluster size
clusterSize = 120;
scheduler = createScheduler(clusterSize);
pjob = createJob(scheduler);

%% setups
%
mediumWidthSet = {1};

%
frontFlagSet = {[0; 0; 0; 0], [0; 0; 1; 1], [1; 1; 1; 1]};
rayAnglesSet = {[-5; -11.25; -22.5; -45], [-5; -22.5; 185; 202.5], [185; 191.25; 202.5; 225]};

%
rayRadiusSet = {1};
viewPlaneSet = {[5; 5]};
%

theta = 0:0.001:pi;
gVals = [-0.9 -0.8 -0.5 -0.3 0 0.3 0.5 0.8 0.9];
kVals = [-75 -25 -10 -5 -1 1 5 10 25 75];
pVals = [gVals kVals];
numGVals = length(gVals);
numKVals = length(kVals);
numAtoms = numGVals + numKVals;
pfD = zeros(length(theta), numAtoms);
for iterG = 1:numGVals,
	pfD(:, iterG) = evaluateHG(theta, gVals(iterG))';
end;
for iterK = 1:numKVals,
	pfD(:, numGVals + iterK) = evaluateVMF(theta, kVals(iterK))';
end;
sigmaTD = ones(1, numAtoms);
sigmaSD = ones(1, numAtoms);

wRealSet = [];
load cluster_centers.mat center_names
inds = [34 18 12 37 24];
names = center_names(inds);
names{end + 1} = 'lucy_d50_k10_g0_w1';
names{end + 1} = 'lucy_d50_k10_g0_w0.9';
names{end + 1} = 'lucy_d50_g0.9_g0_w1';
names{end + 1} = 'lucy_d50_g0.8_g0_w1';
params = parseFileName(names);
for iter = 1:length(names),
	wRealSet{iter} = zeros(length(pVals), 1); 
	wRealSet{iter}(find(pVals == params{4, iter})) = params{7, iter}; 
	wRealSet{iter}(find(pVals == params{6, iter})) = 1 - params{7, iter};
end;
w = abs(randn(length(wRealSet{1}), 1)); wRealSet{end + 1} = w / sum(w);

densitySet = {1 10 50};

%% %% make all
for iterMediumWidth = 1:length(mediumWidthSet),
	for iterFrontFlag = 1:length(frontFlagSet),
		for iterRayRadius = 1:length(rayRadiusSet),
			for iterWReal = 1:length(wRealSet),
				for iterDensity = 1:length(densitySet),

% for iterMediumWidth = 1:1,
% 	for iterFrontFlag = 1:1,
% 		for iterRayRadius = 1:1,
% 			for iterWReal = 1:1,
% 				for iterDensity = 1:1,
%% basic scene info
iorMedium = 1;
mediumDimensions = [mediumWidthSet{iterMediumWidth}; 100; 100];
iorContainer = 1;
containerDimensions = [1; 1; 1];

%% lighting directions
% frontFlag = 1 for frontlighting
frontFlag = frontFlagSet{iterFrontFlag};
rayAngles = rayAnglesSet{iterFrontFlag};
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
												'iorContainer', iorContainer, 'containerDimensions', containerDimensions,...
												'rayOrigin', rayOrigin, 'rayDir', rayDir, 'rayRadius', rayRadius, 'Li', Li,...
												'viewOrigin', viewOrigin, 'viewDir', viewDir, 'viewY', viewY,...
												'viewPlane', viewPlane, 'viewReso', viewReso);
	end;
end;
fprintf('Finished creating %d scenes.\n', numScenes);

%% real material
density = densitySet{iterDensity};
albedo = 0.99;
wReal = wRealSet{iterWReal};
[~, ~, pfReal] = getMixtureMaterial(wReal, sigmaTD, sigmaSD, pfD);
sigmaTReal = density;
sigmaSReal = density * albedo;

%% render reference images
useContinuous = 1;	% always keep 0, except for debugging continuous deriv.
useHybrid = 1;			% always keep 1, except for simulating reference using f-diff approximation.
useLF = 0;				% always keep 0, deriv ignores it and uses it anyway.
useDirect = 0;			% always keep 0, except for geometric.
hstep = 1.0 / sigmaTReal * 0.001;
numPhotons		= 100000000;
numPhotonsLF	= 100000000;
renderer_ref = rendererparams('useContinuous', useContinuous, 'useHybrid', useHybrid,....
								'useLF', useLF, 'useDirect', useDirect',...
								'hstep', hstep, 'numPhotons', numPhotons, 'numPhotonsLF', numPhotonsLF);
rendererSet_ref = repmat(renderer_ref, [numScenes, 1]);

fprintf('iter mediumWidth%d_frontFlag%d_rayRadius%d_wReal%d_density%d',...
iterMediumWidth, iterFrontFlag, iterRayRadius, iterWReal, iterDensity);
name = sprintf('mediumWidth%d_frontFlag%d_rayRadius%d_wReal%d_density%d',...
	iterMediumWidth, iterFrontFlag, iterRayRadius, iterWReal, iterDensity);
fprintf('Started rendering reference images.\n');
createTask(pjob, @largeRenderImageScenes, 0, {sigmaTReal, sigmaSReal, pfReal, theta, sceneSet, rendererSet_ref, name});
fprintf('Finished rendering reference images.\n');

%% %%
				end;
			end;
		end;
	end;
end;

%% submit
submit(pjob);
waitForState(pjob);
results = getAllOutputArguments(pjob);
errmsgs = get(pjob.Tasks, {'ErrorMessage'});
nonempty = ~cellfun(@isempty, errmsgs);
celldisp(errmsgs(nonempty));

destroy(pjob);
