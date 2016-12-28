function ims = renderImageScenes(sigmaT, albedo, gVal, sceneSet, rendererSet)

numScenes = length(sceneSet);
assert(length(rendererSet) == numScenes);
ims = cell(numScenes, 1);
parfor iterScene = 1:numScenes,
	ims{iterScene} = renderImage(sigmaT, albedo, gVal, sceneSet(iterScene), rendererSet(iterScene));
end;