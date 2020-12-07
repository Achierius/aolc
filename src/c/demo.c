/* Proof that we can compile this thing correctly :) */

int main(int argc, char *argv[]) {
    extern void asm_hello_world();
    asm_hello_world();
}
