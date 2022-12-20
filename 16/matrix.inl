#include "matrix.h"
#include <utility>
#include <cstring>

template<typename T>
matrix2d<T>::matrix2d(size_t rows, size_t columns)
	:m_rows(rows),
	m_columns(columns),
	m_elements((rows > 0 && columns > 0) ? (new T[rows * columns]) : nullptr)
{ }

template<typename T>
matrix2d<T>::matrix2d(const matrix2d<T> &src)
	:m_rows(src.m_rows),
	m_columns(src.m_columns),
	m_elements((m_rows > 0 && m_columns > 0) ? (new T[m_rows * m_columns]) : nullptr)
{ 
	if(m_elements != nullptr && src.m_elements != nullptr) {
		memcpy(m_elements, src.m_elements, sizeof(T) * m_rows * m_columns);
	}
}

template<typename T>
matrix2d<T>::matrix2d(matrix2d<T> &&src)
	:m_rows(std::exchange(src.m_rows, 0)),
	m_columns(std::exchange(src.m_columns, 0)),
	m_elements(std::exchange(src.m_elements, nullptr))
{ }

template<typename T>
matrix2d<T>::~matrix2d()
{
	if(m_elements != nullptr) {
		delete [] m_elements;
	}
}

template<typename T>
matrix2d<T> &matrix2d<T>::operator+=(const matrix2d &rhs)
{
	if(rhs.m_rows != m_rows) throw std::domain_error("Matrix addition is only defined for matrices with the same number of rows");
	if(rhs.m_columns != m_columns) throw std::domain_error("Matrix addition is only defined for matrices with the same number of columns");

	const int i_end=m_rows*m_columns;
	for(int i=0; i<i_end; ++i) {
		m_elements[i] += rhs.m_elements[i];
	}

	return *this;
}

template<typename T>
matrix2d<T> &matrix2d<T>::operator-=(const matrix2d &rhs)
{
	if(rhs.m_rows != m_rows) throw std::domain_error("Matrix subtraction is only defined for matrices with the same number of rows");
	if(rhs.m_columns != m_columns) throw std::domain_error("Matrix subtraction is only defined for matrices with the same number of columns");

	const int i_end=m_rows*m_columns;
	for(int i=0; i<i_end; ++i) {
		m_elements[i] -= rhs.m_elements[i];
	}

	return *this;
}

template<typename T>
matrix2d<T> &matrix2d<T>::operator=(const matrix2d &rhs)
{
	m_rows = rhs.m_rows;
	m_columns = rhs.m_columns;

	if(m_elements != nullptr) {
		delete [] m_elements;		
	}

	const size_t size = m_rows * m_columns;
	m_elements = new T[size];
	memcpy(m_elements, rhs.m_elements, sizeof(T) * size);
	return *this;
}

template<typename T>
matrix2d<T> &matrix2d<T>::operator=(matrix2d &&rhs)
{
	m_rows = std::exchange(rhs.m_rows, 0);
	m_columns = std::exchange(rhs.m_columns, 0);

	if(m_elements != nullptr) {
		delete [] m_elements;		
	}

	m_elements = std::exchange(rhs.m_elements, nullptr);
	return *this;
}

template<typename T>
matrix2d<T> matrix2d<T>::operator+(const matrix2d &rhs) const
{
	if(rhs.m_rows != m_rows) throw std::domain_error("Matrix addition is only defined for matrices with the same number of rows");
	if(rhs.m_columns != m_columns) throw std::domain_error("Matrix addition is only defined for matrices with the same number of columns");

	matrix2d<T> result(m_rows, m_columns);
	const int i_end=m_rows*m_columns;
	for(int i=0; i<i_end; ++i) {
		result.m_elements[i] = m_elements[i] + rhs.m_elements[i];
	}
	return result;
}

template<typename T>
matrix2d<T> matrix2d<T>::operator-(const matrix2d &rhs) const
{
	if(rhs.m_rows != m_rows) throw std::domain_error("Matrix subtraction is only defined for matrices with the same number of rows");
	if(rhs.m_columns != m_columns) throw std::domain_error("Matrix subtraction is only defined for matrices with the same number of columns ");

	matrix2d<T> result(m_rows, m_columns);
	const int i_end=m_rows*m_columns;
	for(int i=0; i<i_end; ++i) {
		result.m_elements[i] = m_elements[i] - rhs.m_elements[i];
	}
	return result;
}

template<typename T>
matrix2d<T> matrix2d<T>::operator*(const matrix2d &rhs) const
{
	if(m_columns != rhs.m_rows) throw std::domain_error("Matrix multiplication is only defined when the number of columns in the left-side matrix equal the number of rows in the right-side matrix");

	matrix2d<T> result(m_rows, rhs.m_columns);
	result.zero();
	for(int i=0; i<m_rows; ++i) {
		for(int j=0; i<rhs.m_columns; ++j) {
			auto rv = row_vector(i);
			auto cv = rhs.column_vector(j);
			auto &vv = result.m_elements[i*rhs.m_columns + j];

			for(int k=0; k<m_columns; ++k) {
				vv += rv[k] * cv[k];
			}
		}
	}
	return result;
}

template<typename T>
bool matrix2d<T>::is_invalid() const
{
	return m_rows == 0 || m_columns == 0 || m_elements == nullptr;
}

template<typename T>
bool matrix2d<T>::is_square() const
{
	return m_rows == m_columns;
}


template<typename T>
size_t matrix2d<T>::get_rows() const
{
	return m_rows;
}

template<typename T>
size_t matrix2d<T>::get_columns() const
{
	return m_columns;
}

template<typename T>
void matrix2d<T>::zero()
{
	memset(m_elements, 0, sizeof(T) * m_rows * m_columns);
}

template<typename T>
void matrix2d<T>::fill(T value)
{
	const int i_end=m_rows*m_columns;
	for(int i=0; i<i_end; ++i) {
		m_elements[i] = value;
	}
}
