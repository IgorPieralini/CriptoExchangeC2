# 💸 Projeto Exchange de Criptomoedas (C)

Uma exchange fictícia de criptomoedas desenvolvida em linguagem C, com funcionalidades completas para **usuários** e **administradores**. Todos os dados são armazenados em arquivos `.txt`, simulando um banco de dados simples.

---

## 👥 Tipos de Acesso

🔐 **Login único**: o programa detecta se o usuário é **administrador** ou **investidor comum** com base no CPF e senha cadastrados em `usuarios.txt`.

---

## 🧑‍💼 Área do Usuário

Usuários (investidores) têm acesso a funcionalidades para movimentar saldo e operar criptomoedas.

### Funcionalidades:

* ➕ **Adicionar saldo**
* ➖ **Sacar saldo**
* 💰 **Comprar criptomoedas** (com taxa de operação)
* 💱 **Vender criptomoedas** (com taxa de operação)
* 📄 **Ver extrato de operações**
* 📈 **Atualizar cotações de mercado**

🗂️ **Extratos** são salvos como `extrato_<login>.txt`.

---

## 🚰 Área do Administrador

Administradores têm acesso a ferramentas de controle sobre usuários e criptomoedas.

### Funcionalidades:

* 👤 **Cadastrar usuário**
* ❌ **Remover usuário**
* 🪙 **Cadastrar criptomoeda**
* 🔻 **Remover criptomoeda**
* 🔄 **Atualizar cotações de criptomoedas**
* 💼 **Consultar saldo de qualquer usuário**
* 🧾 **Consultar extrato de qualquer usuário**

---

## 📁 Arquivos Utilizados

| Arquivo               | Função                                         |
| --------------------- | ---------------------------------------------- |
| `usuarios.txt`        | Armazena login, senha, CPF e tipo de usuário   |
| `saldos.txt`          | Guarda o saldo de cada usuário por login       |
| `cripto.txt`          | Lista as criptomoedas, cotações e taxas        |
| `extrato_<login>.txt` | Guarda histórico de transações de cada usuário |

---

## ⚡ Tecnologias

* Linguagem C
* Compilado e testado no [Replit](https://replit.com/)
* Armazenamento em arquivos `.txt`

---

## ✉ Contato

Feito com por \[Igor Marques Pieralini, Victor Augusto Caramori André] [24.224.003-4,24.224.009-1]
