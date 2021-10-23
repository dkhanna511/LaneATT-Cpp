
import pcl

def fitPlane(point_cloud, max_distance):

  segmenter = point_cloud.make_segmenter()

  segmenter.set_model_type(pcl.SACMODEL_PLANE)
  segmenter.set_method_type(pcl.SAC_RANSAC)
  segmenter.set_distance_threshold(max_distance)

  #obtain inlier indices and model coefficients
  inlier_indices, coefficients = segmenter.segment()

  inliers = point_cloud.extract(inlier_indices, negative = False)
  # outliers = point_cloud.extract(inlier_indices, negative = True)
  print(coefficients)

  return inliers, coefficients
