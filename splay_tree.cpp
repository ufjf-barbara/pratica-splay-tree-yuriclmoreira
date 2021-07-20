/*
 * Programa em C++ para Implementar Splay Tree
 * Fonte: https://www.sanfoundry.com/cpp-program-implement-splay-tree/
 */
 
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;
 
struct splay
{
    int key;
    splay* lchild; // filho da esquerda
    splay* rchild; // filho da direita
};
 
class SplayTree
{
    public:
        SplayTree()
        {
        }
 
        // RR(Y rotaciona para a direita)
        splay* RR_Rotate(splay* k2)
        {
            splay* k1 = k2->lchild;
            k2->lchild = k1->rchild;
            k1->rchild = k2;
            return k1;
        }
 
        // LL(Y rotaciona para a esquerda)
        splay* LL_Rotate(splay* k2)
        {
            splay* k1 = k2->rchild;
            k2->rchild = k1->lchild;
            k1->lchild = k2;
            return k1;
        }
         
        // implementaçao de splay tree a partir da raiz
        splay* Splay(int key, splay* root)
        {
            if (!root)
                return NULL;
            splay header;
            /* header.rchild aponta para arvore L (esq) 
            header.lchild aponta para arvore R (dir) */
            header.lchild = header.rchild = NULL;
            splay* LeftTreeMax = &header;
            splay* RightTreeMin = &header;
            while (1)
            {
                if (key < root->key)
                {
                    if (!root->lchild)
                        break;
                    if (key < root->lchild->key)
                    {
                        root = RR_Rotate(root);
                        // apenas zig-zig precisa rotacionar uma vez,
                        if (!root->lchild)
                            break;
                    }
                    /* Link para arvore R (dir) */
                    RightTreeMin->lchild = root;
                    RightTreeMin = RightTreeMin->lchild;
                    root = root->lchild;
                    RightTreeMin->lchild = NULL;
                }
                else if (key > root->key)
                {
                    if (!root->rchild)
                        break;
                    if (key > root->rchild->key)
                    {
                        root = LL_Rotate(root);
                        // apenas zag-zag precisa rotacionar uma vez,
                        if (!root->rchild)
                            break;
                    }
                    /* Link para arvore L (esq) */
                    LeftTreeMax->rchild = root;
                    LeftTreeMax = LeftTreeMax->rchild;
                    root = root->rchild;
                    LeftTreeMax->rchild = NULL;
                }
                else
                    break;
            }
            /* monta arvore L (esq), arvore media e arvore R (dir) */
            LeftTreeMax->rchild = root->lchild;
            RightTreeMin->lchild = root->rchild;
            root->lchild = header.rchild;
            root->rchild = header.lchild;
            return root;
        }
 
        splay* New_Node(int key)
        {
            splay* p_node = new splay;
            if (!p_node)
            {
                fprintf(stderr, "Sem memoria\n");
                exit(1);
            }
            p_node->key = key;
            p_node->lchild = p_node->rchild = NULL;
            return p_node;
        }
 
        splay* Insert(int key, splay* root)
        {
            static splay* p_node = NULL;
            if (!p_node)
                p_node = New_Node(key);
            else
                p_node->key = key;
            if (!root)
            {
                root = p_node;
                p_node = NULL;
                return root;
            }
            root = Splay(key, root);
            /* 
            Arvore binaria de busca que, todas as chaves <= raiz->key estao na root->lchild
            todas as chaves > root->key estao na root->rchild
            */
            if (key < root->key)
            {
                p_node->lchild = root->lchild;
                p_node->rchild = root;
                root->lchild = NULL;
                root = p_node;
            }
            else if (key > root->key)
            {
                p_node->rchild = root->rchild;
                p_node->lchild = root;
                root->rchild = NULL;
                root = p_node;
            }
            else
                return root;
            p_node = NULL;
            return root;
        }
 
        splay* Delete(int key, splay* root)
        {
            splay* temp;
            if (!root)
                return NULL;
            root = Splay(key, root);
            if (key != root->key)
                return root;
            else
            {
                if (!root->lchild)
                {
                    temp = root;
                    root = root->rchild;
                }
                else
                {
                    temp = root;
                    /* Note: Como key == root->key,
                       apos Splay(key, root->lchild),
                       a arvore que temos nao tera subarvore direita.*/
                    root = Splay(key, root->lchild);
                    root->rchild = temp->rchild;
                }
                free(temp);
                return root;
            }
        }
 
        splay* Search(int key, splay* root)
        {
            return Splay(key, root);
        }
 
        void InOrder(splay* root)
        {
            if (root)
            {
                InOrder(root->lchild);
                cout<< "Chave: " <<root->key;
                if(root->lchild)
                    cout<< " | filho da esquerda: "<< root->lchild->key;
                if(root->rchild)
                    cout << " | filho da direita: " << root->rchild->key;
                cout<< "\n";
                InOrder(root->rchild);
            }
        }

        void print_tree(const string& prefix,splay* root, bool isleft){
            if (root){
                cout << prefix;
                //cout<< "Chave: " <<root->key;
                cout << (isleft ? "├──" : "└──" );
                // print the value of the node
                cout << root->key << endl;
                // enter the next tree level - left and right branch
                print_tree(prefix+(isleft ? "│   " : "    "), root->lchild, true);
                print_tree(prefix+(isleft ? "│   " : "    "), root->rchild, false);
            }
        }


};
 
int main()
{
    SplayTree st;
    int vector[10] = {9,8,7,6,5,4,3,2,1,0};
    splay *root;
    root = NULL;
    const int length = 10;
    int i;

    //insere os elementos do vetor um de cada vez
    for(i = 0; i < length; i++)
        root = st.Insert(vector[i], root);

    cout<<"\nEmOrdem: \n";
    st.InOrder(root);

    cout << "\nHorizontal:\n";
    st.print_tree("", root, false);

    int input, choice;

    while(1)
    {
        cout<<"\nOperaçoes Splay Tree:\n";
        cout<<"1. Inserir "<<endl;
        cout<<"2. Remover"<<endl;
        cout<<"3. Buscar"<<endl;
        cout<<"4. Sair"<<endl;
        cout<<"Informe sua escolha: ";
        cin>>choice;
        switch(choice)
        {
        case 1:
            cout<<"Informe o valor a ser inserido: ";
            cin>>input;
            root = st.Insert(input, root);
            cout<<"\nApos Inserir: "<<input<<endl;
            st.InOrder(root);               
            st.print_tree("", root, false);
            break;
        case 2:
            cout<<"Informe o valor a ser removido: ";
            cin>>input;
            root = st.Delete(input, root);
            cout<<"\nApos remover: "<<input<<endl;
            st.InOrder(root);
            st.print_tree("", root, false);
            break;
        case 3:
            cout<<"Informe valor a ser buscado: ";
            cin>>input;
            root = st.Search(input, root);
            cout<<"\nApos busca: "<<input<<endl;
            st.InOrder(root);
            st.print_tree("", root, false);
            break;
 
        case 4:
            exit(1);
        default:
            cout<<"\nTipo invalido! \n";
        }
    }
    cout<<"\n";
    return 0;
}