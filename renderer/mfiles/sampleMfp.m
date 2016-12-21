function samples = sampleMfp(sigmaT, numSamples)

ksi = rand(numSamples, 1);
% samples = - log(1-ksi) / sigmaT;
samples = - log(ksi) / sigmaT;
