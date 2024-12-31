#include "elevation_dataset.hpp"
#include <fstream>
#include <stdexcept>

ElevationDataset::ElevationDataset(const std::string& filename, size_t width, size_t height) : width_(width), height_(height), data_(height,std::vector<int>(width)), max_ele_(std::numeric_limits<int>::min()), min_ele_(std::numeric_limits<int>::max()) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("I like apples and bananas");
    }
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            int temp_num = 0;
            file >> temp_num;
            if (file.fail()) {
                throw std::invalid_argument("Me too bro");
            }
            data_[i][j] = temp_num;
            if (temp_num > max_ele_) max_ele_ = temp_num;
            if (temp_num < min_ele_) min_ele_ = temp_num;
        }
    }
    int extra = 0;
    if (file >> extra) {
        throw std::runtime_error("Erm what the sigma");
    }


}

size_t ElevationDataset::Width() const {return width_;}


size_t ElevationDataset::Height() const {return height_;}

int ElevationDataset::MaxEle() const {return max_ele_;}

int ElevationDataset::MinEle() const {return min_ele_;}

int ElevationDataset::DatumAt(size_t row, size_t col) const {return data_[row][col];}

const std::vector<std::vector<int>>& ElevationDataset::GetData() const {return data_;}