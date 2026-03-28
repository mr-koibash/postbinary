#pragma once

namespace Postbinary {
    class Package;
    class Pb64_32p;
    class Pb128_32ip;
}

namespace Postbinary {
    namespace Utilities {

        class Compressor {
        public:
            static const unsigned int TETRITS_IN_BYTE = 4; 

            static Postbinary::Package* compress(Postbinary::Pb64_32p& number, unsigned int K);
            static Postbinary::Package* compress(Postbinary::Pb128_32ip& number, unsigned int K);
            
            static Postbinary::Pb64_32p* decompressAsPb64(Postbinary::Package& package);
            static Postbinary::Pb128_32ip* decompressAsPb128(Postbinary::Package& package);
        };

    }
}
