/**
 * @file vlfeat_sanity.cpp
 * @brief Sanity test for the VLFeat libraries
 * @author Paolo D'Apice
 */

extern "C" {
#include <vl/generic.h>
}

int main(int, char**) {
    VL_PRINT(vl_get_version_string());
    VL_PRINT(vl_configuration_to_string_copy());

    return 0;
}
