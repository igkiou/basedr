function ims = renderImageScenes(sigmaT, sigmaS, func, theta, sceneSet, rendererSet)

numScenes = length(sceneSet);
assert(length(rendererSet) == numScenes);
ims = cell(numScenes, 1);
parfor iterScene = 1:numScenes,
	ims{iterScene} = renderImage(sigmaT, sigmaS, func, theta, sceneSet(iterScene), rendererSet(iterScene));
end;
