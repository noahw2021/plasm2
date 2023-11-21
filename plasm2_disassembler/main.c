//
//  main.c
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "basetypes.h"
#include "decoder/decoder.h"

int main(int argc, char** argv) {
    printf("PLASM2 Disassembler\n");
    char* InputName = malloc(128);
    char* OutputName = malloc(128);
    char* SymbolsName = malloc(128);
    FILE* InputFile = NULL, *OutputFile = NULL, *SymbolFile = NULL;
    
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            char* ThisArg = argv[i];
            
            if (!strcmp(ThisArg, "-h")) {
                printf("PLASM2 Disassembler Help\n");
                printf("%s -h : Shows this page.\n", argv[0]);
                printf("%s -i=[InFile] : Disassembles a file to stdout.\n", argv[0]);
                printf("%s -i=[InFile] -o=[OutFile] : Disassembles a file to text file.\n", argv[0]);
                printf("%s -i=[InFile] -o=[OutFile] -s=[Symbols] : Disassembles a file to text file file with symbols.\n", argv[0]);
                printf("%s -i=[InFile] -o=stdout -s=[Symbols] : Disassembles a file to stdout with symbols.\n\n", argv[0]);
            
                free(InputName);
                free(OutputName);
                free(SymbolsName);
                return 0;
            }
            
            // Argument Finder
            
            if (!strcmp(ThisArg, "-i=")) { // input
                char* ArgumentBase = ThisArg + 0x3;
                strcpy(InputName, ArgumentBase);
                
                InputFile = fopen(InputName, "rb");
                if (!InputFile) {
                    printf("[ERROR]: Failed to open input file.\n");
                    free(InputName);
                    free(OutputName);
                    free(SymbolsName);
                    if (OutputFile)
                        fclose(OutputFile);
                    if (SymbolFile)
                        fclose(SymbolFile);
            
                    return -1;
                }
            }
            
            if (!strcmp(ThisArg, "-o=")) { // output
                char* ArgumentBase = ThisArg + 0x3;
                strcpy(OutputName, ArgumentBase);
                
                OutputFile = fopen(OutputName, "wb");
                if (!OutputFile) {
                    printf("[ERROR]: Failed to open output file.\n");
                    free(InputName);
                    free(OutputName);
                    free(SymbolsName);
                    if (InputFile)
                        fclose(InputFile);
                    if (SymbolFile)
                        fclose(SymbolFile);
                    
                    return -1;
                }
            }
            
            if (!strcmp(ThisArg, "-s=")) { // symbols
                char* ArgumentBase = ThisArg + 0x3;
                strcpy(SymbolsName, ArgumentBase);
                
                SymbolFile = fopen(SymbolsName, "rb");
                if (!SymbolFile) {
                    printf("[ERROR]: Failed to open symbols file.\n");
                    free(InputName);
                    free(OutputName);
                    free(SymbolsName);
                    if (InputFile)
                        fclose(InputFile);
                    if (OutputFile)
                        fclose(OutputFile);
                }
            }
        }
    }
    
    free(InputName);
    free(OutputName);
    free(SymbolsName);
    
    if (!InputFile) {
        printf("[ERROR]: No input file found.\n");
        if (OutputFile)
            fclose(OutputFile);
        if (SymbolFile)
            fclose(SymbolFile);
        
        return -1;
    }
    
    if (!OutputFile)
        OutputFile = stdout;
    
    fseek(InputFile, 0, SEEK_END);
    WORD32 FileLength = ftell(InputFile);
    fseek(InputFile, 0, SEEK_SET);
    
    BYTE* Data = malloc(FileLength);
    fread(Data, FileLength, 1, InputFile);
    fclose(InputFile);
    
    DecoderInit();
    DecoderSetData(Data, FileLength);
    DecoderSetOutputFile(OutputFile);
    if (SymbolFile)
        DecoderSetSymbolFile(SymbolFile);
    
    while (DecoderGo());
    
    printf("[INFO]: Program completed.\n");
    return 0;
}
