function [val deriv ims imAtoms] = inversion_obj_grad_dummy(w, imageSet, sceneSet, weightSet,...
														sigmaTD, sigmaSD, pfD, theta,...
														renderer, scheduler)

% TODO: Parallelize over both scene and atom.
[numAngles numAtoms] = size(pfD);
numScenes = length(sceneSet);
assert(length(theta) == numAngles);
assert(length(sigmaTD) == numAtoms);
assert(length(sigmaSD) == numAtoms);
assert(length(imageSet) == numScenes);
assert(length(weightSet) == numScenes);

[sigmaT sigmaS pf] = getMixtureMaterial(w, sigmaTD, sigmaSD, pfD);

c1 = clock;
if ((~exist('scheduler', 'var')) || (isempty(scheduler))),
	imAtoms = inversion_obj_grad_dummy_task_nonpar(theta, pf, sigmaT, sigmaS,...
													pfD, sigmaTD, sigmaSD, sceneSet, renderer);
else
	pjob = createMatlabPoolJob(scheduler);
	set(pjob, 'MinimumNumberOfWorkers', get(scheduler, 'ClusterSize'));
	set(pjob, 'MaximumNumberOfWorkers', get(scheduler, 'ClusterSize'));
	createTask(pjob, @inversion_obj_grad_dummy_task, 1, {theta, pf, sigmaT, sigmaS,...
													pfD, sigmaTD, sigmaSD, sceneSet, renderer});
	submit(pjob);
end;

ims = cell(numScenes, 1);
for iterScene = 1:numScenes,
	scene = sceneSet(iterScene);
	ims{iterScene} = renderImage_mex(theta, pf, sigmaT, sigmaS, ...
					scene.iorMedium, scene.mediumDimensions, scene.iorContainer, scene.containerDimensions, ...
					scene.rayOrigin, scene.rayDir, scene.rayRadius, scene.Li, ...
					scene.viewOrigin, scene.viewDir, scene.viewY, scene.viewPlane, scene.viewReso, ...
					renderer.hstep, renderer.numPhotons, renderer.useContinuous, renderer.useLF, renderer.useDirect, renderer.useHybrid)';
end;

if (exist('scheduler', 'var') && (~isempty(scheduler))),
	waitForState(pjob, 'finished');
	imAtoms = getAllOutputArguments(pjob);
	imAtoms = imAtoms{:};
end;
c2 = clock;

val = 0;
for iterScene = 1:numScenes,
	val = weightSet(iterScene) * sum(vec(ims{iterScene} - imageSet{iterScene}) .^ 2);
end;
deriv = zeros(numAtoms, 1);
for iterAtom = 1:numAtoms,
	for iterScene = 1:numScenes,
		val = weightSet(iterScene) * sum(vec(ims{iterScene} - imageSet{iterScene}) .^ 2);
		deriv(iterAtom) = deriv(iterAtom) + 2 * weightSet(iterScene)...
							* vec(ims{iterScene} - imageSet{iterScene})' * vec(imAtoms{iterScene, iterAtom});
	end;
end;
c3 = clock;
fprintf('Gradient rendering %g all %g\n', etime(c2, c1), etime(c3, c1));

end

%%
function imAtoms = inversion_obj_grad_dummy_task(theta, pf, sigmaT, sigmaS,...
													pfD, sigmaTD, sigmaSD, sceneSet, renderer)

[~, numAtoms] = size(pfD);
numScenes = length(sceneSet);
imAtoms = cell(numScenes, numAtoms);
parfor iterAtom = 1:numAtoms,
	imAtoms(:, iterAtom) = inversion_obj_grad_dummy_sub(theta, pf, sigmaT, sigmaS,...
									pfD(:, iterAtom), sigmaTD(iterAtom), sigmaSD(iterAtom),...
									sceneSet, renderer);
end;
												
end											

%%
function imAtoms = inversion_obj_grad_dummy_task_nonpar(theta, pf, sigmaT, sigmaS,...
													pfD, sigmaTD, sigmaSD, sceneSet, renderer)

[~, numAtoms] = size(pfD);
numScenes = length(sceneSet);
imAtoms = cell(numScenes, numAtoms);
for iterAtom = 1:numAtoms,
	imAtoms(:, iterAtom) = inversion_obj_grad_dummy_sub(theta, pf, sigmaT, sigmaS,...
									pfD(:, iterAtom), sigmaTD(iterAtom), sigmaSD(iterAtom),...
									sceneSet, renderer);
end;
												
end

%%
function imAtom = inversion_obj_grad_dummy_sub(theta, pf, sigmaT, sigmaS,...
															pfSingle, sigmaTSingle, sigmaSSingle,...
															sceneSet, renderer)
numScenes = length(sceneSet);
imAtom = cell(numScenes, 1);
for iterScene = 1:numScenes,
	scene = sceneSet(iterScene);
	imAtom{iterScene} = renderDerivImage_mex(theta, pf, sigmaT, sigmaS,...
									theta, pfSingle, sigmaTSingle, sigmaSSingle,...
				scene.iorMedium, scene.mediumDimensions, scene.iorContainer, scene.containerDimensions, ...
				scene.rayOrigin, scene.rayDir, scene.rayRadius, scene.Li, ...
				scene.viewOrigin, scene.viewDir, scene.viewY, scene.viewPlane, scene.viewReso, ...
				renderer.hstep, renderer.numPhotons, renderer.numPhotonsLF, renderer.useContinuous, renderer.useHybrid)';
end;

end
