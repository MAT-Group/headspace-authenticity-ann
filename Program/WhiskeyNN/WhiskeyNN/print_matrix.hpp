#include "yannl.hpp"
#include <cstdio>
#include "colorwin.hpp"

using std::printf;

inline void print_matrix_train(const yannl::Matrix& m, const yannl::Matrix& target) {
	printf("<--MATRIX-->\nRows:%zd\nColumns:%zd\n\t\t", m.rows(), m.cols());
	for (size_t i = 0; i < m.rows(); ++i) {
		for (size_t j = 0; j < m.cols(); ++j) {

			if (m[i][j] == 0 && target[i][j] == 0)
			{
				printf("%7.1f", m[i][j]);
			}
			else
			{
				if (m[i][j] == target[i][j])
				{
					colorwin::withcolor scoped(colorwin::green);
					printf("%7.1f", m[i][j]);
				}
				else
				{
					colorwin::withcolor scoped(colorwin::red);
					printf("%7.1f", m[i][j]);
				}
			}
		}
		printf("\n\n\t\t");
	}
	printf("\n");
}

inline void print_matrix(const yannl::Matrix &m) {
	printf("<--MATRIX-->\nRows:%zd\nColumns:%zd\n\t\t", m.rows(), m.cols());
	for (size_t i = 0; i < m.rows(); ++i) {
		for (size_t j = 0; j < m.cols(); ++j) {
			if (m[i][j] == 1)
			{
				colorwin::withcolor scoped(colorwin::cyan);
				printf("%7.1f", m[i][j]);
			}
			else
			{
				printf("%7.1f", m[i][j]);
			}
		}
		printf("\n\n\t\t");
	}
	printf("\n");
}

inline void print_divider() {
	printf("---------------------------------------------------\n");
}