function [dis res] = inter(L, R, p, d)

dim = length(p);
for iter = 1:dim,
	l(iter) = (L - p(iter)) / d(iter);
	r(iter) = (R - p(iter)) / d(iter);
	if (l(iter) > r(iter)), e = r(iter); r(iter) = l(iter); l(iter) = e; end;
end;
dis.x = l(1);
for iter = 2:dim,
	if l(iter) > dis.x, dis.x = l(iter); end;
end;
dis.y = r(1);
for iter = 2:dim,
	if r(iter) > dis.y, dis.y = r(iter); end;
end;
if (dis.x < 0), dis.x = 0; end;
if (dis.x - dis.y > -eps), res = false;
else res = true;
end;
[p d]
p + dis.x*d
p + dis.y*d
