#include "d16.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Web sunucusu için port
#define HTTP_PORT 8000

// --- Fonksiyonlar ---

// CPUnun mevcut durumunu bir JSON dosyasına yazar
void cpu_export_state_to_json(D16_CPU* cpu) {
    FILE* json_file = fopen("d16_state.json", "w");
    if (json_file == NULL) { 
        perror("JSON durum dosyası oluşturulamadı");
        return; 
    }
    fprintf(json_file, "{\n");
    fprintf(json_file, "  \"pc\": %d,\n", cpu->pc);
    fprintf(json_file, "  \"zf\": %d,\n", cpu->zf);
    fprintf(json_file, "  \"A\": %d,\n", cpu->A);
    fprintf(json_file, "  \"B\": %d,\n", cpu->B);
    fprintf(json_file, "  \"C\": %d,\n", cpu->C);
    fprintf(json_file, "  \"D\": %d,\n", cpu->D);
    fprintf(json_file, "  \"halted\": %s,\n", cpu->halt ? "true" : "false");
    fprintf(json_file, "  \"ram\": [\n    ");
    for (int i = 0; i < RAM_SIZE; i++) {
        fprintf(json_file, "%d%s", cpu->ram[i], (i < RAM_SIZE - 1) ? ", " : "");
        if ((i + 1) % 16 == 0 && i < RAM_SIZE - 1) { fprintf(json_file, "\n    "); }
    }
    fprintf(json_file, "\n  ]\n}\n");
    fclose(json_file);
}

// Register adını (char) register pointerına çevirir
uint8_t* get_register(D16_CPU* cpu, char name) {
    switch (name) {
        case 'A': return &cpu->A; 
        case 'B': return &cpu->B;
        case 'C': return &cpu->C; 
        case 'D': return &cpu->D;
        default: return NULL;
    }
}

// CPUyu başlangıç ilteki durumuna getirir
void cpu_init(D16_CPU* cpu) {
    cpu->A = 0; cpu->B = 0; cpu->C = 0; cpu->D = 0;
    cpu->pc = 0; cpu->zf = 0; cpu->halt = 0;
    memset(cpu->ram, 0, RAM_SIZE * sizeof(uint8_t));
}

// CPU durumunu terminale basar
void cpu_dump(D16_CPU* cpu) {
    printf("PC: %02X | ZF: %d | A: %d | B: %d\n", cpu->pc, cpu->zf, cpu->A, cpu->B);
}

// Program dosyasını okuyup RAMe yükleme
int load_program(D16_CPU* cpu, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) { 
        perror("Program dosyası açılamadı"); 
        return -1; 
    }
    char line[256];
    int address = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == ';' || line[0] == '\n' || line[0] == '\r') continue;
        char command[10] = {0}, arg1_str[10] = {0}, arg2_str[10] = {0};
        sscanf(line, "%s %[^,], %s", command, arg1_str, arg2_str);
        
        if (strcmp(command, "LOAD") == 0) { cpu->ram[address++] = LOAD_OP; cpu->ram[address++] = arg1_str[0]; cpu->ram[address++] = (uint8_t)atoi(arg2_str); }
        else if (strcmp(command, "STORE") == 0) { cpu->ram[address++] = STORE_OP; cpu->ram[address++] = arg1_str[0]; cpu->ram[address++] = (uint8_t)atoi(arg2_str); }
        else if (strcmp(command, "ADD") == 0) { cpu->ram[address++] = ADD_OP; cpu->ram[address++] = arg1_str[0]; cpu->ram[address++] = arg2_str[0]; }
        else if (strcmp(command, "JMP") == 0) { cpu->ram[address++] = JMP_OP; cpu->ram[address++] = (uint8_t)atoi(arg1_str); cpu->ram[address++] = 0; }
        else if (strcmp(command, "HALT") == 0) { cpu->ram[address++] = HALT_OP; cpu->ram[address++] = 0; cpu->ram[address++] = 0; } 
        else if (strcmp(command, "CMP") == 0) { cpu->ram[address++] = CMP_OP; cpu->ram[address++] = arg1_str[0]; cpu->ram[address++] = arg2_str[0]; }
        else if (strcmp(command, "JNZ") == 0) { cpu->ram[address++] = JNZ_OP; cpu->ram[address++] = (uint8_t)atoi(arg1_str); cpu->ram[address++] = 0; }
        else if (strcmp(command, "IN") == 0) { cpu->ram[address++] = IN_OP; cpu->ram[address++] = arg1_str[0]; cpu->ram[address++] = 0; }
        else if (strcmp(command, "OUT") == 0) { cpu->ram[address++] = OUT_OP; cpu->ram[address++] = arg1_str[0]; cpu->ram[address++] = 0; }
        else if (strcmp(command, "SUB") == 0) { cpu->ram[address++] = SUB_OP; cpu->ram[address++] = arg1_str[0]; cpu->ram[address++] = arg2_str[0]; }
    }
    fclose(file);
    printf("Program '%s' başarıyla RAM'e yüklendi.\n", filename);
    return 0;
}

// CPU döngüsü
void cpu_run(D16_CPU* cpu) {
    cpu_export_state_to_json(cpu);
    while (!cpu->halt) {
        cpu_dump(cpu);
        uint8_t opcode = cpu->ram[cpu->pc];
        uint8_t arg1 = cpu->ram[cpu->pc + 1];
        uint8_t arg2 = cpu->ram[cpu->pc + 2];
        int pc_increment = 3;

        switch (opcode) {
            case HALT_OP: 
                cpu->halt = 1; 
                pc_increment = 0; 
                break;
            case LOAD_OP: { 
                uint8_t* r = get_register(cpu, (char)arg1); 
                if (r) *r = arg2; 
                break; 
            }
            case STORE_OP: { 
                uint8_t* r = get_register(cpu, (char)arg1); 
                if (r) cpu->ram[arg2] = *r; 
                break; 
            }
            case ADD_OP: { 
                uint8_t* r1 = get_register(cpu, (char)arg1); 
                uint8_t* r2 = get_register(cpu, (char)arg2); 
                if (r1 && r2) *r1 += *r2; 
                break; 
            }
            case SUB_OP: { 
                uint8_t* r1 = get_register(cpu, (char)arg1); 
                uint8_t* r2 = get_register(cpu, (char)arg2); 
                if (r1 && r2) *r1 -= *r2; 
                break; 
            }
            case JMP_OP: { 
                cpu->pc = arg1; 
                pc_increment = 0; 
                break; 
            }
            case CMP_OP: { 
                uint8_t* r1 = get_register(cpu, (char)arg1); 
                uint8_t* r2 = get_register(cpu, (char)arg2); 
                if (r1 && r2) cpu->zf = (*r1 == *r2); 
                break; 
            }
            case JNZ_OP: { 
                if (cpu->zf == 0) { 
                    cpu->pc = arg1; 
                    pc_increment = 0; 
                } 
                break; 
            }
            case IN_OP: { 
                uint8_t* r = get_register(cpu, (char)arg1); 
                if (r) { 
                    printf("Input (0-255) for %c: ", (char)arg1); 
                    unsigned int v; 
                    scanf("%u", &v); 
                    *r = (uint8_t)v; 
                } 
                break; 
            }
            case OUT_OP: { 
                uint8_t* r = get_register(cpu, (char)arg1); 
                if (r) printf(">>> Output from %c: %d\n", (char)arg1, *r); 
                break; 
            }
            default: 
                printf("Bilinmeyen komut: %d\n", opcode); 
                cpu->halt = 1; 
                break;
        }
        
        if (pc_increment > 0) {
            cpu->pc += pc_increment;
        }
        
        cpu_export_state_to_json(cpu);
        usleep(100000); // 100ms bekleme
    }

    printf("\n--- Program Tamamlandı ---\n");
    cpu_dump(cpu);
    cpu_export_state_to_json(cpu);
}

// Ana fonksiyon
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Kullanım: %s <program_dosyasi.txt>\n", argv[0]);
        return 1;
    }

    // Sisteme göre web sunucusunu başlatan komut
    char server_command[256];
    #ifdef _WIN32
        // Windows için: Yeni bir konsol açmadan arka planda çalıştır
        sprintf(server_command, "start /B python -m http.server %d", HTTP_PORT);
    #else
        // Linux ve macOS için: Arka planda çalıştırır ve çıktısını gizle (> /dev/null 2>&1)
        sprintf(server_command, "python -m http.server %d > /dev/null 2>&1 &", HTTP_PORT);
    #endif

    printf("D16 CPU başlatılıyor...\n");
    printf("Arka planda web sunucusu başlatılıyor...\n");
    system(server_command);

    // Sunucunun başlaması için kısa bir süre bekleme
    sleep(1); 

    printf("\n>>> BAŞARILI! <<<\n");
    printf("GUI Monitörü için tarayıcınızda şu adresi açın:\n");
    printf("http://localhost:%d/d16_monitor.html\n\n", HTTP_PORT);
    printf("--------------------------------------------------\n");
    printf("CPU Simülasyonu Başlıyor:\n\n");

    D16_CPU my_cpu;
    cpu_init(&my_cpu);

    if (load_program(&my_cpu, argv[1]) != 0) {
        return 1;
    }

    cpu_run(&my_cpu);

    printf("\nÖNEMLİ: Simülasyon bitti ancak web sunucusu arka planda çalışmaya devam edebilir.\n");
    printf("Durdurmak için 'pkill -f http.server' (Linux/macOS) veya Görev Yöneticisi'ni (Windows) kullanabilirsiniz.\n");

    return 0;
}