#include "grayscale_image.hpp"
#include "elevation_dataset.hpp"
#include <fstream>
#include <cmath>
// shade_of_gray = std::round((elevation_point - min_ele) / (max_ele - min_ele) * 255)

GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset) : width_(dataset.Width()), height_(dataset.Height()), image_(dataset.Height(), std::vector<Color>(dataset.Width())) {
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            int shade_of_gray = 0;
            int elev_point = dataset.DatumAt(i,j);; 
            if (dataset.MaxEle() != dataset.MinEle()) {
                shade_of_gray = static_cast<int>(std::round(static_cast<double>(elev_point - dataset.MinEle()) * kMaxColorValue / static_cast<double>(dataset.MaxEle() - dataset.MinEle())));
            } 
            image_[i][j] = Color(shade_of_gray, shade_of_gray, shade_of_gray);
        }
    }

}
GrayscaleImage::GrayscaleImage(const std::string& filename, size_t width, size_t height) : width_(width), height_(height), image_(height, std::vector<Color>(width)) {
    ElevationDataset dataset(filename, width, height);
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            int shade_of_gray = 0;
            int elev_point = dataset.DatumAt(i,j);
            if (dataset.MaxEle() != dataset.MinEle()) {
                shade_of_gray = static_cast<int>(std::round(static_cast<double>(elev_point - dataset.MinEle()) * kMaxColorValue / static_cast<double>(dataset.MaxEle() - dataset.MinEle())));
            } 
            image_[i][j] = Color(shade_of_gray, shade_of_gray, shade_of_gray);
        }
    }
}
size_t GrayscaleImage::Width() const {return width_;}
size_t GrayscaleImage::Height() const {return height_;}
unsigned int GrayscaleImage::MaxColorValue() const {return kMaxColorValue;}
const Color& GrayscaleImage::ColorAt(int row, int col) const {return image_[row][col];}
const std::vector<std::vector<Color> >& GrayscaleImage::GetImage() const {return image_;}
void GrayscaleImage::ToPpm(const std::string& name) const {
    std::ofstream file(name);
    if (!file) {
        throw std::runtime_error("I don't know what else to write guys");
    }
    file << "P3\n";
    file << width_ << " " << height_ << "\n";
    file << kMaxColorValue << "\n";

    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            Color value = image_.at(i).at(j);
            file << value.Red() << " " << value.Green() << " " << value.Blue() << " ";

        }
        file << "\n";
    }
}	

