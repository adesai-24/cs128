'#include "db_table.hpp"

void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc) {
  if (col_descs_.size() == row_col_capacity_) {
    for (auto& row : rows_) {
      void** new_row = new void*[row_col_capacity_ * 2];
      for (size_t i = 0; i < row_col_capacity_; i++) {
        new_row[i] = row.second[i];
      }
      delete[] row.second;
      row.second = new_row;
    }
    row_col_capacity_ *= 2;
  }
  col_descs_.push_back(col_desc);
  for (const auto& row : rows_) {
    void** dm_row = row.second;
    DataType last_col_type = col_descs_.back().second;
    if (last_col_type == DataType::kString) {
      dm_row[col_descs_.size() - 1] = new std::string("");
    } else if (last_col_type == DataType::kDouble) {
      dm_row[col_descs_.size() - 1] = new double(0);
    } else if (last_col_type == DataType::kInt) {
      dm_row[col_descs_.size() - 1] = new int(0);
    }
  }
}


void DbTable::DeleteColumnByIdx(unsigned int col_idx) {
  if (col_idx >= col_descs_.size()) {
    throw std::out_of_range("Dawg its out of range");
  }
  if (col_descs_.size() == 1 && !rows_.empty()) {
    throw std::runtime_error("Dawg you can't delete the last column");
  }
  for (const auto& row : rows_) {
    if (col_descs_[col_idx].second == DataType::kString) {
      delete static_cast<std::string*>(row.second[col_idx]);
    } else if (col_descs_[col_idx].second == DataType::kDouble) {
      delete static_cast<double*>(row.second[col_idx]);
    } else if (col_descs_[col_idx].second == DataType::kInt) {
      delete static_cast<int*>(row.second[col_idx]);
    }
    for (unsigned int i = col_idx; i < row_col_capacity_ - 1; ++i) {
      row.second[i] = row.second[i + 1];
    }
  }
  col_descs_.erase(col_descs_.begin() + col_idx);
}

void DbTable::AddRow(const std::initializer_list<std::string>& col_data) {
  if (col_data.size() != col_descs_.size()) {
    throw std::invalid_argument("I JUST LOST MY DAWGGGGGG");
  }
  void** new_row = new void*[row_col_capacity_];
  for (size_t i = 0; i < col_descs_.size(); i++) {
    if (col_descs_[i].second == DataType::kString) {
      new_row[i] = new std::string(col_data.begin()[i]);
    } else if (col_descs_[i].second == DataType::kDouble) {
      new_row[i] = new double(std::stod(col_data.begin()[i]));
    } else if (col_descs_[i].second == DataType::kInt) {
      new_row[i] = new int(std::stoi(col_data.begin()[i]));
    }
  }
  rows_[next_unique_id_] = new_row;
  next_unique_id_++;
}

void DbTable::DeleteRowById(unsigned int id) {
  if (rows_.find(id) == rows_.end()) {
    throw std::invalid_argument("HEHEHEHAW");
  }
  void** d_row = rows_[id];
  for (size_t i = 0; i < col_descs_.size(); i++) {
    if (col_descs_[i].second == DataType::kString) {
      delete static_cast<std::string*>(d_row[i]);
    } else if (col_descs_[i].second == DataType::kDouble) {
      delete static_cast<double*>(d_row[i]);
    } else if (col_descs_[i].second == DataType::kInt) {
      delete static_cast<int*>(d_row[i]);
    }
  }
  delete[] d_row;
  rows_.erase(id);
}

DbTable::DbTable(const DbTable& rhs) {
  next_unique_id_ = rhs.next_unique_id_;
  row_col_capacity_ = rhs.row_col_capacity_;
  col_descs_ = rhs.col_descs_;

  for (const auto& row : rhs.rows_) {
    void** new_row = new void*[row_col_capacity_];
    for (size_t i = 0; i < col_descs_.size(); i++) {
      if (col_descs_[i].second == DataType::kString) {
        new_row[i] = new std::string(*(static_cast<std::string*>(row.second[i])));
      } else if (col_descs_[i].second == DataType::kDouble) {
        new_row[i] = new double(*(static_cast<double*>(row.second[i])));
      } else if (col_descs_[i].second == DataType::kInt) {
        new_row[i] = new int(*(static_cast<int*>(row.second[i])));
      }
    }
    rows_[row.first] = new_row;
  }
}

DbTable& DbTable::operator=(const DbTable& rhs) {
  if (this == &rhs) {
    return *this;
  }

  for (const auto& row : rows_) {
    for (size_t i = 0; i < col_descs_.size(); i++) {
      if (col_descs_[i].second == DataType::kString) {
        delete static_cast<std::string*>(row.second[i]);
      } else if (col_descs_[i].second == DataType::kDouble) {
        delete static_cast<double*>(row.second[i]);
      } else if (col_descs_[i].second == DataType::kInt) {
        delete static_cast<int*>(row.second[i]);
      }
    }
    delete[] row.second;
  }
  rows_.clear();
  next_unique_id_ = rhs.next_unique_id_;
  row_col_capacity_ = rhs.row_col_capacity_;
  col_descs_ = rhs.col_descs_;
  for (const auto& row : rhs.rows_) {
    void** new_row = new void*[row_col_capacity_];
    for (size_t i = 0; i < col_descs_.size(); i++) {
      if (col_descs_[i].second == DataType::kString) {
        new_row[i] = static_cast<void*>(new std::string(*(static_cast<std::string*>(row.second[i]))));
      } else if (col_descs_[i].second == DataType::kDouble) {
        new_row[i] = static_cast<void*>(new double(*(static_cast<double*>(row.second[i]))));
      } else if (col_descs_[i].second == DataType::kInt) {
        new_row[i] = static_cast<void*>(new int(*(static_cast<int*>(row.second[i]))));
      }
    }
    rows_[row.first] = new_row;
  }
  return *this;
}

DbTable::~DbTable() {
  for (const auto& row : rows_) {
    for (size_t i = 0; i < col_descs_.size(); i++) {
      if (col_descs_[i].second == DataType::kString) {
        delete static_cast<std::string*>(row.second[i]);
      } else if (col_descs_[i].second == DataType::kDouble) {
        delete static_cast<double*>(row.second[i]);
      } else if (col_descs_[i].second == DataType::kInt) {
        delete static_cast<int*>(row.second[i]);
      }
    }
    delete[] row.second;
  }
  rows_.clear();
}

std::ostream& operator<<(std::ostream& os, const DbTable& table) {
  for (size_t i = 0; i < table.col_descs_.size(); i++) {
    os << table.col_descs_[i].first << "(";
    if (table.col_descs_[i].second == DataType::kString) {
      os << "std::string)";
    } else if (table.col_descs_[i].second == DataType::kDouble) {
      os << "double)";
    } else if (table.col_descs_[i].second == DataType::kInt) {
      os << "int)";
    }
    if (i != table.col_descs_.size() - 1) {
      os << ", ";
    }
  }
  os << "\n";
  for (const auto& row : table.rows_) {
    for (size_t i = 0; i < table.col_descs_.size(); i++) {
      if (table.col_descs_[i].second == DataType::kString) {
        os << *(static_cast<std::string*>(row.second[i]));
      } else if (table.col_descs_[i].second == DataType::kDouble) {
        os << *(static_cast<double*>(row.second[i]));
      } else if (table.col_descs_[i].second == DataType::kInt) {
        os << *(static_cast<int*>(row.second[i]));
      }
      if (i != table.col_descs_.size() - 1) {
        os << ", ";
      }
    }
    os << "\n";
  }
  return os;
}

size_t DbTable::GetColumnCount() const { return col_descs_.size(); }

size_t DbTable::GetRowCount() const { return rows_.size(); }
