#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

#define INT_TYPE        1
#define BYTE_TYPE       2
#define FLOAT_TYPE      3
#define STRING_TYPE     4

#define LOAD_TYPE       10
#define CALL_TYPE       11

#define NEXT_ARG        100
#define EXEC_INST       101
#define FUNC_FOLLOWS    102
#define FUNC_END        103

#define STORE_INST      1
#define PRINT_INST      2
#define EXIT_INST       3

#define byte uint8_t

int32_t bytesToInt(std::vector<byte> bytes) {
    return (  (int32_t)((bytes[0]) << 24)
            | (int32_t)((bytes[1]) << 16)
            | (int32_t)((bytes[2]) << 8)
            | (int32_t) (bytes[3]));
}

_Float32 bytesToFloat(std::vector<byte> bytes) {
    int tmp = bytesToInt(bytes);
    return *((_Float32*) &tmp);
}

class ByteCodeExecutor {
private:
    std::vector<byte> code;
    std::map<int, void*> vars;
    std::map<int, std::vector<byte>> funcs;

    void varStore(int ref, int32_t val) {
        int32_t *ptr = (int32_t*) malloc(sizeof(int32_t));
        *ptr = val;
        vars[ref] = ptr;
    }

    void varStore(int ref, byte val) {
        byte *ptr = (byte*) malloc(sizeof(byte));
        *ptr = val;
        vars[ref] = ptr;
    }

    void varStore(int ref, _Float32 val) {
        _Float32 *ptr = (_Float32*) malloc(sizeof(_Float32));
        *ptr = val;
        vars[ref] = ptr;
    }

    void* varLoad(int ref) {
        return vars[ref];
    }
public:
    ByteCodeExecutor(std::vector<byte> code) : code(code) {}

    void execInst(byte inst, std::vector<byte> args) {
        switch(inst) {
        case STORE_INST: {
            int ref = bytesToInt(std::vector<byte>(&args[1], &args[5]));
            switch(args[5]) {
            case INT_TYPE:
                varStore(ref, bytesToInt(std::vector<byte>(&args[6], &args[10])));
                break;
            }
        } break;
        case EXIT_INST: {
            switch(args[0]) {
            case BYTE_TYPE:
                exit(args[1]);
                break;
            case INT_TYPE:
                exit(bytesToInt(std::vector<byte>(&args[1], &args[5])));
                break;
            case FLOAT_TYPE:
                break;
            }
        } break; // ← this is intentional
        case PRINT_INST: {
            switch(args[0]) {
            case INT_TYPE:
                std::cout << bytesToInt(std::vector<byte>(&args[1], &args[5]));
                break;
            }
            break;
        } break;
        }
    }

    void execByteCode() {
        byte tmpInst = code[0];
        size_t tmpIndex = 1;
        for (size_t i = 1; i < code.size(); i++) {
            if (code[i] == EXEC_INST) {
                execInst(tmpInst, std::vector<byte>(&code[tmpIndex], &code[i]));
                tmpInst = code[++i];
                tmpIndex = ++i;
            } else if (code[i] == FUNC_FOLLOWS) {
                tmpIndex = ++i;
                int funcRef = code[i];
                for (size_t i1 = i++; i1 < code.size(); i1++) {
                    if (code[i1] == FUNC_END) {
                        funcs[funcRef] = std::vector<byte>(&code[tmpIndex], &code[i1]);
                        i = i1 + 1;
                        break;
                    }
                }
            }
        }
        vars = std::map<int, void*>();
    }
};

int main() {
    ByteCodeExecutor bce({PRINT_INST, INT_TYPE, 0, 0, 0, 4, EXEC_INST, EXIT_INST, BYTE_TYPE, 255, EXEC_INST});
    bce.execByteCode();
}
