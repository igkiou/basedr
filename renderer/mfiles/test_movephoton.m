function [reflected fPos fDir posBuff dirBuff] = test_movephoton(iPos, iDir, dist, showViz)
%% 
% All units are in mm.
if (nargin < 4),
	showViz = 1;
end;

iorMedium = 1.5;
% mediumDimensions = [2 * 1.25; 2 * 27.5; 2 * 27.5];
mediumDimensions = [2.5; 22; 34];

rayOrigin = [0.0 0.0];
rayDir = ones(3, 1); rayDir = normvec(rayDir);
rayRadius = 0.5;
Li = 75000.0;

viewOrigin = [0.0; 0.0];
viewDir = [-1.0; 0.0; 0.0];
viewY = [0.0; 0.0; -1.0];
viewPlane = [50; 50];
viewReso = [128; 128];

[reflected fPos fDir posBuff dirBuff] = test_move_photon_mex(...
						iorMedium, mediumDimensions,...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						iPos, iDir, dist);

if (showViz),
%%

figure;
plot([-mediumDimensions(1)/2;-mediumDimensions(1)/2;mediumDimensions(1)/2;mediumDimensions(1)/2;-mediumDimensions(1)/2],...
		[-mediumDimensions(2)/2;mediumDimensions(2)/2;mediumDimensions(2)/2;-mediumDimensions(2)/2;-mediumDimensions(2)/2]);
hold on;
plot(iPos(1), iPos(2), 'xg', 'Markersize', 10);
quiver(iPos(1), iPos(2), iDir(1), iDir(2));
plot(fPos(1), fPos(2), 'xr', 'Markersize', 10);
quiver(fPos(1), fPos(2), fDir(1), fDir(2));
plot(posBuff(1, :), posBuff(2, :));
quiver(posBuff(1, :), posBuff(2, :), dirBuff(1, :), dirBuff(2, :));

% figure;
% plot3(srcPos(1, :), srcPos(2, :), srcPos(3, :), '.');  hold on;
% axis auto
% % axis([-0.2 0 -0.05 0.05 -0.05 0.05]);
% xlabel('x'); ylabel('y'); zlabel('z'); grid on;
% plot3(scnPos(1, :), scnPos(2, :), scnPos(3, :), 'rx'); hold on
% quiver3(srcPos(1,:), srcPos(2,:), srcPos(3,:), srcDir(1,:), srcDir(2,:), srcDir(3,:)); hold on;
% quiver3(scnPos(1,:), scnPos(2,:), scnPos(3,:), scnDir(1,:), scnDir(2,:), scnDir(3,:), 'r'); hold on;
% plotPlane(rayDir, [-mediumDimensions(1) / 2; rayOrigin], [mediumDimensions(1) / 2; 2 * rayRadius; 2 * rayRadius]);
% plotPlane([-1; 0; 0], [-mediumDimensions(1) / 2; rayOrigin], [mediumDimensions(1); 2 * rayRadius; 2 * rayRadius]);
% for iter = 1:size(srcPos, 2),
% 	line([srcPos(1,iter) scnPos(1,iter)],...
% 		[srcPos(2,iter) scnPos(2,iter)],...
% 		[srcPos(3,iter) scnPos(3,iter)], 'color', [0 0 0]);
% end;
% 
% figure;
% plot(srcPos(1, :), srcPos(2, :), '.');  hold on;
% axis auto
% % axis([-0.2 0 -0.05 0.05 -0.05 0.05]);
% xlabel('x'); ylabel('y'); grid on;
% plot(scnPos(1, :), scnPos(2, :), 'rx'); hold on
% quiver(srcPos(1,:), srcPos(2,:), srcDir(1,:), srcDir(2,:)); hold on;
% quiver(scnPos(1,:), scnPos(2,:), scnDir(1,:), scnDir(2,:), 'r'); hold on;
% 
% figure;
% plot(srcPos(1, :), srcPos(3, :), '.');  hold on;
% axis auto
% % axis([-0.2 0 -0.05 0.05 -0.05 0.05]);
% xlabel('x'); ylabel('z'); grid on;
% plot(scnPos(1, :), scnPos(3, :), 'rx'); hold on
% quiver(srcPos(1,:), srcPos(3,:), srcDir(1,:), srcDir(3,:)); hold on;
% quiver(scnPos(1,:), scnPos(3,:), scnDir(1,:), scnDir(3,:), 'r'); hold on;
% 
% figure;
% plot(srcPos(2, :), srcPos(3, :), '.');  hold on;
% axis auto; axis equal;
% % axis([-0.2 0 -0.05 0.05 -0.05 0.05]);
% xlabel('y'); ylabel('z'); grid on;
% plot(scnPos(2, :), scnPos(3, :), 'rx'); hold on
% quiver(srcPos(2,:), srcPos(3,:), srcDir(2,:), srcDir(3,:)); hold on;
% quiver(scnPos(2,:), scnPos(3,:), scnDir(2,:), scnDir(3,:), 'r'); hold on;

%%
end;
