#ifndef BINTREE_H
#define BINTREE_H
#include <queue>
#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <utility>

static std::mt19937 rng(
    (unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count()
);
template <typename T>
struct Node {
    int size;
	int y;
    T key;
    Node<T>* parent;
    Node<T>* left;
    Node<T>* right;
    Node(const T& value) : key(value), left(nullptr),
    right(nullptr), y(rng()), size(1), parent(nullptr) {}
};
template <typename T>
class BinTree {
  public:
    BinTree() : root(nullptr) {}
    ~BinTree() { destroyLPK(root); }
    void insert(const T& key) {
        root = insert(root, new Node<T>(key));
    }

    Node<T>* getRoot() const { return root; }

    bool contains(const T& x) const {
        return find(root, x);
    }
    void erase(const T& x) {
        if (!contains(x)) {
            return;
        }
        auto [L, tmp_tr] = split_str(root, x);
        auto [del, R] = split_no_str(tmp_tr, x);
        destroyLPK(del);
        root = merge(L, R);
        if (root) {
            root->parent = nullptr;
            updateSize(root);
        }
    }

    std::vector<int> toStructArray() {
        std::vector<int> res;
        doHeapArray(root, 0 ,res);
        return res;
    }


    std::vector<T> toSortArray() const {
        std::vector<T> res;
        res.reserve(root ? root->size : 0);
        push_to_array(root, res);
        return res;
    }
    static BinTree createFromArray(const std::vector<T>& arr) {
        BinTree tr;
        for (const auto& x : arr) {
            tr.insert(x);
        }
        return tr;
    }

    void union_tree(BinTree<T>& tr){
        root = twoToOne(root, tr.root);
        tr.root = nullptr;

    }
    void inter_tree(BinTree<T>& tr) {
        root = inters(root, tr.root);
        tr.root = nullptr;
    }
    template<typename F>
    BinTree<T> map(F f) {
        BinTree<T> res;
        map_1(root, res, f);
        return res;
    }
    template<typename Pred>
    BinTree<T> where(Pred pred) {
        BinTree<T> out;
        where_1(root, out, pred);
        return out;
    }

    template<typename Itog, typename F>
    Itog reduce(Itog it, F f) const {
        return reduce_1(root, it, f);
    }
    /////////////////////////////////////////////////////////////////////////////////

    void printPyramid() const {
    if (root == nullptr) return;
    int h = height(root);

    std::vector<std::vector<Node<T>*>> levels(h);
    std::queue<std::pair<Node<T>*,int>> q;
    q.push({root,0});
    while (!q.empty()) {
        auto [n,lev] = q.front(); // берем из очереди
        q.pop(); // удаляем
        levels[lev].push_back(n);
        if (lev+1 < h) { // если еще есть куда
            Node<T>* leftChild = nullptr;
            if (n) leftChild = n->left;
            q.push(std::make_pair(leftChild, lev+1));
            Node<T>* rightChild = nullptr;
            if (n) rightChild = n->right;
            q.push(std::make_pair(rightChild, lev+1));
        }
    }

    for (int lev = 0; lev < h; ++lev) {
        int width  = (h - lev) * 2; //для расстояний между строками
        int between = (h - lev) * 2;
        std::cout << std::string(width, ' ');
        for (int i = 0; i < (int)levels[lev].size(); ++i) {
            Node<T>* n = levels[lev][i];
            if (n) std::cout << n->key;
            else   std::cout << ' ';
            if (i+1 < (int)levels[lev].size())
                std::cout << std::string(between, ' ');
        }
        std::cout << "\n\n";
    }
}


  private:
    Node<T>* root = nullptr;
    static constexpr int EMPTY = -1;

     std::pair<Node<T>*, Node<T>*> split_str(Node<T>* dt, T x) {
        if (dt == nullptr) {
            return {nullptr, nullptr};
        }
        if (dt->key < x) {
            auto [dt1, dt2] = split_str(dt->right, x);
            dt->right = dt1;
            return {dt, dt2};
        } else {
            auto [dt1, dt2] = split_str(dt->left, x);
            dt->left = dt2;
            return {dt1, dt};
        }
    }

    std::pair<Node<T>*, Node<T>*> split_no_str(Node<T>* dt, T x) {
        if (dt == nullptr) {
            return {nullptr, nullptr};
        }
            if (dt->key <= x) {
                auto [dt1, dt2] = split_no_str(dt->right, x);
                dt->right = dt1;
                return {dt, dt2};
            } else {
                auto [dt1, dt2] = split_no_str(dt->left, x);
                dt->left = dt2;
                return {dt1, dt};
            }
        }


    void doHeapArray (Node<T>* node, int ind, std::vector<int>& arr) {
        if (node == nullptr) {
            return;
        }
        if (ind >= arr.size()) {
            arr.resize(ind + 1, EMPTY);
        }
        arr[ind] = node->key;
        doHeapArray(node->left, 2*ind + 1, arr);
        doHeapArray(node->right, 2*ind + 2, arr);
    }

    static void push_to_array(Node<T>* node, std::vector<T>& out) {
            if (!node) {
                return;
            }
            push_to_array(node->left, out);
            out.push_back(node->key);
            push_to_array(node->right, out);
            return;
    }
    Node<T>* merge(Node<T>* dt1, Node<T>* dt2) {
        if (dt1 == nullptr) {
            return dt2;
        }
        if (dt2 == nullptr) {
            return dt1;
        }
        if (dt1->y > dt2->y) {
            dt1->right = merge(dt1->right, dt2);
            updateSize(dt1);
            return dt1;

        } else {
            dt2->left = merge(dt1, dt2->left);
            updateSize(dt2);
            return dt2;

        }


    }



    Node<T>* twoToOne(Node<T>* A, Node<T>* B) {
        if (A==nullptr) return B;
        if (B==nullptr) return A;
        if (A->y > B->y) {
            auto [B_lt, tmp] = split_str(B, A->key);
            auto [B_eq, B_gt] = split_no_str(tmp, A->key);
            destroyLPK(B_eq);
            A->left  = twoToOne(A->left,  B_lt);
            A->right = twoToOne(A->right, B_gt);
            updateSize(A);
            return A;
        } else {
            auto [A_lt, tmp] = split_str(A, B->key);
            auto [A_eq, A_gt] = split_no_str(tmp, B->key);
            destroyLPK(A_eq);
            B->left  = twoToOne(A_lt,  B->left);
            B->right = twoToOne(A_gt,  B->right);
            updateSize(B);
            return B;
        }
    }

    Node<T>* inters(Node<T>* A, Node<T>* B) {
        if (!A || !B) return nullptr;
        auto [L, not_A] = split_no_str(B, A->key - 1);
        auto [if_A, R] = split_no_str(not_A, A->key);

        Node<T>* left = inters(A->left, L);
        Node<T>* right = inters(A->right, R);

        if (if_A != nullptr) {
            A->left = left;
            A->right = right;
            updateSize(A);
            return A;
        } else {
            delete A;
            Node<T>* res = merge(left, right);
            return res;
        }
    }

    void setChild(Node<T>* parent, Node<T>* child, bool isLeft) {
        if (parent) {
            if (isLeft) parent->left = child;
            else parent->right = child;
        }
        if (child) child->parent = parent;
    }
    void updateSize(Node<T>* node) {
        if (node == nullptr) return;
        int l_node = 0;
        if (node->left != nullptr) {
            l_node = node->left->size;
        }
        int r_node = 0;
        if (node->right != nullptr) {
            r_node = node->right->size;
        }
        node->size = 1 + l_node + r_node;
    }

    Node<T>* insert(Node<T>* root, Node<T>* x) {
        if (root == nullptr) {
            return x;
        }
        if (x->y > root->y) {
            auto [L, R] = split_str(root, x->key);
            setChild(x, L, true);
            setChild(x, R, false);
            updateSize(x);
            return x;
        }
        if (x->key < root->key) {
            setChild(root, insert(root->left, x), true);

        }else setChild(root, insert(root->right, x), false);
        updateSize(root);
        return root;
    }

    template<typename F>
    static void map_1(Node<T>* tr, BinTree<T>& out, F f) {
        if (tr == nullptr) return;
        map_1(tr->left,  out, f);
        out.insert(f(tr->key));
        map_1(tr->right, out, f);
    }

    template<typename Pred>
    static void where_1(Node<T>* node, BinTree<T>& out, Pred pred) {
        if (node == nullptr) return;
        where_1(node->left,  out, pred);
        if (pred(node->key))
            out.insert(node->key);
        where_1(node->right, out, pred);
    }



    void destroyLPK(Node<T>* node) {
        if (!node) return;
        destroyLPK(node->left);
        destroyLPK(node->right);
        delete node;
    }

    bool find(Node<T>* root, T x) const {
        if (root == nullptr) return false;
        if (root->key == x) {
            return true;
        }
        if (root->key > x) {
            return find(root->left, x);
        } else
            return find(root->right, x);
    }

    template<typename Itog, typename F>
    static Itog reduce_1(Node<T>* node, Itog it, F f) {
        if (node == nullptr) return it;
        it = reduce_1(node->left,  it, f);
        it = f(it, node->key);
        return reduce_1(node->right, it, f);
    }

    int height(Node<T>* tr) const {
        if (tr == nullptr) return 0;
        return 1 + std::max(height(tr->left), height(tr->right));
    }

};




#endif //BINTREE_H
