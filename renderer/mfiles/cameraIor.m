function [refX, refY, refZ] = cameraIor(ior, viewDir)

refX = zeros(1, 3);
refY = zeros(1, 3);
refZ = zeros(1, 3);

if (abs(viewDir(1)) > 0),
	refX(2) = - viewDir(2) / ior;
	refX(3) = - viewDir(3) / ior;
	refX(1) = sqrt(1 - refX(2)^2 - refX(3)^2);
	if (viewDir(1) > 0)
		refX(1) = - refX(1);
	end;
end;

if (abs(viewDir(2)) > 0),
	refY(3) = - viewDir(3) / ior;
	refY(1) = - viewDir(1) / ior;
	refY(2) = sqrt(1 - refY(3)^2 - refY(1)^2);
	if (viewDir(2) > 0)
		refY(2) = - refY(2);
	end;
end;

if (abs(viewDir(3)) > 0),
	refZ(1) = - viewDir(1) / ior;
	refZ(2) = - viewDir(2) / ior;
	refZ(3) = sqrt(1 - refZ(1)^2 - refZ(2)^2);
	if (viewDir(3) > 0)
		refZ(3) = - refZ(3);
	end;
end;
