#ifndef D16_H
#define D16_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // uint8_t gibi standart tamsayı türleri için

// Bellek boyutunu tanımla
#define RAM_SIZE 256

// --- Opcode'lar ---
#define HALT_OP 0
#define LOAD_OP 1
#define STORE_OP 2
#define ADD_OP 3
#define JMP_OP 4
#define CMP_OP 5
#define JNZ_OP 6
#define IN_OP 7
#define OUT_OP 8
#define SUB_OP 9


// CPUnun yapısı
typedef struct {
    // Genel amaçlı registerlar
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;

    // Özel registerlar
    uint8_t pc; // Program Counter (Program Sayacı)
    
    // Durum Bayrağı (Status Flag)
    uint8_t zf; // Zero Flag (1 ise son CMP eşit 0 ise değil)

    // Halt (Durdurma) bayrağı
    int halt;

    // RAM
    uint8_t ram[RAM_SIZE];

} D16_CPU;

// --- Fonksiyon Bildirimleri (Prototipler) ---
// Fonksiyonların header'larının .c dosyasında olduğunu söylerler.

uint8_t* get_register(D16_CPU* cpu, char name);
void cpu_init(D16_CPU* cpu);
void cpu_dump(D16_CPU* cpu);
int load_program(D16_CPU* cpu, const char* filename);
void cpu_run(D16_CPU* cpu);

#endif // D16_H
