#include <iostream>

#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"
#include "path_image.hpp"

int main() {
  const std::string kTestInputFile =
      "./example-data/ex_input_data/map-input-w480-h480.dat";
  constexpr size_t kTestInputFileWidth = 480;
  constexpr size_t kTestInputFileHeight = 480;

  ElevationDataset ed(
      kTestInputFile, kTestInputFileWidth, kTestInputFileHeight);
  GrayscaleImage gi2(ed);
  PathImage pi(gi2, ed);
  pi.ToPpm("result.ppm");
  // gi2.ToPpm("result.ppm");
}