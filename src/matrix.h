#ifndef MATRIX_H
#define MATRIX_H


struct Coordinate {
    int row;
    int column;

    inline bool operator==(const Coordinate &b) const {
        return (row == b.row) && (column == b.column);
    }
};

template<>
struct std::hash<Coordinate> {
    std::size_t operator()(const Coordinate c) const noexcept {
        int hash_val = (c.row + c.column) * (c.row + c.column + 1) / 2 + c.row;
        return hash_val;
    }
};

template <typename T>
class Matrix {
public:
    int rows;
    int columns;

    Matrix(int rows, int columns);
    ~Matrix();
    T get(Coordinate coord);
    void set(Coordinate coord, T value);
    
private:
    T** matrix;
};

// implementation

template <typename T>
Matrix<T>::Matrix(int rowcount, int columncount) {
    rows = rowcount;
    columns = columncount;
    matrix  = new T*[rows];
    for (int row = 0; row < rows; row++) {
        matrix[row] = new T[columns];
    }
}

template <typename T>
Matrix<T>::~Matrix() {
    for (int row = 0; row < rows; row++) {
        delete[] matrix[row];
    }
    delete[] matrix;
}

template <typename T>
T Matrix<T>::get(Coordinate coord) {
    return matrix[coord.row][coord.column];
}

template <typename T>
void Matrix<T>::set(Coordinate coord, T value) {
    matrix[coord.row][coord.column] = value;
}
#endif
