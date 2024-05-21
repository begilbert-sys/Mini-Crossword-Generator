#ifndef MATRIX_H
#define MATRIX_H


struct Coordinates {
    int row;
    int column;

    inline bool operator==(const Coordinates &b) const {
        return (row == b.row) && (column == b.column);
    }
};

template<>
struct std::hash<Coordinates> {
    std::size_t operator()(const Coordinates c) const noexcept {
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
    T get(Coordinates coord);
    void set(Coordinates coord, T value);
    
    void fill(T value);
    
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
T Matrix<T>::get(Coordinates coords) {
    return matrix[coords.row][coords.column];
}

template <typename T>
void Matrix<T>::set(Coordinates coords, T value) {
    matrix[coords.row][coords.column] = value;
}


/* fill the matrix with a single value */
template <typename T>
void Matrix<T>::fill(T value) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            matrix[row][col] = value;
        }
    }
}
#endif
