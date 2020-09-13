// Python specific code

// ----- pythoncode -----

%pythoncode %{

# handle typedefs which swig doesn't wrap
ofPoint = ofVec3f
OF_PRIMITIVE_TRIANGLE_STRIP = None

# renaming log -> ofLog
ofLog = log
del log

%}