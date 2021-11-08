%module Pb64_32p
%{
  #include "../../headers/Types/Pb64_32p.h"
  using namespace Postbinary;
  
%}





%apply float *INPUT {float*};
%apply float& OUTPUT { float& };

%include "../../headers/Types/Pb64_32p.h"




