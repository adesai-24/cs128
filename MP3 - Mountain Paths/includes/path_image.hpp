#ifndef PATH_IMAGE_H
#define PATH_IMAGE_H

#include <cstdlib>  // for size_t
#include <vector>
#include <string>
#include <limits>

#include "color.hpp"
#include "path.hpp"
#include "grayscale_image.hpp"
#include "elevation_dataset.hpp"
#include <cmath>


class PathImage {
public:
  // write behavior declarations here; define in path_image.cc.
  PathImage(const GrayscaleImage &image, const ElevationDataset &dataset);
  size_t Width() const;
  size_t Height() const;
  unsigned int MaxColorValue() const;
  const std::vector<Path>& Paths() const;
  const std::vector<std::vector<Color> >& GetPathImage() const;
  void ToPpm(const std::string& name) const;	
  void ColorPath(size_t height_, size_t width_, std::vector<Path>& paths_, std::vector<std::vector<Color>>& path_image_, size_t best_row);
  void InitializePathImage(const GrayscaleImage& image);
  void ProcessRow(const ElevationDataset& dataset, size_t& row, Path& path, unsigned int& temp_ele_change);
  void ProcessTopRow(size_t& row, const ElevationDataset& dataset, size_t j, int current_ele, int next_ele, Path& path, unsigned int& temp_ele_change);
  void ProcessBottomRow(size_t& row, const ElevationDataset& dataset , size_t j, int current_ele, int next_ele, Path& path, unsigned int& temp_ele_change);
  void ProcessMiddleRow(size_t& row, const ElevationDataset& dataset, size_t j, int current_ele, int next_ele, Path& path, unsigned int& temp_ele_change);
private:
  std::vector<Path> paths_; // this is the actual path which we will have to calculate
  size_t width_ = 0;
  size_t height_ = 0;
  std::vector<std::vector<Color>> path_image_; // this is the data member that stores the color values for each pixel and u use for ppm
  static const int kMaxColorValue = 255;
};

#endif
