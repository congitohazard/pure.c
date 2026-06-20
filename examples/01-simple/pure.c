#include <pure/pure.h>

int main(void) {
    PureCompileJob objects = {
        .tc = NULL,
        .ctx = NULL,
        .paths = {
            (PureLiteral []) { "./main.c" },
            (PureLiteral []) { "./main.o" },
            1
        }
    };

    PureLinkJob exe = {
        .tc = NULL,
        .inputPaths = PURE_CONSTRUCT_SLICE(PureLiteral, "./main.c"),
        .output = "./01-simple"
    };

    pure_compile_sources(&objects);
    pure_link_sources(&exe);
}