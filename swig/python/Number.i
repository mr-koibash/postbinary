%module Number
%{
  #include "../../headers/abstracts/Number.h"
  using namespace Postbinary;
  using namespace Postbinary::Abstracts;
%}

%include "std_string.i"


%include "typemaps.i"

// interprete <bytes> as chars
%typemap(in) (char *source) {
    $1 = PyBytes_AsString($input);
}

//---------------
// return chars of number as <bytes>
%typemap(in, numinputs=0) (char **begin, int& size) (char *temp1, int temp2) {
   $1 = &temp1;
   $2 = &temp2;
}
%typemap(argout) (char **begin, int& size) {
    $result = PyBytes_FromStringAndSize(*$1, *$2);
}
%include "../../headers/abstracts/Number.h"
