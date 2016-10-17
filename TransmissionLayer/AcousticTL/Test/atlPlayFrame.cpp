#include "../AcousticTL.h"
#include <vector>

using namespace std;

int main(){
    AcousticTL atl(44100, 20000, 2000);
    atl.sendFrame({0x12, 0x34, 0x45, 0x78, 0x9A});
    cin.get();
    return 0;
}