function largeRenderImageScenes(sigmaTReal, sigmaSReal, pfReal, theta, sceneSet, rendererSet_ref, name)

numScenes = length(sceneSet);
assert(length(rendererSet_ref) == numScenes);
imSet = cell(numScenes, 1);
for iterScene = 1:numScenes,
	imSet{iterScene} = renderImage(sigmaTReal, sigmaSReal, pfReal, theta, sceneSet(iterScene), rendererSet_ref(iterScene));
end;
size(imSet);
eval(sprintf('save %s.mat sceneSet sigmaTReal sigmaSReal pfReal rendererSet_ref imSet', name));
