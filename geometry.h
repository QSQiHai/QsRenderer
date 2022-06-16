#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

template<int n> struct vec {
	vec() = default;
	double& operator[](const int i) { assert(i >= 0 && i < n); return data[i]; }
	const double operator[](const int i) const { assert(i >= 0 && i < n); return data[i]; }
	double norm2() const { return *this * *this; }
	double norm() const { return std::sqrt(norm2()); }
	double data[n] = { 0 };
};

template<int n1, int n2> vec<n1> embed(const vec<n2>& v, double fill = 1) { vec<n1> ret; for (int i = 0; i < n1; i++) { ret[i] = (i < n2 ? v[i] : fill); } return ret; }

template<int n1, int n2> vec<n1> proj(const vec<n2>& v) { vec<n1> ret; for (int i = 0; i < n1; i++) { ret[i] = v[i]; } return ret; }

template<int n> double operator*(const vec<n>& lhs, const vec<n>& rhs) { double ret = 0;	for (int i = 0; i < n; i++) { ret += lhs[i] * rhs[i]; }	return ret; }
template<int n> vec<n> operator+(const vec<n>& lhs, const vec<n>& rhs) { vec<n> ret = lhs; for (int i = 0; i < n; i++) { ret[i] += rhs[i]; } return ret; }
template<int n> vec<n> operator-(const vec<n>& lhs, const vec<n>& rhs) { vec<n> ret = lhs; for (int i = 0; i < n; i++) { ret[i] -= rhs[i]; } return ret; }
template<int n> vec<n> operator*(const double& rhs, const vec<n>& lhs) { vec<n> ret = lhs; for (int i = 0; i < n; i++) { ret[i] *= rhs; } return ret; }
template<int n> vec<n> operator*(const vec<n>& lhs, const double& rhs) { vec<n> ret = lhs; for (int i = 0; i < n; i++) { ret[i] *= rhs; } return ret; }
template<int n> vec<n> operator/(const vec<n>& lhs, const double& rhs) { vec<n> ret = lhs; for (int i = 0; i < n; i++) { ret[i] /= rhs; } return ret; }
template<int n> std::ostream& operator<<(std::ostream& out, const vec<n>& v) { for (int i = 0; i < n; i++) { out << v[i] << " "; }	return out; }

template<> struct vec<2> {
	vec() = default;
	vec(double x, double y) : x(x), y(y) {}
	double& operator[](const int i) { assert(i >= 0 && i < 2); return i ? y : x; }
	double  operator[](const int i) const { assert(i >= 0 && i < 2); return i ? y : x; }
	double norm2() const { return *this * *this; }
	double norm()  const { return std::sqrt(norm2()); }
	vec& normalize() { *this = (*this) / norm(); return *this; }
	double x{}, y{};
};

template<> struct vec<3> {
	vec() = default;
	vec(double x, double y, double z) : x(x), y(y), z(z) {}
	double& operator[](const int i) { assert(i >= 0 && i < 3); return i ? (1 == i ? y : z) : x; }
	double  operator[](const int i) const { assert(i >= 0 && i < 3); return i ? (1 == i ? y : z) : x; }
	double norm2() const { return *this * *this; }
	double norm()  const { return std::sqrt(norm2()); }
	vec& normalize() { *this = (*this) / norm(); return *this; }
	double x{}, y{}, z{};
};

typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;
vec3 cross(const vec3& v1, const vec3& v2);

template<int n> struct dt;


template<int n, int m>
struct mat {
	vec<m> rows[n] = { {} };

	vec<m>& operator[] (const int idx) {
		assert(idx >= 0 && idx < n);
		return rows[idx];
	}

	const vec<m>& operator[] (const int idx) const {
		assert(idx >= 0 && idx < n);
		return rows[idx];
	}

	vec<n> col(const int idx) const {
		assert(idx >= 0 && idx < m);
		vec<n> ret;
		for (int i = 0; i < n; i++) {
			ret[i] = rows[i][idx];
		}
		return ret;
	}

	void set_col(const int idx, const vec<n>& v) {
		assert(idx >= 0 && idx < m);
		for (int i = 0; i < n; i++) {
			rows[i][idx] = v[i];
		}
		return;
	}

	static mat<n, m> identity() {
		mat<n, m> ret;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				ret[i][j] = (i == j);
			}
		}
		return ret;
	}

	double det() const {
		return dt<m>::det(*this);
	}

	mat<n - 1, m - 1> get_minor(const int row, const int col) const {
		mat<n - 1, m - 1> ret;
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < m - 1; j++) {
				ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1];
			}
		}
		return ret;
	}

	double cofactor(const int row, const int col) const {
		return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
	}

	mat<n, m> adjugate() const {
		mat<n, m> ret;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				ret[i][j] = cofactor(i, j);
			}
		}
		return ret;
	}

	mat<n, m> invert_transpose() const {
		mat<n, m> ret = adjugate();
		return ret / (ret[0] * rows[0]);
	}

	mat<n, m> invert() const {
		return invert_transpose().transpose();
	}

	mat<m, n> transpose() const {
		mat<m, n> ret;
		for (int i = 0; i < m; i++) {
			ret[i] = this->col(i);
		}
		return ret;
	}
};


template<int n>
struct dt {
	static double det(const mat<n, n>& src) {
		double ret = 0;
		for (int i = 0; i < n; i++) {
			ret += src[0][i] * src.cofactor(0, i);
		}
		return ret;
	}
};

template<>
struct dt<1> {
	static double det(const mat<1, 1>& src) {
		return src[0][0];
	}
};

template<int n, int m>
vec<n> operator*(const mat<n, m>& lhs, const vec<m>& rhs) {
	vec<n> ret;
	for (int i = 0; i < n; i++) {
		ret[i] = lhs[i] * rhs;
	}
	return ret;
}

template<int R1, int C1, int C2>
mat<R1, C2> operator*(const mat<R1, C1>& lhs, const mat<C1, C2>& rhs) {
	mat<R1, C2> result;
	for (int i = 0; i < R1; i++) {
		for (int j = 0; j < C2; j++) {
			result[i][j] = lhs[i] * rhs.col(j);
		}
	}
	return result;
}

template<int n, int m>
mat<n, m> operator*(const mat<n, m>& lhs, const double& val) {
	mat<n, m> result;
	for (int i = 0; i < n; i++) {
		result[i] = lhs[i] * val;
	}
	return result;
}

template<int n, int m>
mat<n, m> operator/(const mat<n, m>& lhs, const double& val) {
	mat<n, m> result;
	for (int i = 0; i < n; i++) {
		result[i] = lhs[i] / val;
	}
	return result;
}

template<int n, int m>
mat<n, m> operator+(const mat<n, m>& lhs, const mat<n, m>& rhs) {
	mat<n, m> result;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			result[i][j] = lhs[i][j] + rhs[i][j];
		}
	}
	return result;
}

template<int n, int m>
mat<n, m> operator-(const mat<n, m>& lhs, const mat<n, m>& rhs) {
	mat<n, m> result;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			result[i][j] = lhs[i][j] - rhs[i][j];
		}
	}
	return result;
}

template<int n, int m>
std::ostream& operator<<(std::ostream& out, const mat<n, m>& matrix) {
	for (int i = 0; i < n; i++) {
		out << matrix[i] << std::endl;
	}
	return out;
}
