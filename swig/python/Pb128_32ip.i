%module Pb128_32ip
%{
  #include "../../headers/types/Pb128_32ip.h"
  #include "../../headers/types/Pb64_32p.h"
  using namespace Postbinary;
%}

%import "Pb64_32p.i"

%apply float *INPUT {float*};
%apply float& OUTPUT { float& };

%newobject Postbinary::Pb128_32ip::getLeft;
%newobject Postbinary::Pb128_32ip::getRight;

%include "../../headers/types/Pb128_32ip.h"

%extend Postbinary::Pb128_32ip {
    Pb128_32ip(float number1, float number2, float number3, float number4) {
        return new Postbinary::Pb128_32ip(number1, number2, number3, number4);
    }
}
