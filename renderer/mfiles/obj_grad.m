function [val, deriv, img, dSigmaT, dAlbedo, dGVal] = obj_grad(materialParams,...
														samplingMaterialParams,...
														scene, renderer)

sigmaT = materialParams(1);
albedo = materialParams(2);
gVal = materialParams(3);
samplingSigmaT = samplingMaterialParams(1);
samplingAlbedo = samplingMaterialParams(2);
samplingGVal = samplingMaterialParams(3);

assert(sigmaT > 0);
assert((albedo >= 0) && (albedo <= 1));
assert((gVal >= 0) && (gVal <= 1));
assert(samplingSigmaT > 0);
assert((samplingAlbedo >= 0) && (samplingAlbedo <= 1));
assert((samplingGVal >= 0) && (samplingGVal <= 1));
												
[img, dSigmaT, dAlbedo, dGVal] = renderDerivImageWeight(sigmaT, albedo, gVal,...
										samplingSigmaT, samplingAlbedo, samplingGVal, scene, renderer);
									
val = meanv(img);
deriv = [meanv(dSigmaT); meanv(dAlbedo); meanv(dGVal);];