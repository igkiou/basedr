function [sigmaT sigmaS pf] = getMixtureMaterial(w, sigmaTD, sigmaSD, pfD)
numAtoms = length(w);
assert(size(sigmaTD, 2) == numAtoms);
assert(size(sigmaSD, 2) == numAtoms);
assert(size(pfD, 2) == numAtoms);
assert(~anyv(w < 0));
assert(abs(sum(w) - 1) < 10 ^ - 6);

sigmaT = sigmaTD * w;
sigmaS = sigmaSD * w;
pf = pfD * (sigmaSD' .* w) / sigmaS;
