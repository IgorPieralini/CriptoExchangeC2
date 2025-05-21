#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 100
#define TAM 50

// --- Estruturas ---
typedef struct {
    char login[TAM];
    char senha[TAM];
    char tipo[TAM]; // admin ou user
} Usuario;

typedef struct {
    char nome[TAM];
    float cotacao;
    float percCompra;
    float percVenda;
} Criptomoeda;

Usuario usuarios[MAX_ITENS];
Criptomoeda criptos[MAX_ITENS];
int totalUsuarios = 0, totalCriptos = 0;

// --- Utils ---
int buscarUsuario(char *login) {
    for (int i = 0; i < totalUsuarios; i++)
        if (strcmp(usuarios[i].login, login) == 0) return i;
    return -1;
}

int buscarCripto(char *nome) {
    for (int i = 0; i < totalCriptos; i++)
        if (strcmp(criptos[i].nome, nome) == 0) return i;
    return -1;
}

// --- Usuários ---
void carregarUsuarios() {
    FILE *f = fopen("usuarios.txt", "r");
    if (!f) {
        f = fopen("usuarios.txt", "w");
        fprintf(f, "admin;1234;admin\n");
        fclose(f);
        f = fopen("usuarios.txt", "r");
    }
    totalUsuarios = 0;
    while (fscanf(f, "%[^;];%[^;];%[^\n]\n", usuarios[totalUsuarios].login, usuarios[totalUsuarios].senha, usuarios[totalUsuarios].tipo) == 3) {
        totalUsuarios++;
    }
    fclose(f);
}

void salvarUsuarios() {
    FILE *f = fopen("usuarios.txt", "w");
    for (int i = 0; i < totalUsuarios; i++)
        fprintf(f, "%s;%s;%s\n", usuarios[i].login, usuarios[i].senha, usuarios[i].tipo);
    fclose(f);
}

void cadastrarUsuario() {
    if (totalUsuarios >= MAX_ITENS) return;
    Usuario u;
    printf("Login: "); scanf("%s", u.login);
    if (buscarUsuario(u.login) != -1) {
        printf("Usuario ja existe.\n"); return;
    }
    printf("Senha: "); scanf("%s", u.senha);
    strcpy(u.tipo, "user");
    usuarios[totalUsuarios++] = u;
    salvarUsuarios();
    printf("Usuario cadastrado.\n");
}

void removerUsuario() {
    char login[TAM];
    printf("Login para remover: "); scanf("%s", login);
    int idx = buscarUsuario(login);
    if (idx == -1 || strcmp(usuarios[idx].tipo, "admin") == 0) {
        printf("Nao removido.\n"); return;
    }
    for (int i = idx; i < totalUsuarios - 1; i++)
        usuarios[i] = usuarios[i + 1];
    totalUsuarios--;
    salvarUsuarios();
    printf("Usuario removido.\n");
}

// --- Criptomoedas ---
void carregarCriptos() {
    FILE *f = fopen("cripto.txt", "r");
    totalCriptos = 0;
    if (!f) return;
    while (fscanf(f, "%[^;];%f;%f;%f\n", criptos[totalCriptos].nome, &criptos[totalCriptos].cotacao,
                  &criptos[totalCriptos].percCompra, &criptos[totalCriptos].percVenda) == 4)
        totalCriptos++;
    fclose(f);
}

void salvarCriptos() {
    FILE *f = fopen("cripto.txt", "w");
    for (int i = 0; i < totalCriptos; i++)
        fprintf(f, "%s;%.2f;%.2f;%.2f\n", criptos[i].nome, criptos[i].cotacao,
                criptos[i].percCompra, criptos[i].percVenda);
    fclose(f);
}

void cadastrarCripto() {
    if (totalCriptos >= MAX_ITENS) return;
    Criptomoeda c;
    printf("Nome: "); scanf("%s", c.nome);
    if (buscarCripto(c.nome) != -1) {
        printf("Ja existe.\n"); return;
    }
    printf("Cotacao: "); scanf("%f", &c.cotacao);
    printf("%%Compra: "); scanf("%f", &c.percCompra);
    printf("%%Venda: "); scanf("%f", &c.percVenda);
    criptos[totalCriptos++] = c;
    salvarCriptos();
    printf("Criptomoeda cadastrada.\n");
}

void removerCripto() {
    char nome[TAM];
    printf("Nome: "); scanf("%s", nome);
    int idx = buscarCripto(nome);
    if (idx == -1) return;
    for (int i = idx; i < totalCriptos - 1; i++)
        criptos[i] = criptos[i + 1];
    totalCriptos--;
    salvarCriptos();
    printf("Removida.\n");
}

void atualizarCotacoes() {
    srand(time(NULL));
    for (int i = 0; i < totalCriptos; i++) {
        float var = ((rand() % 401) / 100.0f) - 2.0f;
        criptos[i].cotacao += var;
        if (criptos[i].cotacao < 0) criptos[i].cotacao = 0;
    }
    salvarCriptos();
    printf("Cotacoes atualizadas.\n");
}

// --- Saldo ---
float obterSaldo(char *login) {
    FILE *f = fopen("saldos.txt", "r");
    char l[TAM]; float s;
    if (!f) return 0;
    while (fscanf(f, "%[^;];%f\n", l, &s) == 2)
        if (strcmp(login, l) == 0) {
            fclose(f); return s;
        }
    fclose(f); return 0;
}

void salvarSaldo(char *login, float saldo) {
    FILE *f = fopen("saldos.txt", "r");
    FILE *tmp = fopen("tmp.txt", "w");
    char l[TAM]; float s; int achou = 0;
    if (f) {
        while (fscanf(f, "%[^;];%f\n", l, &s) == 2) {
            if (strcmp(login, l) == 0) {
                fprintf(tmp, "%s;%.2f\n", login, saldo);
                achou = 1;
            } else fprintf(tmp, "%s;%.2f\n", l, s);
        }
        fclose(f);
    }
    if (!achou) fprintf(tmp, "%s;%.2f\n", login, saldo);
    fclose(tmp);
    remove("saldos.txt"); rename("tmp.txt", "saldos.txt");
}

void registrarExtrato(char *login, char *mensagem) {
    char nomeArq[100];
    sprintf(nomeArq, "extrato_%s.txt", login);
    FILE *f = fopen(nomeArq, "a");
    fprintf(f, "%s\n", mensagem);
    fclose(f);
}

void adicionarSaldo(char *login) {
    float valor;
    printf("Valor: "); scanf("%f", &valor);
    if (valor <= 0) return;
    float saldo = obterSaldo(login) + valor;
    salvarSaldo(login, saldo);
    registrarExtrato(login, "Deposito realizado.");
    printf("Saldo atualizado: %.2f\n", saldo);
}

void sacarSaldo(char *login) {
    float valor;
    printf("Valor: "); scanf("%f", &valor);
    float saldo = obterSaldo(login);
    if (valor <= 0 || valor > saldo) {
        printf("Saque invalido.\n"); return;
    }
    saldo -= valor;
    salvarSaldo(login, saldo);
    registrarExtrato(login, "Saque realizado.");
    printf("Saldo atualizado: %.2f\n", saldo);
}

void comprarCripto(char *login) {
    char nome[TAM]; float qtd;
    printf("Cripto: "); scanf("%s", nome);
    int idx = buscarCripto(nome);
    if (idx == -1) return;
    printf("Qtd: "); scanf("%f", &qtd);
    float custo = qtd * criptos[idx].cotacao * (1 + criptos[idx].percCompra / 100.0);
    float saldo = obterSaldo(login);
    if (custo > saldo) {
        printf("Saldo insuficiente.\n"); return;
    }
    FILE *f = fopen("carteiras.txt", "a");
    fprintf(f, "%s;%s;%.2f\n", login, nome, qtd);
    fclose(f);
    salvarSaldo(login, saldo - custo);
    registrarExtrato(login, "Compra de cripto realizada.");
    printf("Compra realizada!\n");
}

void venderCripto(char *login) {
    char nome[TAM]; float qtd;
    printf("Cripto: "); scanf("%s", nome);
    printf("Qtd: "); scanf("%f", &qtd);
    int idx = buscarCripto(nome);
    if (idx == -1) return;
    float ganho = qtd * criptos[idx].cotacao * (1 - criptos[idx].percVenda / 100.0);
    float saldo = obterSaldo(login);
    saldo += ganho;
    salvarSaldo(login, saldo);
    registrarExtrato(login, "Venda de cripto realizada.");
    printf("Venda realizada. Saldo: %.2f\n", saldo);
}

void verExtrato(char *login) {
    char nomeArq[100], linha[200];
    sprintf(nomeArq, "extrato_%s.txt", login);
    FILE *f = fopen(nomeArq, "r");
    if (!f) { printf("Sem extrato.\n"); return; }
    while (fgets(linha, sizeof(linha), f)) printf("%s", linha);
    fclose(f);
}

void consultarSaldoUsuario() {
    char login[TAM];
    printf("Login do usuario para consultar saldo: ");
    scanf("%s", login);

    FILE *file = fopen("saldos.txt", "r");
    if (!file) {
        printf("Arquivo saldos.txt nao encontrado.\n");
        return;
    }

    char linha[100], user[TAM];
    float saldo;
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), file)) {
        if (sscanf(linha, "%[^;];%f", user, &saldo) == 2) {
            if (strcmp(user, login) == 0) {
                printf("Saldo de %s: R$ %.2f\n", user, saldo);
                encontrado = 1;
                break;
            }
        }
    }

    fclose(file);

    if (!encontrado) {
        printf("Usuario nao encontrado em saldos.txt.\n");
    }
}

void consultarExtratoUsuario() {
    char login[TAM];
    printf("Login do usuario para consultar extrato: ");
    scanf("%s", login);

    char nomeArquivo[TAM + 20];
    sprintf(nomeArquivo, "extrato_%s.txt", login);

    FILE *file = fopen(nomeArquivo, "r");
    if (!file) {
        printf("Extrato do usuario nao encontrado.\n");
        return;
    }

    printf("\n--- Extrato de %s ---\n", login);
    char linha[200];
    while (fgets(linha, sizeof(linha), file)) {
        printf("%s", linha);
    }
    fclose(file);
}

// --- Menus ---
void menuAdmin() {
    int opc;
    do {
        printf("\n--- Menu Administrador ---\n");
        printf("1 - Cadastrar usuario\n");
        printf("2 - Remover usuario\n");
        printf("3 - Cadastrar criptomoeda\n");
        printf("4 - Remover criptomoeda\n");
        printf("5 - Atualizar cotacoes\n");
        printf("6 - Consultar saldo de usuario\n");
        printf("7 - Consultar extrato de usuario\n");
        printf("8 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opc);
        switch (opc) {
            case 1: cadastrarUsuario(); break;
            case 2: removerUsuario(); break;
            case 3: cadastrarCripto(); break;
            case 4: removerCripto(); break;
            case 5: atualizarCotacoes(); break;
            case 6: consultarSaldoUsuario(); break;
            case 7: consultarExtratoUsuario(); break;
            case 8: printf("Saindo do menu administrador...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opc != 8);
}


void menuUser(char *login) {
    int op;
    do {
        printf("\n--- Menu Usuario ---\n1-Adic. Saldo\n2-Sacar\n3-Comprar\n4-Vender\n5-Extrato\n6-Atualizar Cotacoes\n7-Sair\n> ");
        scanf("%d", &op);
        switch (op) {
            case 1: adicionarSaldo(login); break;
            case 2: sacarSaldo(login); break;
            case 3: comprarCripto(login); break;
            case 4: venderCripto(login); break;
            case 5: verExtrato(login); break;
            case 6: atualizarCotacoes(); break;
        }
    } while (op != 7);
}

int loginSistema(char *loginOut) {
    char login[TAM], senha[TAM];
    printf("Login: "); scanf("%s", login);
    printf("Senha: "); scanf("%s", senha);
    int idx = buscarUsuario(login);
    if (idx == -1 || strcmp(usuarios[idx].senha, senha) != 0) return -1;
    strcpy(loginOut, login);
    return strcmp(usuarios[idx].tipo, "admin") == 0 ? 1 : 2;
}

int main() {
    carregarUsuarios();
    carregarCriptos();
    char login[TAM];
    int tipo = loginSistema(login);
    if (tipo == -1) { printf("Login invalido.\n"); return 0; }
    tipo == 1 ? menuAdmin() : menuUser(login);
    return 0;
}
