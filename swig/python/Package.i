%module Package
%{
    #include "../../headers/Constants.h"
    #include "../../headers/abstracts/Number.h"
    #include "../../headers/types/Package.h"

using namespace Postbinary;
using namespace Postbinary::Abstracts;
using namespace Postbinary::Constants;
%}

%include "../../headers/Constants.h"
%import "Number.i"
%include "../../headers/types/Package.h"
