%module Pb128_32ip
%{
  #include "../../headers/types/Pb128_32ip.h"
  #include "../../headers/types/Pb64_32p.h"
  using namespace Postbinary;
%}

%apply float *INPUT {float*};
%apply float& OUTPUT { float& };

%include "../../headers/types/Pb128_32ip.h"
