#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

#define N 10
int codC = 1;
int posCurso = 0;
int posAluno = 0;
int posProfessor = 0;

using namespace std;

struct Cursos {
    int cod;
    string nomeCurso;
    int cargaHor;
    int vagas;
    int alunosMatriculados[N];
    int totalMatriculados = 0;
};

struct Alunos {
    int matricula;
    string nomeAluno;
    int idade;
    Cursos Curso;
};

/* Nova struct Professor */
struct Professor {
    int id;
    char nome[100];
    char disciplina[100];
};

void mostrarMenu(){
    cout << "------- MENU -------\n";
    cout << "1 - Cadastrar curso.\n";
    cout << "2 - Cadastrar aluno.\n";
    cout << "3 - Listar cursos.\n";
    cout << "4 - Listar alunos.\n";
    cout << "5 - Buscar alunos por curso.\n";
    cout << "6 - Relatorio Geral.\n";
    cout << "7 - Salvar cursos (binario).\n";
    cout << "8 - Carregar cursos (binario).\n";
    cout << "9 - Salvar alunos (binario).\n";
    cout << "10 - Carregar alunos (binario).\n";
    cout << "11 - Cadastrar professor.\n";
    cout << "12 - Listar professores.\n";
    cout << "13 - Salvar professores (binario).\n";
    cout << "14 - Carregar professores (binario).\n";
    cout << "0 - Sair.\n";
}

void cadastrarCurso(Cursos *c, Cursos vet[]) {
    cin.ignore();
    cout << "Digite o nome do curso: ";
    getline(cin, c->nomeCurso);
    cout << "Digite a carga horaria do curso: ";
    cin >> c->cargaHor;
    cout << "Digite a quantidade de vagas: ";
    cin >>  c->vagas;
    c->cod = codC;
    codC++;
}

void adicionarCurso(Cursos vet[], Cursos c){
    if(posCurso < N){
        vet[posCurso++] = c;
        cout << "Curso cadastrado!\n";
    } else {
        cout << "Lista cheia. Nao foi possivel realizar o cadastro.\n";
    }
}
void listarCurso(Cursos vet[]) {
    for(int i=0; i < posCurso; i++){
        cout << "Curso COD: " << vet[i].cod << endl;
        cout << "Nome do Curso: " << vet[i].nomeCurso << endl;
        cout << "Carga horaria total: " << vet[i].cargaHor << endl;
        cout << "Quantidade de vagas: " << vet[i].vagas << endl;
        cout << "Alunos matriculados:" << vet[i].totalMatriculados << endl;
    }
}

Cursos buscarCurso(int cod, Cursos vetorCursos[]){
    for(int i=0;i<posCurso;i++){
        int codId= vetorCursos[i].cod;
        if(cod == codId){
            vetorCursos[i].totalMatriculados++;
            return vetorCursos[i];
        }
    }
    Cursos vazio;
    vazio.cod = -1;
    return vazio;
}

void cadastrarAluno(Alunos *a, Cursos vetorCursos[]){
    cout << "Digite o numero de matricula: ";
    cin >> a->matricula;
    cin.ignore();
    cout << "Digite seu nome completo: ";
    getline(cin, a->nomeAluno);
    cout << "Digite sua idade: ";
    cin >> a->idade;
    cout << "Digite o id do curso: ";
    int codA;
    cin >> codA;
    a->Curso = buscarCurso(codA, vetorCursos);
}

void adicionarAlunos(Alunos vet[], Alunos a){
    if(posAluno < N){
        vet[posAluno++] = a;
        cout << "Aluno cadastrado!\n";
    } else{
        cout << "Lista cheia. Nao foi possivel cadastrar o aluno!";
    }
}

void listarAluno(Alunos vet[]){
    for(int i=0; i < posAluno; i++){
        cout << "ALuno ID: " << i+1 << endl;
        cout << "Matricula: " << vet[i].matricula << endl;
        cout << "Nome: " << vet[i].nomeAluno << endl;
        cout << "Idade: " << vet[i].idade << endl;
        cout << "Curso: " << vet[i].Curso.nomeCurso << endl;
    }
}

void buscarAluno(int cod, Alunos vetorAlunos[]){
    for(int i=0; i<posAluno;i++){
        if(cod == vetorAlunos[i].Curso.cod){
            cout << "Nome do Aluno: " << vetorAlunos[i].nomeAluno << endl;
        }
    }
}

void relatorioGeral(Cursos vetorCursos[]) {
    cout << "----- RELATORIO GERAL -----\n";
    for (int i = 0; i < posCurso; i++) {
        cout << "Curso: " << vetorCursos[i].nomeCurso << endl;
        cout << "Alunos matriculados: " << vetorCursos[i].totalMatriculados << endl;
    }
}



void salvarCursosBin(Cursos vet[]) {
    ofstream arq("cursos.bin", ios::binary);
    if(!arq.is_open()){
        cout << "Falha ao abrir cursos.bin para escrita.\n";
        return;
    }

    arq.write((char*)&posCurso, sizeof(int));

    for(int i=0;i<posCurso;i++){
        arq.write((char*)&vet[i].cod, sizeof(int));
        int len = (int)vet[i].nomeCurso.size();
        arq.write((char*)&len, sizeof(int));
        if(len > 0) arq.write(vet[i].nomeCurso.c_str(), len);
        arq.write((char*)&vet[i].cargaHor, sizeof(int));
        arq.write((char*)&vet[i].vagas, sizeof(int));
        arq.write((char*)&vet[i].totalMatriculados, sizeof(int));
        arq.write((char*)vet[i].alunosMatriculados, sizeof(int) * N);
    }

    arq.close();
    cout << "Cursos salvos em cursos.bin\n";
}

void carregarCursosBin(Cursos vet[]) {
    ifstream arq("cursos.bin", ios::binary);
    if(!arq.is_open()){
        cout << "Falha ao abrir cursos.bin para leitura.\n";
        return;
    }

    arq.read((char*)&posCurso, sizeof(int));
    if(posCurso < 0) posCurso = 0;
    int maiorCod = 0;

    for(int i=0;i<posCurso && i < N;i++){
        arq.read((char*)&vet[i].cod, sizeof(int));
        if(vet[i].cod > maiorCod) maiorCod = vet[i].cod;
        int len = 0;
        arq.read((char*)&len, sizeof(int));
        vet[i].nomeCurso.clear();
        if(len > 0){
            string tmp;
            tmp.resize(len);
            arq.read(&tmp[0], len);
            vet[i].nomeCurso = tmp;
        }
        arq.read((char*)&vet[i].cargaHor, sizeof(int));
        arq.read((char*)&vet[i].vagas, sizeof(int));
        arq.read((char*)&vet[i].totalMatriculados, sizeof(int));
        arq.read((char*)vet[i].alunosMatriculados, sizeof(int) * N);
    }
    codC = maiorCod + 1;
    arq.close();
    cout << "Cursos carregados de cursos.bin\n";
}


void salvarAlunosBin(Alunos vet[]) {
    ofstream arq("alunos.bin", ios::binary);
    if(!arq.is_open()){
        cout << "Falha ao abrir alunos.bin para escrita.\n";
        return;
    }

    arq.write((char*)&posAluno, sizeof(int));
    for(int i=0;i<posAluno;i++){
        arq.write((char*)&vet[i].matricula, sizeof(int));
        int lenNome = (int)vet[i].nomeAluno.size();
        arq.write((char*)&lenNome, sizeof(int));
        if(lenNome > 0) arq.write(vet[i].nomeAluno.c_str(), lenNome);
        arq.write((char*)&vet[i].idade, sizeof(int));
        arq.write((char*)&vet[i].Curso.cod, sizeof(int));
        int lenCurso = (int)vet[i].Curso.nomeCurso.size();
        arq.write((char*)&lenCurso, sizeof(int));
        if(lenCurso > 0) arq.write(vet[i].Curso.nomeCurso.c_str(), lenCurso);
        arq.write((char*)&vet[i].Curso.cargaHor, sizeof(int));
        arq.write((char*)&vet[i].Curso.vagas, sizeof(int));
        arq.write((char*)&vet[i].Curso.totalMatriculados, sizeof(int));
        arq.write((char*)vet[i].Curso.alunosMatriculados, sizeof(int) * N);
    }

    arq.close();
    cout << "Alunos salvos em alunos.bin\n";
}

void carregarAlunosBin(Alunos vet[]) {
    ifstream arq("alunos.bin", ios::binary);
    if(!arq.is_open()){
        cout << "Falha ao abrir alunos.bin para leitura.\n";
        return;
    }

    arq.read((char*)&posAluno, sizeof(int));
    if(posAluno < 0) posAluno = 0;

    for(int i=0;i<posAluno && i < N;i++){
        arq.read((char*)&vet[i].matricula, sizeof(int));
        int lenNome = 0;
        arq.read((char*)&lenNome, sizeof(int));
        vet[i].nomeAluno.clear();
        if(lenNome > 0){
            string tmp;
            tmp.resize(lenNome);
            arq.read(&tmp[0], lenNome);
            vet[i].nomeAluno = tmp;
        }
        arq.read((char*)&vet[i].idade, sizeof(int));
        arq.read((char*)&vet[i].Curso.cod, sizeof(int));
        int lenCurso = 0;
        arq.read((char*)&lenCurso, sizeof(int));
        vet[i].Curso.nomeCurso.clear();
        if(lenCurso > 0){
            string tmp;
            tmp.resize(lenCurso);
            arq.read(&tmp[0], lenCurso);
            vet[i].Curso.nomeCurso = tmp;
        }
        arq.read((char*)&vet[i].Curso.cargaHor, sizeof(int));
        arq.read((char*)&vet[i].Curso.vagas, sizeof(int));
        arq.read((char*)&vet[i].Curso.totalMatriculados, sizeof(int));
        arq.read((char*)vet[i].Curso.alunosMatriculados, sizeof(int) * N);
    }

    arq.close();
    cout << "Alunos carregados de alunos.bin\n";
}

void cadastrarProfessor(Professor *p){
    cout << "ID do professor: ";
    cin >> p->id;
    cin.ignore();
    cout << "Nome do professor: ";
    cin.getline(p->nome, 100);
    cout << "Disciplina que leciona: ";
    cin.getline(p->disciplina, 100);
}

void adicionarProfessor(Professor vet[], Professor p){
    if(posProfessor < N){
        vet[posProfessor++] = p;
        cout << "Professor cadastrado!\n";
    } else {
        cout << "Lista de professores cheia.\n";
    }
}

void listarProfessor(Professor vet[]){
    if(posProfessor == 0){
        cout << "Nenhum professor cadastrado.\n";
        return;
    }
    for(int i=0;i<posProfessor;i++){
        cout << "\nID: " << vet[i].id << endl;
        cout << "Nome: " << vet[i].nome << endl;
        cout << "Disciplina: " << vet[i].disciplina << endl;
    }
}

void salvarProfessoresBin(Professor vet[]){
    ofstream arq("professores.bin", ios::binary);
    if(!arq.is_open()){
        cout << "Falha ao abrir professores.bin para escrita.\n";
        return;
    }

    arq.write((char*)&posProfessor, sizeof(int));
    for(int i=0;i<posProfessor;i++){
        arq.write((char*)&vet[i].id, sizeof(int));
        arq.write(vet[i].nome, sizeof(vet[i].nome));
        arq.write(vet[i].disciplina, sizeof(vet[i].disciplina));
    }

    arq.close();
    cout << "Professores salvos em professores.bin\n";
}

void carregarProfessoresBin(Professor vet[]){
    ifstream arq("professores.bin", ios::binary);
    if(!arq.is_open()){
        cout << "Falha ao abrir professores.bin para leitura.\n";
        return;
    }

    arq.read((char*)&posProfessor, sizeof(int));
    if(posProfessor < 0) posProfessor = 0;

    for(int i=0;i<posProfessor && i < N;i++){
        arq.read((char*)&vet[i].id, sizeof(int));
        arq.read(vet[i].nome, sizeof(vet[i].nome));
        arq.read(vet[i].disciplina, sizeof(vet[i].disciplina));
    }

    arq.close();
    cout << "Professores carregados de professores.bin\n";
}


int main() {
    Cursos cursosFacul;
    Alunos alunosFacul;

    Cursos vetorCursos[N];

    // Vinculando o aluno ao curso
    //cursosFacul.totalMatriculados  = vetorCursos[0];

    Alunos vetorAlunos[N];
    int opcao;

    // Professores
    Professor vetorProf[N];
    Professor prof;

    mostrarMenu();
    cin >> opcao;

    while(opcao != 0){
        switch(opcao) {
            case 1:
                cadastrarCurso(&cursosFacul, vetorCursos);
                adicionarCurso(vetorCursos, cursosFacul);
                break;
            case 2:
                cadastrarAluno(&alunosFacul, vetorCursos);
                adicionarAlunos(vetorAlunos, alunosFacul);
                break;
            case 3:
                listarCurso(vetorCursos);
                break;
            case 4:
                listarAluno(vetorAlunos);
                break;
            case 5:
                {
                    int buscaAluno;
                    cout << "Digite o codigo da disciplina: ";
                    cin >> buscaAluno;
                    buscarAluno(buscaAluno, vetorAlunos);
                }
                break;
            case 6:
                relatorioGeral(vetorCursos);
                break;

            case 7:
                salvarCursosBin(vetorCursos);
                break;
            case 8:
                carregarCursosBin(vetorCursos);
                break;
            case 9:
                salvarAlunosBin(vetorAlunos);
                break;
            case 10:
                carregarAlunosBin(vetorAlunos);
                break;

            case 11:
                cadastrarProfessor(&prof);
                adicionarProfessor(vetorProf, prof);
                break;
            case 12:
                listarProfessor(vetorProf);
                break;
            case 13:
                salvarProfessoresBin(vetorProf);
                break;
            case 14:
                carregarProfessoresBin(vetorProf);
                break;

            default:
                cout << "Opacao invalida. Digite novamente...\n";
                break;
        }
        mostrarMenu();
        cin >> opcao;
    }
    return 0;
}
