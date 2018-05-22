function Xp = simplex_interior_projection_vector(X, val)

[N S] = size(X);

if (S ~= 1),
	error('This version is for a single vector only Call SIMPLEX_PROJECTION instead.');
end;

if ((nargin < 2) || (isempty(val))),
	val = ones(1, S);
end;

if (length(val) ~= S),
	error('Length of val must be equal to 1.');
end;

if (any(val <= 0)),
	error('val must only have positive values.');
end;


X(X < 0) = 0;
Xp = X;
if(sum(X) > val),
	M = sort(X, 1, 'descend');
	CM = cumsum(M);
	CMroj = 1 ./ (1:N)' .* (CM - val);
	rho = find(M - CMroj > 0, 1, 'last');
	theta = CMroj(rho);
	Xp = max(X - theta, 0);
end;
