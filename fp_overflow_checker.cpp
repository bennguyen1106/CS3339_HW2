#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstring>
#include <cmath>

using namespace std;

void printIEEEBits(float f) {
    uint32_t bits;
    memcpy(&bits, &f, sizeof(bits));
    unsigned int sign = (bits >> 31) & 0x1;
    unsigned int exponent = (bits >> 23) & 0xFF;
    unsigned int fraction = bits & 0x7FFFFF;
    cout << sign << " " 
         << bitset<8>(exponent) << " " 
         << bitset<23>(fraction) << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "usage:" << endl;
        cout << "      ./fp_overflow_checker loop_bound loop_counter" << endl << endl;
        cout << "      loop_bound is a positive floating-point value" << endl;
        cout << "      loop_counter is a positive floating-point value" << endl;
        return 0;
    }

    float loop_bound = atof(argv[1]);
    float loop_counter = atof(argv[2]);

    cout << "Loop bound:   "; printIEEEBits(loop_bound);
    cout << "Loop counter: "; printIEEEBits(loop_counter);
    cout << endl;

    uint32_t bound_bits, counter_bits;
    memcpy(&bound_bits, &loop_bound, sizeof(bound_bits));
    memcpy(&counter_bits, &loop_counter, sizeof(counter_bits));

    int bound_exp = (bound_bits >> 23) & 0xFF;
    int counter_exp = (counter_bits >> 23) & 0xFF;

    if ((bound_exp - counter_exp) >= 24) {
        cout << "Warning: Possible overflow!" << endl;
        cout << "Overflow threshold:" << endl;

        uint32_t thresh_bits = (counter_exp + 24) << 23;
        float threshold;
        memcpy(&threshold, &thresh_bits, sizeof(threshold));

        cout << "  " << scientific << setprecision(5) << threshold << endl;
        cout << "  "; 
        printIEEEBits(threshold);
    } else {
        cout << "There is no overflow!" << endl;
    }

    return 0;
}