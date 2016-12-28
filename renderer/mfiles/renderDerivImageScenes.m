function ims = renderDerivImageScenes(sigmaT, albedo, gVal, sceneSet, rendererSet)

numScenes = length(sceneSet);
assert(length(rendererSet) == numScenes);
ims = cell(numScenes, 4);
parfor iterScene = 1:numScenes,
	[ims{iterScene}{1}, ims{iterScene}{2}, ims{iterScene}{3}, ims{iterScene}{4}] = ...
		renderDerivImage(sigmaT, albedo, gVal, sceneSet(iterScene), rendererSet(iterScene));
end;
