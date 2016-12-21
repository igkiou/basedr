function [refDir] = simSnell(n1, n2, viewDir)

refDir = zeros(3, 1);
refDir(2) = viewDir(2) / n2 * n1;
refDir(3) = viewDir(3) / n2 * n1;
refDir(1) = sqrt(1 - refDir(2) ^ 2 - refDir(3) ^ 2) * sign(viewDir(1));
