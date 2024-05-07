// fopen, fclose, fwrite, scanf
#include <stdio.h>
// srand, rand, NULL, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// time
#include <time.h>
// bool, true, false
#include <stdbool.h>

// defino o tamanho máximo dos atributos da struct, além dos caminhos para o database e o database_temp pra fazer operações de update e delete
#define MAX_SIZE 50
#define DATABASE "users.dat"
#define DATABASE_TEMP "temp.dat"

// functions prototypes, permite usar as funções antes de serem declaradas
void createUser();
void listAllUsers();
void updateUser();
void deleteUser();

// struct para armazenar as informações do usuário, como username e email
struct User {
    int id;
    char username[MAX_SIZE];
    char email[MAX_SIZE];
};

// função principal
int main() {
    printf("### SIMPLE C CRUD ###\n\n");

    // declaração de variável booleana para deixar o while mais elegante
    bool runningProgram = true;

    while (runningProgram) {
        // opção em int, pois o scanf já formata para inteiro de acordo com o formatador usado
        int option;

        printf("Operations available:\n");
        printf("1. Create User\n");
        printf("2. List All Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("0. Exit\n\n");

        printf("Choose an option: > ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                createUser();
                break;
            case 2:
                listAllUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 0:
                printf("Exiting...\n");
                runningProgram = false;
                break;
            default:
                printf("\nInvalid option!\n\n");
        }
    }

    // ao invés de retornar 0, retornamos a macro
    return EXIT_SUCCESS;
}

void createUser() {
    printf("\n### CREATING A NEW USER ###\n\n");

    // abrir o arquivo de database, como binário, para append
    FILE *fileHandler = fopen(DATABASE, "ab");

    // verificar se o arquivo existe
    if (fileHandler == NULL) {
        printf("\nFailed to open file!\n\n");
        return;
    }

    // declarar a struct para armazenar os valores
    struct User user;

    // coletar as informações dos usuários e armazenar na struct
    printf("Digit the user's username: > ");
    scanf("%s", user.username);

    printf("Digit the user's email: > ");
    scanf("%s", user.email);

    // semeando a função rand com o horário atual para ter valores pseudo aleatórios
    srand(time(NULL));

    // gerando um id aleatório
    user.id = rand() % 9999;

    // escrever os bytes da struct no arquivo
    fwrite(&user, sizeof(struct User), 1, fileHandler);

    printf("\nUser created successfully!\n\n");

    // fechar o arquivo
    fclose(fileHandler);
}

void listAllUsers() {
    printf("\n### LISTING ALL USERS ###\n\n");

    // abrir o arquivo de database, como binário, para leitura
    FILE *fileHandler = fopen(DATABASE, "rb");

    // verificar se o arquivo existe
    if (fileHandler == NULL) {
        printf("Failed to open file!\n\n");
        return;
    }

    // declarar a struct para armazenar os valores
    struct User user;

    // fazer a leitura do arquivo, enquanto tiver valores para ler, armazenando na struct os valores que estão no arquivo
    while (fread(&user, sizeof(user), 1, fileHandler) == 1) {
        // printando na tela os arquivos
        printf("%d - %s - %s\n", user.id, user.username, user.email);
    }

    printf("\n");

    // fechando o arquivo
    fclose(fileHandler);
}

void updateUser() {
    printf("\n### UPDATING A USER ###\n\n");

    // abrir o arquivo de database, como binário, para leitura
    FILE *fileHandler = fopen(DATABASE, "rb");

    // verificar se o arquivo existe
    if (fileHandler == NULL) {
        printf("\nFailed to open file!\n");
    }

    // declarar variável para armazenar o id do usuário
    int id;

    // coletar valor do id do usuário a ser alterado e armazenar na variável
    printf("Digit the user's ID to update: > ");
    // lembrar que scanf precisa de um ponteiro
    scanf("%d", &id);

    // declaração de struct para armazenar os valores lidos
    struct User user;
    // declaração de variável booleana para indicar se o usuário foi encontrado, será utilizado abaixo
    bool userFound = false;
    // abrir o arquivo de database temporário, como binário, para escrita
    FILE *fileHandlerTemp = fopen(DATABASE_TEMP, "wb");

    // ler do database principal os dados e gravando na struct enquanto for 1
    while (fread(&user, sizeof(user), 1, fileHandler) == 1) {
        // verificando se o id passado do usuário é o mesmo da struct lida no loop, se for:
        if (user.id == id) {
            // declaramos uma nova struct, somente pra não misturar as coisas
            struct User updatedUser;

            // pedimos para o usuário o novo username e armazenamos na nova struct
            printf("Digit the new username: > ");
            scanf("%s", updatedUser.username);

            // pedimos para o usuário o novo email e armazenamos na nova struct
            printf("Digit the new email: > ");
            scanf("%s", updatedUser.email);

            // atribuimos o valor do id antigo do usuário na nova struct
            updatedUser.id = user.id;

            // escrevemos o novo usuário no database temporário
            fwrite(&updatedUser, sizeof(updatedUser), 1, fileHandlerTemp);

            // declaramos a variável do usuário encontrado como true
            userFound = true;
            // pedimos pra continuar o laço
            continue;
        }

        // se não for igual os ids, escrevemos o usuário no database temporário
        // isso irá reescrever todos os usuários no database temporário
        fwrite(&user, sizeof(user), 1, fileHandlerTemp);
    }

    // se o usuário for encontrado, declaramos na tela que foi alterado, se não, também informamos que não foi encontrado
    if (userFound) {
        printf("\nUser updated successfully!\n\n");
    } else {
        printf("\nUser not found!\n\n");
    }

    // fechamos os dois arquivos de database
    fclose(fileHandler);
    fclose(fileHandlerTemp);

    // removemos o database principal, pois já copiamos todos os usuários para o database temporário
    remove(DATABASE);
    // renomeamos o database temporário para o database principal
    rename(DATABASE_TEMP, DATABASE);
}

void deleteUser() {
    printf("\n### DELETING A USER ###\n\n");

    // abrir o arquivo de database, como binário, para leitura
    FILE *fileHandler = fopen(DATABASE, "rb");

    // verificar se o arquivo existe
    if (fileHandler == NULL) {
        printf("\nFailed to open file!\n");
        return;
    }

    // declarar variável para armazenar o id do usuário
    int id;

    // coletar valor do id do usuário a ser alterado e armazenar na variável
    printf("Digit the user ID to delete: > ");
    // lembrar que scanf precisa de um ponteiro
    scanf("%d", &id);

    // declaração de struct para armazenar os valores lidos
    struct User user;
    // declaração de variável booleana para indicar se o usuário foi encontrado, será utilizado abaixo
    bool userFound = false;
    // abrir o arquivo de database temporário, como binário, para escrita
    FILE *fileHandlerTemp = fopen(DATABASE_TEMP, "wb");

    // ler do database principal os dados e gravando na struct enquanto for 1
    while (fread(&user, sizeof(user), 1, fileHandler) == 1) {
        // verificando se o id passado do usuário é o mesmo da struct lida no loop, se for:
        if (user.id == id) {
            // declaramos a variável do usuário encontrado como true
            userFound = true;
            // pedimos pra continuar o loop
            // essa etapa é importante, pois não adicionamos esse usuário ao database temporário, pois é o usuário que queremos excluir
            continue;
        }

        // se não for igual os ids, escrevemos o usuário no database temporário
        // isso irá reescrever todos os usuários no database temporário
        fwrite(&user, sizeof(user), 1, fileHandlerTemp);
    }

    // se o usuário for encontrado, declaramos na tela que foi removido, se não, também informamos que não foi encontrado
    if (userFound) {
        printf("\nUser deleted successfully!\n\n");
    } else {
        printf("\nUser not found!\n\n");
    }

    // fechamos os dois arquivos de database
    fclose(fileHandler);
    fclose(fileHandlerTemp);

    // removemos o database principal, pois já copiamos todos os usuários para o database temporário
    remove(DATABASE);
    // renomeamos o database temporário para o database principal
    rename(DATABASE_TEMP, DATABASE);
}