function Xp = simplex_interior_projection(X, vals)

X(X < 0) = 0;
inds = sum(X) > vals;
Xp = X;
Xp(:, inds) = simplex_projection(X(:, inds), vals(inds));
