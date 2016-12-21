function plotPlane(normalVec, originVec, bounds)

if ((nargin < 2) || isempty(originVec)),
	originVec = [0; 0; 0];
end;

if ((nargin < 3) || isempty(bounds)),
	bounds = [10; 10; 10];
end;

%# a plane is a*x+b*y+c*z+d=0
%# [a,b,c] is the normal. Thus, we have to calculate
%# d and we're set
d = - originVec' * normalVec; %'# dot product for less typing

if (normalVec(3) ~= 0),
	% create x,y
	[xx, yy] = ndgrid((-bounds(1)+originVec(1)):0.5:(bounds(1)+originVec(1)), (-bounds(2)+originVec(2)):0.5:(bounds(2)+originVec(2)));

	% calculate corresponding z
	z = (- normalVec(1) * xx - normalVec(2) * yy - d) / normalVec(3);
else
	if (normalVec(2) ~= 0),
		% create x,y
		[xx, z] = ndgrid((-bounds(1)+originVec(1)):0.5:(bounds(1)+originVec(1)), (-bounds(3)+originVec(3)):0.5:(bounds(3)+originVec(3)));

		% calculate corresponding z
		yy = (- normalVec(1) * xx - d) / normalVec(2);
	else
		% create x,y
		[yy, z] = ndgrid((-bounds(2)+originVec(2)):0.5:(bounds(2)+originVec(2)), (-bounds(3)+originVec(3)):0.5:(bounds(3)+originVec(3)));

		% calculate corresponding z
		xx = -d / normalVec(1) * ones(size(yy));
	end;
end;
	
%# plot the surface
surf(xx, yy, z, 0.5 * ones(size(z)));
