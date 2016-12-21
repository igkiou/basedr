function eta = update_eta(iter, eta0, stepUpdate)
% options: 
% 0 = constant;
% 1 = inverse sqrt of iter;
% 2 = inverse of iter.
% 3 = line search.

switch stepUpdate
	case 0,
		eta = eta0;
		return;
	case 1,
		eta = eta0 / sqrt(iter);
		return;
	case 2,
		eta = eta0 / iter;
		return;
	case 3,
		error('Line search method not implemented yet.');
	otherwise,
		error('Invalid stepsize update specification.');
end;
