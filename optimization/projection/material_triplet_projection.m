function X = material_triplet_projection(X)
%% project material triplets to valid material space
%	X		3 x N matrix, where each X(:, n) column is a triplet of the
%			form [sigmaT; albedo; gVal].

[C numTriplets] = size(X);

if (C ~= 3),
	error('First dimension of input must be 3 (material triplets).');
end;

% sigmaT must be positive
inds = X(1, :) < 0;
X(1, inds) = 0;

% albedo must be in [0, 1]
inds = X(2, :) < 0;
X(2, inds) = 0;
inds = X(2, :) > 1;
X(2, inds) = 1;

% g must be in [-1, 1]
inds = X(3, :) < -1;
X(3, inds) = -1;
inds = X(3, :) > 1;
X(3, inds) = 1;