#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

constexpr int kNumMax = 10000000;

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

int SeamCarver::GetEnergy(int row, int col) const {
  int left_col = (col == 0) ? width_ - 1 : col - 1;
  int right_col = (col == width_ - 1) ? 0 : col + 1;
  int up_row = (row == 0) ? height_ - 1 : row - 1;
  int down_row = (row == height_ - 1) ? 0 : row + 1;

  int col_energy = (image_.GetPixel(row, left_col).GetRed() -
                    image_.GetPixel(row, right_col).GetRed()) *
                       (image_.GetPixel(row, left_col).GetRed() -
                        image_.GetPixel(row, right_col).GetRed()) +
                   (image_.GetPixel(row, left_col).GetGreen() -
                    image_.GetPixel(row, right_col).GetGreen()) *
                       (image_.GetPixel(row, left_col).GetGreen() -
                        image_.GetPixel(row, right_col).GetGreen()) +
                   (image_.GetPixel(row, left_col).GetBlue() -
                    image_.GetPixel(row, right_col).GetBlue()) *
                       (image_.GetPixel(row, left_col).GetBlue() -
                        image_.GetPixel(row, right_col).GetBlue());

  int row_energy = (image_.GetPixel(up_row, col).GetRed() -
                    image_.GetPixel(down_row, col).GetRed()) *
                       (image_.GetPixel(up_row, col).GetRed() -
                        image_.GetPixel(down_row, col).GetRed()) +
                   (image_.GetPixel(up_row, col).GetGreen() -
                    image_.GetPixel(down_row, col).GetGreen()) *
                       (image_.GetPixel(up_row, col).GetGreen() -
                        image_.GetPixel(down_row, col).GetGreen()) +
                   (image_.GetPixel(up_row, col).GetBlue() -
                    image_.GetPixel(down_row, col).GetBlue()) *
                       (image_.GetPixel(up_row, col).GetBlue() -
                        image_.GetPixel(down_row, col).GetBlue());

  return col_energy + row_energy;
}

void SeamCarver::FindMinEnergy(
    int left, int middle, int right, int& min_energy, int& min_dir) const {
  min_energy = middle;
  min_dir = 0;  

  if (left < min_energy || (left == min_energy && min_dir > -1)) {
    min_energy = left;
    min_dir = -1;
  }
  if (right < min_energy || (right == min_energy && min_dir > 1)) {
    min_energy = right;
    min_dir = 1;
  }
}

void SeamCarver::ComputeVerticalEnergySums(int** energy_sums) const {
  int height = GetHeight();
  int width = GetWidth();

  for (int row = 1; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int up_left = (col > 0) ? energy_sums[row - 1][col - 1] : kNumMax;
      int up = energy_sums[row - 1][col];
      int up_right =
          (col < width - 1) ? energy_sums[row - 1][col + 1] : kNumMax;

      int min_energy = 0;
      int min_dir = 0;
      FindMinEnergy(up_left, up, up_right, min_energy, min_dir);

      energy_sums[row][col] = GetEnergy(row, col) + min_energy;
    }
  }
}

int SeamCarver::FindMinIndexInLastRow(int** energy_sums) const {
  int height = GetHeight();
  int width = GetWidth();
  int min_total_energy = kNumMax;
  int min_col = 0;

  for (int col = 0; col < width; col++) {
    if (energy_sums[height - 1][col] < min_total_energy) {
      min_total_energy = energy_sums[height - 1][col];
      min_col = col;
    }
  }
  return min_col;
}

int* SeamCarver::TraceVerticalSeam(int** energy_sums, int min_col) const {
  int height = GetHeight();
  int width = GetWidth();
  int* seam = new int[height];
  seam[height - 1] = min_col;

  for (int row = height - 1; row > 0; row--) {
    int col = seam[row];
    int up_left = (col > 0) ? energy_sums[row - 1][col - 1] : kNumMax;
    int up = energy_sums[row - 1][col];
    int up_right = (col < width - 1) ? energy_sums[row - 1][col + 1] : kNumMax;

    int min_energy = 0;
    int min_dir = 0;
    FindMinEnergy(up_left, up, up_right, min_energy, min_dir);

    seam[row - 1] = col + min_dir;
  }
  return seam;
}

int* SeamCarver::GetVerticalSeam() const {
  int height = GetHeight();
  int width = GetWidth();
  int** energy_sums = new int*[height];
  for (int i = 0; i < height; i++) {
    energy_sums[i] = new int[width];
  }
  for (int col = 0; col < width; col++) {
    energy_sums[0][col] = GetEnergy(0, col);
  }
  ComputeVerticalEnergySums(energy_sums);
  int min_col = FindMinIndexInLastRow(energy_sums);
  int* seam = TraceVerticalSeam(energy_sums, min_col);

  for (int i = 0; i < height; i++) {
    delete[] energy_sums[i];
  }
  delete[] energy_sums;

  return seam;
}

void SeamCarver::ComputeHorizontalEnergySums(int** energy_sums) const {
  int height = GetHeight();
  int width = GetWidth();

  for (int col = 1; col < width; col++) {
    for (int row = 0; row < height; row++) {
      int left_up = (row > 0) ? energy_sums[row - 1][col - 1] : kNumMax;
      int left = (col > 0) ? energy_sums[row][col - 1] : kNumMax;
      int left_down =
          (row < height - 1) ? energy_sums[row + 1][col - 1] : kNumMax;

      int min_energy = 0;
      int min_dir = 0;
      FindMinEnergy(left_up, left, left_down, min_energy, min_dir);

      energy_sums[row][col] = GetEnergy(row, col) + min_energy;
    }
  }
}

int SeamCarver::FindMinIndexInLastColumn(int** energy_sums) const {
  int height = GetHeight();
  int width = GetWidth();
  int min_total_energy = kNumMax;
  int min_row = 0;

  for (int row = 0; row < height; row++) {
    if (energy_sums[row][width - 1] < min_total_energy) {
      min_total_energy = energy_sums[row][width - 1];
      min_row = row;
    }
  }
  return min_row;
}

int* SeamCarver::TraceHorizontalSeam(int** energy_sums, int min_row) const {
  int height = GetHeight();
  int width = GetWidth();
  int* seam = new int[width];
  seam[width - 1] = min_row;

  for (int col = width - 1; col > 0; col--) {
    int row = seam[col];
    int left_up = (row > 0) ? energy_sums[row - 1][col - 1] : kNumMax;
    int left = energy_sums[row][col - 1];
    int left_down =
        (row < height - 1) ? energy_sums[row + 1][col - 1] : kNumMax;

    int min_energy = 0;
    int min_dir = 0;
    FindMinEnergy(left_up, left, left_down, min_energy, min_dir);

    seam[col - 1] = row + min_dir;
  }
  return seam;
}

int* SeamCarver::GetHorizontalSeam() const {
  int height = GetHeight();
  int width = GetWidth();
  int** energy_sums = new int*[height];
  for (int i = 0; i < height; i++) {
    energy_sums[i] = new int[width];
  }
  for (int row = 0; row < height; row++) {
    energy_sums[row][0] = GetEnergy(row, 0);
  }
  ComputeHorizontalEnergySums(energy_sums);
  int min_row = FindMinIndexInLastColumn(energy_sums);
  int* seam = TraceHorizontalSeam(energy_sums, min_row);
  for (int i = 0; i < height; i++) {
    delete[] energy_sums[i];
  }
  delete[] energy_sums;

  return seam;
}

void SeamCarver::RemoveVerticalSeam() {
  int* seam = GetVerticalSeam();
  int new_width = width_ - 1;
  ImagePPM new_image(new_width, height_, image_.GetMaxColorValue());

  for (int row = 0; row < height_; row++) {
    int new_col = 0;
    for (int col = 0; col < width_; col++) {
      if (col != seam[row]) {
        new_image.SetPixel(row, new_col, image_.GetPixel(row, col));
        new_col++;
      }
    }
  }

  image_ = new_image;
  width_ = new_width;
  delete[] seam;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();
  int new_height = height_ - 1;
  ImagePPM new_image(width_, new_height, image_.GetMaxColorValue());

  for (int col = 0; col < width_; col++) {
    int seam_row = seam[col];
    int new_row = 0;
    for (int row = 0; row < height_; row++) {
      if (row != seam_row) {
        new_image.SetPixel(new_row, col, image_.GetPixel(row, col));
        new_row++;
      }
    }
  }

  image_ = new_image;
  height_ = new_height;
  delete[] seam;
}