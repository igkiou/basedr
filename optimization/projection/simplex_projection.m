function Xp = simplex_projection(X, vals)

[N S] = size(X);

if ((nargin < 2) || (isempty(vals))),
	vals = ones(1, S);
end;

if (length(vals) ~= S),
	error('Length of VALS must be equal to number of vectors to project.');
end;

if (any(vals <= 0)),
	error('VALS must only have positive values.');
end;

M = sort(X, 1, 'descend');
CM = cumsum(M);
CMroj = bsxfun(@times, 1 ./ (1:N)', bsxfun(@minus, CM, vals));
rho = sum(M - CMroj > 0);
theta = CMroj(sub2ind([N S], rho, 1:S));
Xp = max(bsxfun(@minus, X, theta), 0);
