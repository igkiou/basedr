function plotSourceTests(srcPos, srcDir, scnPos, scnDir)

figure;
plot3(srcPos(1, :), srcPos(2, :), srcPos(3, :), '.');  hold on;
axis auto
% axis([-0.2 0 -0.05 0.05 -0.05 0.05]);
xlabel('x'); ylabel('y'); zlabel('z'); grid on;
plot3(scnPos(1, :), scnPos(2, :), scnPos(3, :), 'rx'); hold on
quiver3(srcPos(1,:), srcPos(2,:), srcPos(3,:), srcDir(1,:), srcDir(2,:), srcDir(3,:)); hold on;
quiver3(scnPos(1,:), scnPos(2,:), scnPos(3,:), scnDir(1,:), scnDir(2,:), scnDir(3,:), 'r'); hold on;

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
