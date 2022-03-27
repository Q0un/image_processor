#include "sharpening.h"

namespace filters {

const std::vector<std::vector<float>>& Sharpening::GetMatrix() const {
    return MATRIX;
}

} // namespace filters
