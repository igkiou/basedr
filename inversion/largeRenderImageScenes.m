function imSet = largeRenderImageScenes(sigmaTReal, albedoReal, gValReal, sceneSet, rendererSet_ref, name)

numScenes = length(sceneSet);
assert(length(rendererSet_ref) == numScenes);
imSet = cell(numScenes, 1);
for iterScene = 1:numScenes,
	imSet{iterScene} = renderImage(sigmaTReal, albedoReal, gValReal, sceneSet(iterScene), rendererSet_ref(iterScene));
end;
size(imSet);
eval(sprintf('save %s.mat sceneSet sigmaTReal albedoReal gValReal rendererSet_ref imSet', name));
