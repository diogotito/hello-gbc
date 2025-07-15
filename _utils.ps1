function new_c_module([string] $module_name) {
    $headerGuard = "__$($module_name.ToUpper())_H"
    Push-Location "$(git rev-parse --show-toplevel)/src"
    try {
@"
#ifndef $headerGuard
#define $headerGuard

// TODO

#endif
"@ | Out-File -Encoding utf8NoBOM -NoClobber ($module_name + ".h")

@"
#include "${module_name}.h"

// TODO

"@ | Out-File -Encoding utf8NoBOM -NoClobber ($module_name + ".c")
    
        git add ($module_name + ".h") ($module_name + ".c")
        code ($module_name + ".h") ($module_name + ".c")
    } finally {
        Pop-Location
    }

}
