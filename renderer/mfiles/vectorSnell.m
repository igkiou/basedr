function [refDir fresnelTrans] = vectorSnell(n1, n2, n, rayDir)

% This must point from medium 1 to medium 2. This example is
% for backside.
% nm = [-1; 0; 0];
% n = - nm;	
eta = n2 / n1;
c1 = n' * rayDir;
c2 = sqrt(1 - 1 / eta ^ 2 * (1 - c1 ^ 2));
refDir = 1 / eta * rayDir - (1 / eta * c1 - c2) * n;

Rs = abs(c1 - eta * c2) / (c1 + eta * c2);
Rt = abs(c2 - eta * c1) / (c2 + eta * c1);
R = (Rs ^ 2 + Rt ^ 2) / 2;
fresnelTrans = 1 - R;
