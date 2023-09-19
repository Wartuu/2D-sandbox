#include <utils/error.h>

void util::throwError(ERROR_TYPE error) {
	std::exit(abs(error)*-1);
}