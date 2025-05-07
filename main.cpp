#include "BinTree.h"
#include <iostream>
#include <vector>

void printInOrder(Node<int>* node) {
    if (!node) return;
    printInOrder(node->left);
    std::cout << node->key << ' ';
    printInOrder(node->right);
}

int main() {
    std::vector<int>  arr = {1,2,3,4,5,6};
    auto tree = BinTree<int>::createFromArray(arr);
    std::cout<<"Начальное дерево: ";
    printInOrder(tree.getRoot());
    std::cout<<"\n\n";

    std::cout<<"Есть 3?  "<<(tree.contains(3)?"да":"нету")<<"\n";
    std::cout<<"Есть 10? "<<(tree.contains(10)?"да":"нет")<<"\n\n";

    auto sorted = tree.toSortArray();
    std::cout<<"вывод отсортированный: ";
    for (int x : sorted) std::cout << x << ' ';
    std::cout<<"\n\n";

    auto structArr = tree.toStructArray();
    std::cout<<"структурный массив: ";
    for (int x : structArr) {
        if (x==-1) std::cout << "_ ";
        else   std::cout << x << ' ';
    }
    std::cout << "\n\n";

    tree.erase(4);
    std::cout << "Убираем 4: ";
    printInOrder(tree.getRoot());
    std::cout << "\n\n";


    auto A=BinTree<int>::createFromArray({1,2,3,5,8});
    auto B=BinTree<int>::createFromArray({2,3,5,7,11});
    A.union_tree(B);
    std::cout<<"объединение для (1 2 3 5 8) и (2 3 5 7 11): ";
    printInOrder(A.getRoot());
    std::cout<<"\n\n";


    auto C=BinTree<int>::createFromArray({1,2,3,5,8});
    auto D=BinTree<int>::createFromArray({2,3,5,7,11});
    C.inter_tree(D);
    std::cout<<"Пересечение (1 2 3 5 8) и (2 3 5 7 11): ";
    printInOrder(C.getRoot());
    std::cout<<"\n\n";


    auto sq=tree.map([](int x){return x*x;});
    std::cout << "map ^2: ";
    printInOrder(sq.getRoot());
    std::cout << "\n\n";


    auto evens = tree.where([](int x){ return x%2==0; });
    std::cout<<"where(четные): ";
    printInOrder(evens.getRoot());
    std::cout<<"\n\n";


    int sum=tree.reduce(0, [](int acc,int x){ return acc + x; });
    int prod=tree.reduce(1, [](int acc,int x){ return acc * x; });
    std::cout<<"reduce(сумма): "<<sum<<"\n";
    std::cout<<"reduce(умножаем): "<<prod<<"\n\n";

    std::cout << "пирамидка (доп.):\n";
    tree.printPyramid();
    std::cout << "\n";

    return 0;
}
