%%%% setup problem
%% cluster size
clusterSize = 10;
test_simulation_flag = 0;
store = 'fifth';

%% basic scene info
iorMedium = 1;
mediumDimensions = [1; 100; 100];
iorContainer = 1;
containerDimensions = [2; 16; 16];

%% lighting directions
% frontFlag = 1 for frontlighting
frontFlag = [0; 0; 0; 0;];
% frontFlag = 0;
rayAngles = deg2rad([-5; -11.25; -22.5; -45]);
% rayAngles = deg2rad(-5);
numRayAngles = length(rayAngles);
assert(length(frontFlag) == numRayAngles);
rayRadius = 0.01;
Li = 75000.0;

%% viewing directions
% viewAngles = deg2rad([0; -10; -20]);
viewAngles = deg2rad(0);
numViewAngles = length(viewAngles);
viewOrigin = [0.0; 0.0];

%% image params
viewPlane = [1; 1];
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

%% dictionary
density = 1;
albedo = 0.99;
theta = 0:0.001:pi;
gVals = [-0.95 -0.9 -0.8 -0.5 -0.3];
kVals = [-75 -50 -25 -10];
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
sigmaTD = density * ones(1, numAtoms);
sigmaSD = albedo * density * ones(1, numAtoms);

%% real material
wReal = abs(randn(numAtoms, 1));
% wReal = ones(numAtoms, 1);
wReal = wReal / sum(wReal);
[sigmaTReal sigmaSReal pfReal] = getMixtureMaterial(wReal, sigmaTD, sigmaSD, pfD);

%% render reference images
useContinuous = 1;	% always keep 0, except for debugging continuous deriv.
useHybrid = 1;			% always keep 1, except for simulating reference using f-diff approximation.
useLF = 0;				% always keep 0, deriv ignores it and uses it anyway.
useDirect = 0;			% always keep 0, except for geometric.
hstep = 1.0 / sigmaTReal * 0.001;
numPhotons		= 1000;
numPhotonsLF	= 1000;
renderer_ref = rendererparams('useContinuous', useContinuous, 'useHybrid', useHybrid,....
								'useLF', useLF, 'useDirect', useDirect',...
								'hstep', hstep, 'numPhotons', numPhotons, 'numPhotonsLF', numPhotonsLF);
rendererSet_ref = repmat(renderer_ref, [numScenes, 1]);

fprintf('Started rendering reference images.\n');
ref_start = clock;
if (clusterSize > 1),
	scheduler = createScheduler(clusterSize);
	pjob = createMatlabPoolJob(scheduler);
	set(pjob, 'MinimumNumberOfWorkers', get(scheduler, 'ClusterSize'));
	set(pjob, 'MaximumNumberOfWorkers', get(scheduler, 'ClusterSize'));
	createTask(pjob, @renderImageScenes, 1, {sigmaTReal, sigmaSReal, pfReal, theta, sceneSet, rendererSet_ref});
	
	submit(pjob);
	waitForState(pjob, 'finished');
	imSet = getAllOutputArguments(pjob);
	imSet = imSet{:};
else
	scheduler = [];
	imSet = renderImageScenes(sigmaTReal, sigmaSReal, pfReal, theta, sceneSet, rendererSet_ref);
end;
ref_end = clock;
fprintf('Finished rendering reference images.\n');
%% create weights
% weightSet = ones(numScenes, 1) / 64 / 64 / numScenes;
for iterScene = 1:numScenes,
	weightSet(iterScene) = 1 / sum(vec(imSet{iterScene}) .^ 2) / numScenes;  
end;

eval(sprintf('save %s_init.mat', store));
%% start optimization

useContinuous = 1;	% always keep 0, except for debugging continuous deriv.
useHybrid = 1;			% always keep 1, except for simulating reference using f-diff approximation.
useLF = 0;				% always keep 0, deriv ignores it and uses it anyway.
useDirect = 0;			% always keep 0, except for geometric.
hstep = 1.0 / sigmaTReal * 0.01;
numPhotons		= 10000000;
numPhotonsLF	= 10000000;
renderer = rendererparams('useContinuous', useContinuous, 'useHybrid', useHybrid,....
								'useLF', useLF, 'useDirect', useDirect',...
								'hstep', hstep, 'numPhotons', numPhotons, 'numPhotonsLF', numPhotonsLF);

if (test_simulation_flag == 1),
	numIters = 3;
	verbose = 1;
	solver = optimparams('numIters', numIters, 'store', store, 'verbose', verbose);

	obj_grad_func = @(w)inversion_obj_grad_dummy(w, imSet, sceneSet, weightSet, sigmaTD, sigmaSD, pfD, theta, renderer, scheduler);
	proj_func = @(w)simplex_projection_vector(w, 1);
	wInit = ones(numAtoms, 1) / numAtoms;

	fprintf('Started optimization.\n');
% 	[wAvg wLast] = projected_stochastic_gradient(wInit, solver, obj_grad_func, proj_func); 
% 	[wAvg wLast] = projected_dual_averaging(wInit, solver, obj_grad_func, proj_func); 
% 	[wAvg wLast] = exponentiated_gradient(wInit, solver, obj_grad_func, 1); 
% 	[wAvg wLast] = exponentiated_dual_averaging(wInit, solver, obj_grad_func, 1); 
% 	[wAvg wLast] = projected_accelerated_dual_averaging(wInit, solver, obj_grad_func, proj_func); 
	[wAvg wLast] = projected_stochastic_accelerated_gradient(wInit, solver, obj_grad_func, proj_func); 
	fprintf('SGD %g %g %g %g\n', norm(wAvg - wReal) / norm(wReal),...
										norm(wLast - wReal) / norm(wReal),...
										maxv(abs(wAvg - wReal) ./ abs(wReal)),...
										maxv(abs(wLast - wReal) ./ abs(wReal)));
	fprintf('Finished optimization.\n');
else
fprintf('Started gradients.\n');
	grad_start = clock;
	numIters = 3;
	for iter = 1:numIters
		fprintf('Gradient iter %d.\n', iter);
	% 	w = abs(randn(numAtoms, 1));
		w = (1:numAtoms)';
		w = w / sum(w);
	% 	[~, deriv{iter}, ims{iter} imAtoms{iter}] = inversion_obj_grad_parallel(w, imSet, sceneSet, weightSet,...
	% 												sigmaTD, sigmaSD, pfD, theta,...
	% 												renderer, scheduler);
		[~, deriv{iter}, ims{iter} imAtoms{iter}] = inversion_obj_grad_dummy(w, imSet, sceneSet, weightSet,...
													sigmaTD, sigmaSD, pfD, theta,...
													renderer, scheduler);
	end;
	grad_end = clock;
	fprintf('Finished gradients.\n');
end;
