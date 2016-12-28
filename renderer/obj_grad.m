function [val deriv ims imAtoms] = inversion_obj_grad_parallel(w, imageSet, sceneSet, weightSet,...
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
	imAtoms = inversion_obj_grad_parallel_task_nonpar(theta, pf, sigmaT, sigmaS,...
													pfD, sigmaTD, sigmaSD, sceneSet, renderer);
else
	pjob = createMatlabPoolJob(scheduler);
	set(pjob, 'MinimumNumberOfWorkers', get(scheduler, 'ClusterSize'));
	set(pjob, 'MaximumNumberOfWorkers', get(scheduler, 'ClusterSize'));
	createTask(pjob, @inversion_obj_grad_parallel_task, 1, {theta, pf, sigmaT, sigmaS,...
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
function imAtoms = inversion_obj_grad_parallel_task(theta, pf, sigmaT, sigmaS,...
													pfD, sigmaTD, sigmaSD, sceneSet, renderer)

[~, numAtoms] = size(pfD);
numScenes = length(sceneSet);
numIters = numScenes * numAtoms;
pfDExt = repmat(pfD, [1 1 numScenes]);
sigmaTDExt = repmat(sigmaTD, [1 1 numScenes]);
sigmaSDExt = repmat(sigmaSD, [1 1 numScenes]);
sceneSetExt = repmat(sceneSet, [1 1 numAtoms]);
sceneSetExt = permute(sceneSetExt, [3 2 1]);
imAtoms = cell(numScenes * numAtoms, 1);
rendererSet = repmat(renderer, [1 1 numIters]);
thetaSet = repmat(theta, [1 1 numIters]);
pfSet = repmat(pf, [1 1 numIters]);
sigmaTSet = repmat(sigmaT, [1 1 numIters]);
sigmaSSet = repmat(sigmaS, [1 1 numIters]);
parfor iter = 1:numIters,
	imAtoms{iter} = renderDerivImage_mex(...
		thetaSet(:, :, iter), pfSet(:, :, iter), sigmaTSet(:, :, iter), sigmaSSet(:, :, iter),...
		thetaSet(:, :, iter), pfDExt(:, iter), sigmaTDExt(iter), sigmaSDExt(iter),...
		sceneSetExt(iter).iorMedium, sceneSetExt(iter).mediumDimensions,...
		sceneSetExt(iter).iorContainer, sceneSetExt(iter).containerDimensions, ...
		sceneSetExt(iter).rayOrigin, sceneSetExt(iter).rayDir, sceneSetExt(iter).rayRadius, sceneSetExt(iter).Li, ...
		sceneSetExt(iter).viewOrigin, sceneSetExt(iter).viewDir, sceneSetExt(iter).viewY,...
		sceneSetExt(iter).viewPlane, sceneSetExt(iter).viewReso, ...
		rendererSet(:, :, iter).hstep, rendererSet(:, :, iter).numPhotons, rendererSet(:, :, iter).numPhotonsLF,...
		rendererSet(:, :, iter).useContinuous, rendererSet(:, :, iter).useHybrid)';
end;
imAtoms = reshape(imAtoms, [numAtoms, numScenes])';

end											

%%
function imAtoms = inversion_obj_grad_parallel_task_nonpar(theta, pf, sigmaT, sigmaS,...
													pfD, sigmaTD, sigmaSD, sceneSet, renderer)

[~, numAtoms] = size(pfD);
numScenes = length(sceneSet);
imAtoms = cell(numScenes, numAtoms);
for iterScene = 1:numScenes,
	for iterAtom = 1:numAtoms,
		imAtoms{iterScene, iterAtom} = renderDerivImage_mex(theta, pf, sigmaT, sigmaS,...
								theta, pfD(:, iterAtom), sigmaTD(iterAtom), sigmaSD(iterAtom),...
								sceneSet(iterScene).iorMedium, sceneSet(iterScene).mediumDimensions,...
								sceneSet(iterScene).iorContainer, sceneSet(iterScene).containerDimensions, ...
								sceneSet(iterScene).rayOrigin, sceneSet(iterScene).rayDir,...
								sceneSet(iterScene).rayRadius, sceneSet(iterScene).Li, ...
								sceneSet(iterScene).viewOrigin, sceneSet(iterScene).viewDir,...
								sceneSet(iterScene).viewY, sceneSet(iterScene).viewPlane, sceneSet(iterScene).viewReso, ...
								renderer.hstep, renderer.numPhotons, renderer.numPhotonsLF, renderer.useContinuous, renderer.useHybrid)';
	end;
end;
												
end
