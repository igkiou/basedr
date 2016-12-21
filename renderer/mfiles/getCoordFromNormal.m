function [coord1 coord2] = getCoordFromNormal(dir)

xZero = (dir(1) == 0);
yZero = (dir(2) == 0);
zZero = (dir(3) == 0);

if (~(xZero || yZero || zZero)),
	coord1(1) = dir(3);
	coord1(2) = dir(3);
	coord1(3) = - dir(1) - dir(2);
elseif (~(xZero || yZero)),
	coord1(1) = dir(2);
	coord1(2) = - dir(1);
	coord1(3) = 0.0;
elseif (~(yZero || zZero)),
	coord1(1) = 0.0;
	coord1(2) = dir(3);
	coord1(3) = - dir(2);
elseif (~(zZero || xZero)),
	coord1(1) = - dir(3);
	coord1(2) = 0.0;
	coord1(3) = dir(1);
elseif (~xZero),
	coord1(1) = 0.0;
	coord1(2) = 1.0;
	coord1(3) = 0.0;
elseif (~yZero),
	coord1(1) = 0.0;
	coord1(2) = 0.0;
	coord1(3) = 1.0;
elseif (~zZero),
	coord1(1) = 1.0;
	coord1(2) = 0.0;
	coord1(3) = 0.0;
end;
coord1 = normvec(coord1);
coord2 = cross(dir, coord1);
