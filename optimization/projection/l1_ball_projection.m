function Xp = l1_ball_projection(X, vals)

inds = sum(abs(X)) > vals;
Xp = X;
Xp(:, inds) = simplex_projection(abs(X(:, inds)), vals(inds));
Xp(:, inds) = sign(X(:, inds)) .* Xp(:, inds);
