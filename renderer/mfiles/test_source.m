function [srcPos srcDir scnPos scnDir] = test_source(rayDir, rayOriginOrig, frontLighting, numPhotons, showViz)
%% 
% All units are in mm.
if (nargin < 5),
	showViz = 1;
end;

theta = 0:0.001:pi;
func = evaluateHG(theta, 0.8);
% func = zeros(size(theta)); func(1) = 1;
% [theta func] = readDensity('../samples/hg0p8.txt');
sigmaT = 0;
sigmaS = 0;

iorMedium = 1.2;
% mediumDimensions = [2 * 1.25; 2 * 27.5; 2 * 27.5];
mediumDimensions = [2.5; 22; 34];

if (frontLighting == 0) ,
	rayOrigin = [-mediumDimensions(1) / 2; rayOriginOrig];
else
	rayOrigin = [mediumDimensions(1) / 2; rayOriginOrig];
end;
% rayOrigin = [0.0 0.0];
% rayAngle = -0.3927;
% rayAngle = -0.7854;
% rayAngle = 0;
% rayDir = [cos(rayAngle); sin(rayAngle); 0];
% rayDir = ones(3, 1); rayDir = normvec(rayDir);
% rayDir = randn(3,1); rayDir = normvec(rayDir);
[c1 c2] = getCoordFromNormal(rayDir)
refDir = vectorSnell(1, iorMedium, [1;0;0], rayDir)'
rayRadius = 0.5;
Li = 75000.0;

viewOrigin = [0.0; 0.0];

% viewDir = [1.0; 1.0; -0.5];
% viewY = [0.5; 0.5; 2.0];
viewDir = [-1.0; 0.0; 0.0];
viewY = [0.0; 0.0; -1.0];
% viewY = [0.0; 1.0; 0.0];
viewPlane = [50; 50];
viewReso = [128; 128];

% hstep = 1.0 / sigmaT * 0.0001;	% this appears to give better means
if (sigmaT > 0),
	hstep = 1.0 / sigmaT * 0.001;
else
	hstep = mediumDimensions(1);
end;
% numPhotons = 5000000;
useContinuous = 1;

[srcPos srcDir scnPos scnDir] = test_source_mex(theta, func, sigmaT, sigmaS, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, useContinuous);

if (showViz),
%%

figure;
plot3(srcPos(1, :), srcPos(2, :), srcPos(3, :), '.');  hold on;
axis auto
% axis([-0.2 0 -0.05 0.05 -0.05 0.05]);
xlabel('x'); ylabel('y'); zlabel('z'); grid on;
plot3(scnPos(1, :), scnPos(2, :), scnPos(3, :), 'rx'); hold on
quiver3(srcPos(1,:), srcPos(2,:), srcPos(3,:), srcDir(1,:), srcDir(2,:), srcDir(3,:)); hold on;
quiver3(scnPos(1,:), scnPos(2,:), scnPos(3,:), scnDir(1,:), scnDir(2,:), scnDir(3,:), 'r'); hold on;
plotPlane(rayDir, rayOrigin, [mediumDimensions(1) / 2; 2 * rayRadius; 2 * rayRadius]);
plotPlane([-1; 0; 0], rayOrigin, [mediumDimensions(1); 2 * rayRadius; 2 * rayRadius]);
for iter = 1:size(srcPos, 2),
	line([srcPos(1,iter) scnPos(1,iter)],...
		[srcPos(2,iter) scnPos(2,iter)],...
		[srcPos(3,iter) scnPos(3,iter)], 'color', [0 0 0]);
end;

figure;
plot(srcPos(1, :), srcPos(2, :), '.');  hold on;
axis auto
% axis([-0.2 0 -0.05 0.05 -0.05 0.05]);
xlabel('x'); ylabel('y'); grid on;
plot(scnPos(1, :), scnPos(2, :), 'rx'); hold on
quiver(srcPos(1,:), srcPos(2,:), srcDir(1,:), srcDir(2,:)); hold on;
quiver(scnPos(1,:), scnPos(2,:), scnDir(1,:), scnDir(2,:), 'r'); hold on;

figure;
plot(srcPos(1, :), srcPos(3, :), '.');  hold on;
axis auto
% axis([-0.2 0 -0.05 0.05 -0.05 0.05]);
xlabel('x'); ylabel('z'); grid on;
plot(scnPos(1, :), scnPos(3, :), 'rx'); hold on
quiver(srcPos(1,:), srcPos(3,:), srcDir(1,:), srcDir(3,:)); hold on;
quiver(scnPos(1,:), scnPos(3,:), scnDir(1,:), scnDir(3,:), 'r'); hold on;

figure;
plot(srcPos(2, :), srcPos(3, :), '.');  hold on;
axis auto; axis equal;
% axis([-0.2 0 -0.05 0.05 -0.05 0.05]);
xlabel('y'); ylabel('z'); grid on;
plot(scnPos(2, :), scnPos(3, :), 'rx'); hold on
quiver(srcPos(2,:), srcPos(3,:), srcDir(2,:), srcDir(3,:)); hold on;
quiver(scnPos(2,:), scnPos(3,:), scnDir(2,:), scnDir(3,:), 'r'); hold on;

%%
end;
