sigmaT = 5.0;
% sigmaT = 100;
albedo = 0.99;
g = 0.8;

iorMedium = 1.0;
mediumDimensions = [2 * 0.125, 2 * 2.75, 2 * 2.75];

rayOrigin = [0 0];
rayAngle = -0.3927;
% rayAngle = 0;
rayDir = [cos(rayAngle), sin(rayAngle) 0];
rayRadius = 0.05;
Li = 75000.0;

viewOrigin = [0.0, 0.0];
viewDir = [-1.0, 0.0, 0.0];
viewY = [0.0, 0.0, 1.0];
viewPlane = [5, 5];
viewReso = [128, 128];

numPhotons = 5000000;

cd ..
!make distclean; make USE_GCC=1 USE_LIST=1
cd mexfiles

useContinuous = 1;
im_hg0p8_gcc_mat_double_list_continuous = renderImage_mex(sigmaT, albedo, g, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, useContinuous)';
					
cd ..
!make distclean; make USE_GCC=1
cd mexfiles

useContinuous = 1;
im_hg0p8_gcc_mat_double_nolist_continuous = renderImage_mex(sigmaT, albedo, g, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, useContinuous)';

cd ..
!make distclean; make USE_LIST=1 USE_NO_OPENEXR=1
cd mexfiles

useContinuous = 1;
im_hg0p8_icpc_mat_double_list_continuous = renderImage_mex(sigmaT, albedo, g, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, useContinuous)';
					
cd ..
!make distclean; make USE_NO_OPENEXR=1
cd mexfiles

useContinuous = 1;
im_hg0p8_icpc_mat_double_nolist_continuous = renderImage_mex(sigmaT, albedo, g, ...
						iorMedium, mediumDimensions, ...
						rayOrigin, rayDir, rayRadius, Li, ...
						viewOrigin, viewDir, viewY, viewPlane, viewReso, ...
						hstep, numPhotons, useContinuous)';
