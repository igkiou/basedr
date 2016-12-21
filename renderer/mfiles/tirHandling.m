function [p d] = tirHandling(p, d, dist, mediumDimensions, ior)

blockL = [-mediumDimensions(1) / 2, -mediumDimensions(2) / 2, -mediumDimensions(3) / 2];
blockR = [mediumDimensions(1) / 2, mediumDimensions(2) / 2, mediumDimensions(3) / 2];

p1 = p + dist * d;

cIn = zeros(3,1);
[isIn cIn(1) cIn(2) cIn(3)] = inside(p1, blockL, blockR);
while(~isIn),
	i = 0;
	while (i < 3 && cIn(i + 1)),
		i = i + 1;
	end;
	i
	dis1 = (blockR(i + 1) - p(i + 1)) / d(i + 1);
	dis2 = (blockL(i + 1) - p(i + 1)) / d(i + 1);
	if (~(dis1 * dis2 < -eps))
		return
	end;
	if (dis1 > 0),
		dis = dis1;
	else
		dis = dis2;
	end;
	p = p + dis * d;
	dist = dist - dis;
	
	d1 = d(mod(i + 1, 3) + 1) * ior;
	d2 = d(mod(i + 2, 3) + 1) * ior;
	if (d1^2 + d2^2 > 1),
		d(i + 1) = - d(i + 1);
	else
		return;
	end;
	p1 = p + dist * d;
	[isIn cIn(1) cIn(2) cIn(3)] = inside(p1, blockL, blockR);
end;
p = p1;

end

function [isIn xIn yIn zIn] = inside(p, blockL, blockR)


xIn = p(1) - blockL(1) > -eps * max(1.0, max(abs(p(1)), abs(blockL(1)))) && ...
blockR(1) - p(1) > -eps * max(1.0, max(abs(p(1)), abs(blockR(1))));
yIn = p(2) - blockL(2) > -eps * max(1.0, max(abs(p(2)), abs(blockL(2)))) && ...
blockR(2) - p(2) > -eps * max(1.0, max(abs(p(2)), abs(blockR(2))));
zIn = p(3) - blockL(3) > -eps * max(1.0, max(abs(p(3)), abs(blockL(3)))) && ...
blockR(3) - p(3) > -eps * max(1.0, max(abs(p(3)), abs(blockR(3))));
isIn = xIn && yIn && zIn;

end
