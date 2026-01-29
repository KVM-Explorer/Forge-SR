export module forge.core.math:matrix;
import std;
export namespace forge{
    
    // 默认采用列主序存储
    template<typename T,size_t Rows,size_t Cols>
    class Matrix{
        alignas(16) std::array<T,Rows*Cols> data_{};
        int Rows_=Rows;
        int Cols_=Cols;
        public:
        static constexpr Matrix identity(){
            Matrix m;
            for(size_t i=0;i<std::min(Rows,Cols);++i){
                m(i,i)=1;
            }
            return m;
        }

        constexpr const T& operator()(size_t row,size_t col) const{
            // 按行列访问元素
            // return data_[row*Cols+col];
            // 按列访问元素
            return data_[col*Rows+row];
        }        
        constexpr T& operator()(size_t row,size_t col){
            // 按行列访问元素
            // return data_[row*Cols+col];
            // 按列访问元素
            return data_[col*Rows+row];
        }
    };

    using Mat4 = Matrix<float,4,4>;
    using Mat3 = Matrix<float,3,3>;

    // Matrix multiplication
    template <typename T, size_t R1, size_t C1, size_t C2>
    auto operator*(const Matrix<T, R1, C1>& a, const Matrix<T, C1, C2>& b) {
        Matrix<T, R1, C2> result;
        for (size_t i = 0; i < R1; ++i) {
            for (size_t j = 0; j < C2; ++j) {
                T sum = 0;
                for (size_t k = 0; k < C1; ++k) {
                    // 像数学书上一样访问：Row i, Col k
                    sum += a(i, k) * b(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    // Scalar multiplication
    template <typename T, size_t R1, size_t C1>
    auto operator*(const Matrix<T, R1, C1>& a, const T& b) {
        Matrix<T, R1, C1> result;
        for (size_t i = 0; i < R1; ++i) {
            for (size_t j = 0; j < C1; ++j) {
                result(i, j) = a(i, j) * b;
            }
        }
    }

    // Matrix addition
    template <typename T, size_t R1, size_t C1>
    auto operator+(const Matrix<T, R1, C1>& a, const Matrix<T, R1, C1>& b) {
        Matrix<T, R1, C1> result;
        for (size_t i = 0; i < R1; ++i) {
            for (size_t j = 0; j < C1; ++j) {
                result(i, j) = a(i, j) + b(i, j);
            }
        }
    }

    // transpose
    template <typename T, size_t R, size_t C>
    auto transpose(const Matrix<T, R, C>& m) {
        Matrix<T, C, R> result;
        for (size_t i = 0; i < R; ++i) {
            for (size_t j = 0; j < C; ++j) {
                result(j, i) = m(i, j);
            }
        }
        std::swap(result.Rows_, result.Cols_);
        return result;
    }
}

