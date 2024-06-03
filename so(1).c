#include <windows.h>
#include <stdio.h>

/* Comentários do aluno */
//Comentários do livro.

/*O Código abaixo daqui foi tirado do livro "Fundamentos de Sistemas Operacionais. Capitulo 9 - Mémoria virtual' figura 9.25" O mesmo serve apenas para ilustrar como através de uma API do windows acessar uma memória que está mapeada e ler o conteudo dessa memória compartilhada.*/

int main()
{
    HANDLE hMapFile; /*A variável do tipo HANDLE servirá apenas para que se possa armazenar o arquivo a ser manipulado, ela é importante para o mapeamento da memoria compartilhada, lembrando que o tipo HANDLE é um ponteiro, que exibe informações em qualquer processo do sistema. */
    
    LPVOID lpMapAddress; /*Essa variáfvel vai guardar o endereço da memória que foi mapeada*/
    char message[] = "Hello, World!";

    // Cria um objeto de arquivo mapeado
    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE, // usa a memória do sistema de paginação
        NULL,                 // atributos de segurança padrão
        PAGE_READWRITE,       // permissões de leitura/gravação
        0,                    // tamanho máximo (parte alta)
        256,                  // tamanho máximo (parte baixa)
        TEXT("SharedObject")  // nome do objeto de arquivo mapeado
    );

    if (hMapFile == NULL)
    /*Essa parte do código verifica o objeto de arquivo que mapeado, se ele foi criado de forma certa. A função CreateFileMapping retornará um NULL se ocorrer um problema e irá imprimir uma mensagem de erro com o código do erro (GetLastError()) Além de terminar o programa com return 1. Sem essa verificação, o programa continuaria o mapeamento de memória, gerando diversos erros.*/
    {
        printf("Could not create file mapping object (%d).\n", GetLastError());
        return 1;
    }

    // Mapeia uma visão do arquivo na memória
    lpMapAddress = MapViewOfFile(
        hMapFile,            // manipulador do objeto mapeado
        FILE_MAP_ALL_ACCESS, // acesso de leitura/gravação
        0,                   // visão mapeada do arquivo inteiro (offset alto)
        0,                   // visão mapeada do arquivo inteiro (offset baixo)
        0                    // mapeia o arquivo inteiro
    );

    if (lpMapAddress == NULL)
    /*Essa parte serve para saber se a visão que está acima foi mapeada com sucesso, Caso não, será retornado uma mensagem de erro com o código do erro (GetLastError()) */
    {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    // Escreve na memória compartilhada
    CopyMemory(lpMapAddress, message, sizeof(message));

    printf("Message written to shared memory: %s\n", message);

    // Desmapeia a visão do arquivo da memória
    UnmapViewOfFile(lpMapAddress);

    // Fecha o manipulador do objeto de arquivo mapeado
    CloseHandle(hMapFile);

    return 0;
}
