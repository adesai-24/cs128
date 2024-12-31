#include "path_image.hpp"

#include <fstream>

// we will be writing the all paths in path_image_

// PathImage::PathImage(const GrayscaleImage& image, const ElevationDataset& dataset): paths_(dataset.Height(), Path(image.Width(), 0)), width_(dataset.Width()), height_(dataset.Height()), path_image_(image.Height(), std::vector<Color>(image.Width())) {
//   for (size_t i = 0; i < height_; i++) {
//     for (size_t j = 0; j < width_; j++) {
//       path_image_[i][j] = image.ColorAt(i, j);
//     }
//   }
//   size_t best_row = 0;
//   unsigned int min_ele_change = std::numeric_limits<unsigned int>::max();
//   for (size_t i = 0; i < height_; i++) {
//     size_t row = i;
//     Path path(width_, i);
//     unsigned int temp_ele_change = 0;
//     for (size_t j = 0; j < width_ - 1; j++) {
//       int current_ele = dataset.DatumAt(row, j);
//       int next_ele = dataset.DatumAt(row, j+1);
//       if (row == 0) {
//         int next_ele_down = dataset.DatumAt(row+1, j+1);
//         int diff_across = abs(current_ele - next_ele);
//         int diff_down = abs(current_ele - next_ele_down);

//         if (diff_across <= diff_down) {
//           path.SetLoc(j+1, row);
//           temp_ele_change += diff_across;
//         } else {
//           row++;
//           path.SetLoc(j+1, row);
//           temp_ele_change += diff_down;
//         }
//       } else if (row == height_ - 1) {
//         int next_ele_up = dataset.DatumAt(row-1, j+1);
//         int diff_across = abs(current_ele - next_ele);
//         int diff_up = abs(current_ele - next_ele_up);

//         if (diff_across <= diff_up) {
//           path.SetLoc(j+1, row);
//           temp_ele_change += diff_across;
//         } else {
//           row--;
//           path.SetLoc(j+1, row);
//           temp_ele_change += diff_up;
//         }
//       } else {
//         int next_ele_up = dataset.DatumAt(row-1, j+1);
//         int next_ele_down = dataset.DatumAt(row+1, j+1);
//         int diff_across = abs(current_ele - next_ele);
//         int diff_up = abs(current_ele - next_ele_up);
//         int diff_down = abs(current_ele - next_ele_down);

//         if (diff_across <= diff_up && diff_across <= diff_down) {
//           path.SetLoc(j+1, row);
//           temp_ele_change += diff_across;
//         } else if (diff_up < diff_down) {
//           row--;
//           path.SetLoc(j+1, row);
//           temp_ele_change += diff_up;
//         } else if (diff_down <= diff_up) {
//           row++;
//           path.SetLoc(j+1, row);
//           temp_ele_change += diff_down;
//         }
//       }
//     }
//     if (temp_ele_change < min_ele_change) {
//       best_row = i;
//       min_ele_change = temp_ele_change;
//     }
//     paths_[i] = path;
//   }
//   ColorPath(paths_, width_, height_, path_image_, best_row);

// }

constexpr int kR1 = 252;
constexpr int kG1 = 25;
constexpr int kB1 = 63;
constexpr int kR2 = 31;
constexpr int kG2 = 253;
constexpr int kB2 = 13;

PathImage::PathImage(const GrayscaleImage& image, const ElevationDataset& dataset)
    : paths_(dataset.Height(), Path(image.Width(), 0)),
      width_(dataset.Width()), height_(dataset.Height()),
      path_image_(image.Height(), std::vector<Color>(image.Width())) {

  InitializePathImage(image);

  size_t best_row = 0;
  unsigned int min_ele_change = std::numeric_limits<unsigned int>::max();

  for (size_t i = 0; i < height_; i++) {
    size_t row = i;
    Path path(width_, i);
    unsigned int temp_ele_change = 0;
    ProcessRow(dataset, row, path, temp_ele_change);

    if (temp_ele_change < min_ele_change) {
      best_row = i;
      min_ele_change = temp_ele_change;
    }
    paths_[i] = path;
  }

  ColorPath(height_, width_, paths_, path_image_, best_row);
}

void PathImage::InitializePathImage(const GrayscaleImage& image) {
  for (size_t i = 0; i < height_; i++) {
    for (size_t j = 0; j < width_; j++) {
      path_image_[i][j] = image.ColorAt(i, j);
    }
  }
}

void PathImage::ProcessRow(const ElevationDataset& dataset, size_t& row, Path& path, unsigned int& temp_ele_change) {
  for (size_t j = 0; j < width_ - 1; j++) {
    int current_ele = dataset.DatumAt(row, j);
    int next_ele = dataset.DatumAt(row, j + 1);

    if (row == 0) {
      ProcessTopRow(row, dataset, j, current_ele, next_ele, path, temp_ele_change);
    } else if (row == height_ - 1) {
      ProcessBottomRow(row, dataset, j, current_ele, next_ele, path, temp_ele_change);
    } else {
      ProcessMiddleRow(row, dataset, j, current_ele, next_ele, path, temp_ele_change);
    }
  }
}

void PathImage::ProcessTopRow(size_t& row, const ElevationDataset& dataset, size_t j, int current_ele, int next_ele, Path& path, unsigned int& temp_ele_change) {
  int next_ele_down = dataset.DatumAt(row + 1, j + 1);
  int diff_across = abs(current_ele - next_ele);
  int diff_down = abs(current_ele - next_ele_down);

  if (diff_across <= diff_down) {
    path.SetLoc(j + 1, row);
    temp_ele_change += diff_across;
  } else {
    row++;
    path.SetLoc(j + 1, row);
    temp_ele_change += diff_down;
  }
}

void PathImage::ProcessBottomRow(size_t& row, const ElevationDataset& dataset, size_t j, int current_ele, int next_ele, Path& path, unsigned int& temp_ele_change) {
  int next_ele_up = dataset.DatumAt(row - 1, j + 1);
  int diff_across = abs(current_ele - next_ele);
  int diff_up = abs(current_ele - next_ele_up);

  if (diff_across <= diff_up) {
    path.SetLoc(j + 1, row);
    temp_ele_change += diff_across;
  } else {
    row--;
    path.SetLoc(j + 1, row);
    temp_ele_change += diff_up;
  }
}

void PathImage::ProcessMiddleRow(size_t& row, const ElevationDataset& dataset, size_t j, int current_ele, int next_ele, Path& path, unsigned int& temp_ele_change) {
  int next_ele_up = dataset.DatumAt(row - 1, j + 1);
  int next_ele_down = dataset.DatumAt(row + 1, j + 1);
  int diff_across = abs(current_ele - next_ele);
  int diff_up = abs(current_ele - next_ele_up);
  int diff_down = abs(current_ele - next_ele_down);

  if (diff_across <= diff_up && diff_across <= diff_down) {
    path.SetLoc(j + 1, row);
    temp_ele_change += diff_across;
  } else if (diff_up < diff_down) {
    row--;
    path.SetLoc(j + 1, row);
    temp_ele_change += diff_up;
  } else {
    row++;
    path.SetLoc(j + 1, row);
    temp_ele_change += diff_down;
  }
}


void PathImage::ColorPath(size_t height, size_t width, std::vector<Path>& paths, std::vector<std::vector<Color>>& path_image, size_t best_row) {
  for (size_t i = 0; i < height; i++) {
    const Path& path = paths[i];
    for (size_t j = 0; j < width; j++) {
      path_image[path.GetPath().at(j)][j] = Color(kR1, kG1, kB1);
    }
  }
  const Path& best_path = paths[best_row];
  for (size_t j = 0; j < width; j++) {
    path_image[best_path.GetPath().at(j)][j] = Color(kR2, kG2, kB2);
  }

}
size_t PathImage::Height() const { return height_; }
size_t PathImage::Width() const { return width_; }
unsigned int PathImage::MaxColorValue() const { return kMaxColorValue; }
const std::vector<Path>& PathImage::Paths() const { return paths_; }
const std::vector<std::vector<Color>>& PathImage::GetPathImage() const {
  return path_image_;
}
void PathImage::ToPpm(const std::string& name) const {
  std::ofstream file(name);
  if (!file) {
    throw std::runtime_error("I don't know what else to write guys");
  }
  file << "P3\n";
  file << width_ << " " << height_ << "\n";
  file << kMaxColorValue << "\n";

  for (size_t i = 0; i < height_; i++) {
    for (size_t j = 0; j < width_; j++) {
      Color value = path_image_[i][j];
      file << static_cast<int>(value.Red()) << " "
           << static_cast<int>(value.Green()) << " "
           << static_cast<int>(value.Blue()) << " ";
    }
    file << "\n";
  }
}