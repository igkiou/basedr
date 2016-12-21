function Xp = l1_sphere_projection(X, vals)

% TODO: incorrect.
Xp = simplex_projection(abs(X), vals);
Xp = sign(X) .* Xp;
