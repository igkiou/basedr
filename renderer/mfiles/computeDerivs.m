syms gVal theta
hg = 1 / 4 * (1 - gVal ^ 2) ./ (1 + gVal ^ 2 - 2 * gVal * cos(theta)) .^ (3 / 2);
simplify(diff(hg, gVal))
simplify(diff(hg, gVal) / hg)