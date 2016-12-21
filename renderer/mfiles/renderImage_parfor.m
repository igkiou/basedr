function ims = renderImage_parfor(numIters, numWorkers)

ims = cell(numIters * numWorkers, 1);
parfor iter = 1:(numIters * numWorkers),
	ims{iter} = renderImage;
end;
