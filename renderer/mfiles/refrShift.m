function [pos dir medDist] = refrShift(n1, n2, width, rayDir, rayOrigin)

if (nargin < 5),
	rayOrigin = [0; 0; 0];
end;

refDir = vectorSnell(n1, n2, [1; 0; 0], rayDir);
dist = width / refDir(1);
pos = rayOrigin + dist * refDir;
dir = refDir;
medDist = norm(pos - rayOrigin);
